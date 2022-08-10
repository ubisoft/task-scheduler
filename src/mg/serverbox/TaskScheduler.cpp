// Copyright (C) 2022 Ubisoft Entertainment
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 2 of
// the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this program.  If not, see
// <https://www.gnu.org/licenses/>.

#include "TaskScheduler.h"

#include "mg/common/ForwardList.h"
#include "mg/common/ScratchPad.h"
#include "mg/common/Time.h"

namespace mg {
namespace serverbox {

	using TaskForwardList = mg::common::ForwardList<Task>;

	TaskScheduler::TaskScheduler(
		const char* aName,
		uint32 aThreadCount,
		uint32 aSubQueueSize)
		: myQueueReady(aSubQueueSize)
		, myIsSchedulerWorking(0)
		, myIsStopped(0)
	{
		myThreads.SetCount(aThreadCount);
		for (uint32 i = 0; i < aThreadCount; ++i)
		{
			myThreads[i] = new TaskSchedulerThread(aName, this);
			myThreads[i]->Start();
		}
	}

	TaskScheduler::~TaskScheduler()
	{
		mg::common::AtomicFlagSet(&myIsStopped);
		for (int i = 0, count = myThreads.Count(); i < count; ++i)
			myThreads[i]->Stop();
		// It is enough to wake the sched-thread. It will wakeup
		// another worker, and they will wakeup each other like
		// domino.
		mySignalFront.Send();
		for (int i = 0, count = myThreads.Count(); i < count; ++i)
			myThreads[i]->StopAndDelete();
		MG_COMMON_ASSERT(myQueueFront.PopAllFastReversed() == nullptr);
		MG_COMMON_ASSERT(myQueueWaiting.Count() == 0);
		MG_COMMON_ASSERT(myQueueReady.Count() == 0);
	}

	void
	TaskScheduler::Reserve(
		uint32 aCount)
	{
		myQueueReady.Reserve(aCount);
	}

	void
	TaskScheduler::Post(
		Task* aTask)
	{
		MG_COMMON_ASSERT(!aTask->myIsInQueues);
		aTask->myIsInQueues = true;
		PrivPost(aTask);
	}

	void
	TaskScheduler::Wakeup(
		Task* aTask)
	{
		// Don't do the load inside of the cycle. It is needed
		// only first time. On next iterations the cmpxchg returns
		// the old value anyway.
		int32 old;
		int32 oldNext = mg::common::AtomicLoad(&aTask->myStatus);
		// Note, that the loop is not a busy loop nor a spin-lock.
		// Because it would mean the thread couldn't progress
		// until some other thread does something. Here, on the
		// contrary, the thread does progress always, and even
		// better if other threads don't do anything. Should be
		// one iteration in like 99.9999% cases.
		do
		{
			old = oldNext;
			// Signal and ready mean the task will be executed
			// ASAP anyway. Also can't override the signal,
			// because it is stronger than a wakeup.
			if (old == TASK_STATUS_SIGNALED || old == TASK_STATUS_READY)
				return;
		} while ((oldNext = mg::common::AtomicCompareExchange(&aTask->myStatus,
			TASK_STATUS_READY, old)) != old);

		// If the task was in the waiting queue. Need to re-push
		// it to let the scheduler know the task must be removed
		// from the queue earlier.
		if (old == TASK_STATUS_WAITING)
			PrivPost(aTask);
	}

	void
	TaskScheduler::Signal(
		Task* aTask)
	{
		int32 old = mg::common::AtomicExchange(&aTask->myStatus, TASK_STATUS_SIGNALED);
		// WAITING - the task was in the waiting queue. Need to
		// re-push it to let the scheduler know the task must be
		// removed from the queue earlier.
		if (old == TASK_STATUS_WAITING)
			PrivPost(aTask);
	}

	inline void
	TaskScheduler::PrivPost(
		Task* aTask)
	{
		MG_COMMON_ASSERT(aTask->myIsInQueues);
		if (myQueueFront.Push(aTask))
			mySignalFront.Send();
	}

