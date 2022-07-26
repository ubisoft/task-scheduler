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

#pragma once

#include "mg/common/BinaryHeap.h"
#include "mg/common/MultiConsumerQueue.h"
#include "mg/common/MultiProducerQueueIntrusive.h"
#include "mg/common/Signal.h"
#include "mg/common/Thread.h"

#include "mg/serverbox/Task.h"

namespace mg {
namespace serverbox {

	// Front queue is populated by external threads and by worker
	// threads. So it is multi-producer. It is dispatched by
	// sched-thread only, so it is single-consumer. Each task goes
	// firstly to the front queue.
	using TaskSchedulerQueueFront = mg::common::MultiProducerQueueIntrusive<Task>;
	// Waiting queue is only used by one worker thread at a time,
	// so it has no concurrent access and is therefore not
	// protected with a lock. Tasks move from the front queue to
	// the waiting queue only if they have a deadline in the
	// future.
	using TaskSchedulerQueueWaiting = mg::common::BinaryHeapMinIntrusive<Task>;
	// Ready queue is populated only by the sched-thread and
	// consumed by all workers when dispatching tasks. Tasks move
	// to the ready queue from either the front queue if a task is
	// due to execute right away or from the waiting queue if a
	// task deadline is met (or it was woken up or signaled).
	using TaskSchedulerQueueReady = mg::common::MultiConsumerQueue<Task>;
	using TaskSchedulerQueueReadyConsumer = mg::common::MultiConsumerQueueConsumer<Task>;

	// Special type to post callbacks not bound to a task. The
	// scheduler creates tasks for them inside. Keep in mind, that
	// **it is not free**, as the scheduler will need to allocate
	// the task object on its own, and it adds +1 virtual call,
	// because the one-shot callback is called via a normal task
	// callback. Avoid it for all perf-critical code.
	using TaskCallbackOneShot = mg::common::Callback<void(void)>;

	class TaskSchedulerThread;

	// Scheduler for asynchronous execution of tasks. Can be used
	// for tons of one-shot short-living tasks, as well as for
	// long-living periodic tasks with deadlines.
	class TaskScheduler
	{
	public:
		// The name is displayed as a part of the worker thread
		// names. It should be not longer than 4 symbols to be
		// displayed not truncated. See Thread.cpp for appropriate
		// shortcuts.
		// Sub-queue size is used by the ready-queue dispatched by
		// the worker threads. The bigger is the size, the smaller
		// is contention between the workers. But too big size may
		// lead to huge cache misses and memory overuse. A few
		// thousands (1-5) usually is fine.
		TaskScheduler(
			const char* aName,
			uint32 aThreadCount,
			uint32 aSubQueueSize);

		~TaskScheduler();

		// Post as is. The task can be configured using its Set
		// methods before the Post.
		void Post(
			Task* aTask);

		// You must prefer Deadline or Wait if you want an
		// infinite delay. To avoid unnecessary current time get.
		void PostDelay(
			Task* aTask,
			uint32 aDelay);

		void PostDeadline(
			Task* aTask,
			uint64 aDeadline);

		void PostWait(
			Task* aTask);

		template<typename... Args>
		void PostOneShot(
			Args&&... aArgs);

		// Wakeup guarantees that the task will be executed again
		// without a delay, at least once.
		//
		// However won't affect a signaled task. Because if the
		// task is signaled during execution, then was woken up,
		// then consumed the signal **in the same execution**,
		// then the wakeup didn't do anything. Without a signal
		// the wakeup would make an effect even during execution.
		//
		// Typical usecase for Wakeup() is when your task
		// schedules some async work to another thread and goes to
		// infinite sleep using Wait. It needs to be woken up when
		// the work is done to progress further. Also it can be
		// used for cancellations if you protect the task with
		// reference counting.
		//
		// ## Attention!!!
		//
		// Be extremely careful, if you use Wakeup to reschedule a
		// task sleeping on a deadline, when some work is done.
		// This is how it may look in a naive way:
		//
		//     void
		//     FinishSomeWork()
		//     {
		//         isReady = true;                          // (1)
		//         scheduler.Wakeup(task);                  // (2)
		//     }
		//
		//     void
		//     TaskBody(Task* aTask)
		//     {
		//         if (isReady)
		//         {
		//             // Work is done.
		//             delete this;
		//             return;
		//         }
		//         // Timed out.
		//         assert(aTask->IsExpired());
		//         isCanceled = true;
		//         // Wait for the work cancel.
		//		   scheduler.PostWait(aTask);
		//     }
		//
		//     void
		//     TaskStart()
		//     {
		//         scheduler.PostDeadline(new Task(TaskBody), 30 * 1000);
		//     }
		//
		// This code is broken. Because after (1) the task may be
		// woken up by a timeout, will see isReady, and will
		// delete itself. Then Wakeup() in (2) will crash.
		//
		// Solutions for that:
		// - Use ref counting and delete the task on last ref;
		// - Use Signal (see Signal method);
		// - Don't use deadlines and delays.
		//
		// ## Note about deletion
		//
		// Deletion can be heavy, and you may want to do it in a
		// worker thread always. Then you can't use reference
		// counting + Wakeup(). The canceling thread will need to
		// call Wakeup() + unref, and may end up removing the last
		// ref, if the task woke up fast and removed the
		// next-to-last ref. Then you have to delete the task not
		// in the worker thread.
		//
		// Solutions for that:
		// - Post a new 'deletion' task on last ref removal;
		// - Use Signal.
		//
		void Wakeup(
			Task* aTask);

