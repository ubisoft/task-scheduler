#include "TaskScheduler.h"

#include "mg/common/Time.h"

namespace mg {
namespace serverbox {

	TaskScheduler::TaskScheduler(
		const char* aName,
		uint32_t aThreadCount,
		uint32_t aSubQueueSize)
		: myQueueReady(aSubQueueSize)
		, myExecBatchSize(aSubQueueSize)
		, mySchedBatchSize(aSubQueueSize * aThreadCount)
		, myIsSchedulerWorking(0)
		, myIsStopped(0)
	{
		myThreads.resize(aThreadCount);
		for (TaskSchedulerThread*& t : myThreads)
		{
			t = new TaskSchedulerThread(aName, this);
			t->Start();
		}
	}

	TaskScheduler::~TaskScheduler()
	{
		mg::common::AtomicFlagSet(&myIsStopped);
		for (TaskSchedulerThread* t : myThreads)
			t->Stop();
		// It is enough to wake the sched-thread. It will wakeup
		// another worker, and they will wakeup each other like
		// domino.
		mySignalFront.Send();
		for (TaskSchedulerThread* t : myThreads)
			t->StopAndDelete();
		MG_COMMON_ASSERT(myQueuePending.IsEmpty());
		MG_COMMON_ASSERT(myQueueFront.PopAllFastReversed() == nullptr);
		MG_COMMON_ASSERT(myQueueWaiting.Count() == 0);
		MG_COMMON_ASSERT(myQueueReady.Count() == 0);
	}

	void
	TaskScheduler::Reserve(
		uint32_t aCount)
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
		int32_t old;
		int32_t oldNext = mg::common::AtomicLoad(&aTask->myStatus);
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
		int32_t old = mg::common::AtomicExchange(&aTask->myStatus, TASK_STATUS_SIGNALED);
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

		int32_t old;
		Task* t;
		Task* next;
		Task* tail;
		TaskSchedulerQueuePending ready;
		uint64_t deadline;
		uint64_t timestamp = mg::common::GetMilliseconds();
		uint32_t batch;
		uint32_t maxBatch = mySchedBatchSize;

	retry:
		if (PrivIsStopped())
			goto end;

		// -------------------------------------------------------
		// Handle waiting tasks. They are older than the ones in
		// the front queue, so must be handled first.

		batch = 0;
		while (myQueueWaiting.Count() > 0 && ++batch < maxBatch)
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
		// Handle front tasks.

		mySignalFront.Receive();
		// Popping the front queue takes linear time due to how the multi-producer queue
		// is implemented. It is not batched so far, but even for millions of tasks it is
		// a few milliseconds tops.
		t = myQueueFront.PopAll(tail);
		myQueuePending.Append(t, tail);
		batch = 0;
		while (!myQueuePending.IsEmpty() && ++batch < maxBatch)
		{
			t = myQueuePending.PopFirst();
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
			ready.Append(t);
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

		if (myQueueReady.Count() == 0 && myQueuePending.IsEmpty())
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
					mySignalFront.ReceiveTimed((uint32_t)(deadline - timestamp));
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
		int32_t old = mg::common::AtomicCompareExchange(&aTask->myStatus,
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
		: Thread(mg::common::StringFormat(
			"mgsb.tsksch%s", aSchedulerName).c_str())
		, myScheduler(aScheduler)
		, myExecuteCount(0)
		, myScheduleCount(0)
	{
		myConsumer.Attach(&myScheduler->myQueueReady);
	}

	void
	TaskSchedulerThread::Run()
	{
		int64_t maxBatch = myScheduler->myExecBatchSize;
		int64_t batch;
		while (!myScheduler->PrivIsStopped())
		{
			do
			{
				if (myScheduler->PrivSchedule())
					mg::common::AtomicIncrement64(&myScheduleCount);
				batch = 0;
				while (myScheduler->PrivExecute(myConsumer.Pop()) && ++batch < maxBatch);
				mg::common::AtomicAdd64(&myExecuteCount, batch);
			} while (batch == maxBatch);
			MG_COMMON_ASSERT(batch < maxBatch);

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