	bool
	TaskScheduler::PrivSchedule()
	{
		if (mg::common::AtomicFlagSet(&myIsSchedulerWorking) != 0)
			return false;

		int32 old;
		Task* t;
		Task* next;
		TaskForwardList ready;
		uint64 deadline;
		uint64 timestamp = mg::common::GetMilliseconds();

	retry:
		if (PrivIsStopped())
			goto end;

		// -------------------------------------------------------
		// Handle waiting tasks. They are older than the ones in
		// the front queue, so must be handled first.

		while (myQueueWaiting.Count() > 0)
		{
			t = myQueueWaiting.GetTop();
			if (t->myDeadline > timestamp)
				break;
			myQueueWaiting.RemoveTop();
			t->myIsExpired = true;

			old = mg::common::AtomicCompareExchange(&t->myStatus, TASK_STATUS_READY,
				TASK_STATUS_WAITING);
			if (old == TASK_STATUS_READY || old == TASK_STATUS_SIGNALED)
			{
				// The task is woken up explicitly or signaled. It
				// means the waker thread saw the task in WAITING
				// state.
				//
				// Protocol for wakers in that case is to push the
				// task to the front queue.
				//
				// It means the task can't be added to the ready
				// queue until it is collected from the front
				// queue. Otherwise it is possible to end up with
				// the task added to the ready queue below and at
				// the same time to the front queue by the waker
				// thread.
				continue;
			}

			MG_COMMON_ASSERT(old == TASK_STATUS_WAITING);
			ready.Append(t);
		}

		// -------------------------------------------------------
		// Handle front tasks. They are popped unordered to make
		// it faster. But the reversed order is ok because of 2
		// reasons:
		//
		// - For WAITING tasks order of insertion into the binary
		//   heap does not matter, they are re-sorted anyway
		//   inside of the heap into its own order;
		//
		// - For other tasks the order matters. But the loop below
		//   will reverse them while scanning the popped list, in
		//   one pass.

		mySignalFront.Receive();
		t = myQueueFront.PopAllFastReversed();

		while (t != nullptr)
		{
			next = t->myNext;
			t->myNext = nullptr;

			if (timestamp < t->myDeadline)
			{
				t->myIsExpired = false;
				old = mg::common::AtomicCompareExchange(&t->myStatus, TASK_STATUS_WAITING,
					TASK_STATUS_PENDING);
				if (old == TASK_STATUS_PENDING)
				{
					// The task is not added to the heap in case
					// it is put to sleep until explicit wakeup or
					// a signal. Because anyway it won't be popped
					// ever. And signal/wakeup work fine even if
					// the task is waiting but not in the waiting
					// queue. Only status matters.
					if (t->myDeadline != MG_DEADLINE_INFINITE)
						myQueueWaiting.Push(t);
					else
						MG_COMMON_ASSERT(t->myIndex == -1);
					// Even if the task is woken right now, it is
					// ok to add it to the waiting queue. Because
					// it is also added to the front queue by the
					// wakeup, and the scheduler handles this case
					// below.
					t = next;
					continue;
				}
				MG_COMMON_ASSERT(
					// The task was woken up or signaled
					// specifically to ignore the deadline.
					old == TASK_STATUS_READY ||
					old == TASK_STATUS_SIGNALED);
			}
			else
			{
				t->myIsExpired = true;
				old = mg::common::AtomicCompareExchange(
					&t->myStatus, TASK_STATUS_READY, TASK_STATUS_PENDING
				);
				MG_COMMON_ASSERT(
					// Normal task reached its dispatch.
					old == TASK_STATUS_PENDING ||
					// The task was woken up or signaled
					// explicitly.
					old == TASK_STATUS_READY ||
					old == TASK_STATUS_SIGNALED);
			}
			// The task can be also stored in the waiting queue
			// and then pushed to the front queue to wake it up or
			// signal earlier. In this case it must be removed
			// from the waiting queue. A task never should be in
			// two queues simultaneously.
			if (t->myIndex >= 0)
				myQueueWaiting.Remove(t);

			// Here the front tasks are reversed second time.
			// First time was at their pop from the front queue.
			// So reverse + reverse = original order.
			ready.Prepend(t);
			t = next;
		}

		// End of tasks polling.
		// -------------------------------------------------------

		t = ready.PopAll();
		while (t != nullptr)
		{
			next = t->myNext;
			t->myNext = nullptr;
			myQueueReady.PushPending(t);
			t = next;
		}
		myQueueReady.FlushPending();

		if (myQueueReady.Count() == 0)
		{
			// No ready tasks means the other workers already
			// sleep on ready-signal. Or are going to start
			// sleeping any moment. So the sched can't quit. It
			// must retry until either a front task appears, or
			// one of the waiting tasks' deadline is expired.
			if (myQueueWaiting.Count() == 0)
			{
				mySignalFront.ReceiveBlocking();
			}
			else
			{
				deadline = myQueueWaiting.GetTop()->myDeadline;
				timestamp = mg::common::GetMilliseconds();
				if (deadline > timestamp)
					mySignalFront.ReceiveTimed((uint32) (deadline - timestamp));
			}
			goto retry;
		}

	end:
		mg::common::AtomicFlagClear(&myIsSchedulerWorking);

		// The signal is absolutely vital to have exactly here.
		// If the signal would not be emitted here, all the
		// workers could block on ready tasks in their loops.
		//
		// It could easily happen like this. Assume there are 2
		// threads. One becomes the scheduler, schedules one task
		// above, and is paused by the OS right *before* the
		// scheduler flag is freed.
		//
		// Then the second thread starts working, executes the
		// ready task, fails to become a sched (the flag is still
		// taken), and goes to infinite sleep waiting for ready
		// tasks.
		//
		// Then the first thread continues execution, frees the
		// flag, also fails to execute anything and also goes to
		// infinite sleep on the same condition.
		//
		// The signal below fixes it so if a thread finished the
		// scheduling, it will either go and execute tasks, or
		// will go sleep, but the signal will wake it again or
		// some another thread to elect a new sched-thread.
		//
		// On the other hand it does not lead to busy-loop. If
		// there are no new expired tasks, eventually one worker
		// will stuck in scheduler role on waiting for new tasks,
		// and other workers will sleep on waiting for ready
		// tasks.
		PrivSignalReady();
		return true;
	}