		// Signal is a communication channel between the task and
		// any other code. It allows to forcefully wakeup a task
		// and set a 'signal' flag **atomically**, which the task
		// won't be able to miss anyhow. Meaning of the signal can
		// be anything you want - a sub-task completion, a future
		// being ready, or whatever else. Signal is completely
		// lock-free, so you should prefer Signal instead of
		// thread synchronization objects (with locks) when
		// possible.
		//
		// Almost always you need Signal instead of Wakeup.
		//
		// Here is a bit modified example from Wakeup doc, but
		// using the signal to tell the task that the sub-task was
		// completed:
		//
		//     void
		//     FinishSomeWork()
		//     {
		//         isReady = true;
		//         result = 'Success';
		//         Signal(task);
		//     }
		//
		//     void
		//     TaskBody(Task* aTask)
		//     {
		//         if (aTask->ReceiveSignal())
		//         {
		//             // Work is done.
		//             assert(isReady);                     // (1)
		//             printf("%s", result);                // (2)
		//             delete this;
		//             return;
		//         }
		//         // Timed out.
		//         isCanceled = true;
		//         // Wait for the work cancel.
		//		   scheduler.PostWait(aTask);
		//     }
		//
		//     void
		//     TaskStart()
		//     {
		//         scheduler.PostDeadline(new Task(TaskBody), 30 * 1000);
		//     }
		//
		// You can be sure, that in (1) the signal sender does not
		// use the task anymore. It can be safely deleted or
		// re-scheduled, you can reuse isReady, etc.
		//
		// Despite signal is just a flag, you can associate it
		// with any other more complex objects. For example, you
		// can be sure that in (2) the result is here.
		//
		// If the task wants to continue after signal (for
		// example, to cleanup some stuff before termination), it
		// must ReceiveSignal(). Then it can proceed, and the
		// signal can be re-used again.
		//
		void Signal(
			Task* aTask);

		// For statistics collection only.
		TaskSchedulerThread*const* GetThreads(
			uint32& aOutCount) const;

	private:
		void PrivPost(
			Task* aTask);

		bool PrivSchedule();

		bool PrivExecute(
			Task* aTask);

		void PrivWaitReady();

		void PrivSignalReady();

		bool PrivIsStopped();

		// Each task firstly goes to the front queue, from where
		// it is dispatched to the other queues by sched-thread.
		TaskSchedulerQueueFront myQueueFront;
		mg::common::Signal mySignalFront;

		// Front queue is very hot, it is updated by all threads -
		// external ones and workers. Should not invalidate the
		// cache of the less intensively updated fields. It is
		// separated from the fields below by thread list, which
		// is almost never updated or read. So can be used as a
		// barrier.
		mg::common::HybridArray<TaskSchedulerThread*, 8> myThreads;

		TaskSchedulerQueueWaiting myQueueWaiting;
		TaskSchedulerQueueReady myQueueReady;
		mg::common::Signal mySignalReady;

		// The pending and waiting tasks must be dispatched
		// somehow to be moved to the ready queue. For that there
		// is a 'scheduling process'. It is not pinned to any
		// thread, but instead it migrates between worker threads
		// as soon as one of them does not have ready tasks to do.
		//
		// On one hand that allows to avoid having a separate
		// scheduler thread. On the other hand it still allows not
		// to use any locks for the objects used for the
		// scheduling only, such as the waiting queue.
		//
		// The worker thread, doing the scheduling right now, is
		// called 'sched-thread' throughout the code.
		int32 myIsSchedulerWorking;
		int32 myIsStopped;

		friend class TaskSchedulerThread;
	};

	class TaskSchedulerThread
		: public mg::common::Thread
	{
	public:
		TaskSchedulerThread(
			const char* aSchedulerName,
			TaskScheduler* aScheduler);

		uint64 StatPopExecuteCount();

		uint64 StatPopScheduleCount();

	private:
		void Run() override;

		TaskScheduler* myScheduler;
		TaskSchedulerQueueReadyConsumer myConsumer;
		int64 myExecuteCount;
		int64 myScheduleCount;
	};

	struct TaskOneShot
		: public Task
	{
		template<typename... Args>
		TaskOneShot(
			Args&&...);

		void Execute(
			Task* aTask);

		TaskCallbackOneShot myCallback;
	};

	inline void
	TaskScheduler::PostDelay(
		Task* aTask,
		uint32 aDelay)
	{
		aTask->SetDelay(aDelay);
		Post(aTask);
	}

	inline void
	TaskScheduler::PostDeadline(
		Task* aTask,
		uint64 aDeadline)
	{
		aTask->SetDeadline(aDeadline);
		Post(aTask);
	}

	inline void
	TaskScheduler::PostWait(
		Task* aTask)
	{
		aTask->SetWait();
		Post(aTask);
	}

	inline TaskSchedulerThread*const*
	TaskScheduler::GetThreads(
		uint32& aOutCount) const
	{
		aOutCount = myThreads.Count();
		return myThreads.GetBuffer();
	}

	inline uint64
	TaskSchedulerThread::StatPopExecuteCount()
	{
		return (uint64)mg::common::AtomicExchange64(&myExecuteCount, 0);
	}

	inline uint64
	TaskSchedulerThread::StatPopScheduleCount()
	{
		return (uint64)mg::common::AtomicExchange64(&myScheduleCount, 0);
	}

	template<typename... Args>
	inline void
	TaskScheduler::PostOneShot(
		Args&&... aArgs)
	{
		Post(new TaskOneShot(mg::common::Forward<Args>(aArgs)...));
	}

	template<typename... Args>
	inline
	TaskOneShot::TaskOneShot(
		Args&&... aArgs)
		: Task(this, &TaskOneShot::Execute)
		, myCallback(mg::common::Forward<Args>(aArgs)...)
	{
	}

}
}