	bool
	TaskScheduler::PrivExecute(
		Task* aTask)
	{
		if (aTask == nullptr)
			return false;
		MG_COMMON_ASSERT(aTask->myIsInQueues);
		aTask->myIsInQueues = false;
		int32 old = mg::common::AtomicCompareExchange(&aTask->myStatus,
			TASK_STATUS_PENDING, TASK_STATUS_READY);
		MG_COMMON_ASSERT(old == TASK_STATUS_READY || old == TASK_STATUS_SIGNALED);
		// The task object shall not be accessed anyhow after
		// execution. It may be deleted inside.
		aTask->PrivExecute();
		return true;
	}

	void
	TaskScheduler::PrivWaitReady()
	{
		mySignalReady.ReceiveBlocking();
	}

	void
	TaskScheduler::PrivSignalReady()
	{
		mySignalReady.Send();
	}

	bool
	TaskScheduler::PrivIsStopped()
	{
		return mg::common::AtomicFlagTest(&myIsStopped) != 0;
	}

	TaskSchedulerThread::TaskSchedulerThread(
		const char* aSchedulerName,
		TaskScheduler* aScheduler)
		: Thread(mg::common::ScratchSprintf("mgsb.tsksch%s", aSchedulerName))
		, myScheduler(aScheduler)
		, myExecuteCount(0)
		, myScheduleCount(0)
	{
		myConsumer.Attach(&myScheduler->myQueueReady);
	}

	void
	TaskSchedulerThread::Run()
	{
		while (!myScheduler->PrivIsStopped())
		{
			if (myScheduler->PrivSchedule())
				mg::common::AtomicIncrement64(&myScheduleCount);

			int64 execCount = 0;
			while (myScheduler->PrivExecute(myConsumer.Pop()))
				++execCount;
			mg::common::AtomicAdd64(&myExecuteCount, execCount);

			myScheduler->PrivWaitReady();
		}
		myScheduler->PrivSignalReady();
	}

	void
	TaskOneShot::Execute(
		Task* aTask)
	{
		MG_COMMON_ASSERT(aTask == this);
		myCallback();
		delete this;
	}

}
}
