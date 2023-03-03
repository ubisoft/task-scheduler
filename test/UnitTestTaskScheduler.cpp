#include "UnitTest.h"

#include "mg/common/QPTimer.h"
#include "mg/common/Random.h"
#include "mg/common/Time.h"

#include "mg/serverbox/TaskScheduler.h"

namespace mg {
namespace unittests {

	static void
	UnitTestTaskSchedulerBasic()
	{
		TestCaseGuard guard("Basic");

		mg::serverbox::TaskScheduler sched("tst", 1, 5);
		mg::serverbox::TaskCallback cb;
		mg::serverbox::Task* tp;
		int32_t progress;

		// Simple test for a task being executed 3 times.
		progress = 0;
		cb.Set([&](mg::serverbox::Task* aTask) {
			MG_COMMON_ASSERT(aTask == tp);
			MG_COMMON_ASSERT(aTask->IsExpired());
			if (mg::common::AtomicIncrement(&progress) < 3)
				sched.Post(aTask);
		});
		mg::serverbox::Task t(cb);
		tp = &t;
		sched.Post(&t);
		while (mg::common::AtomicLoad(&progress) != 3)
			mg::common::Sleep(1);

		// Delay should be respected.
		uint64_t timestamp = mg::common::GetMilliseconds();
		progress = 0;
		cb.Set([&](mg::serverbox::Task* aTask) {
			MG_COMMON_ASSERT(aTask == tp);
			MG_COMMON_ASSERT(aTask->IsExpired());
			MG_COMMON_ASSERT(mg::common::GetMilliseconds() >= timestamp + 5);
			mg::common::AtomicFlagSet(&progress);
		});
		t.SetCallback(cb);
		t.SetDelay(5);
		sched.Post(&t);
		while (!mg::common::AtomicFlagTest(&progress))
			mg::common::Sleep(1);

		// Deadline should be respected.
		timestamp = 0;
		progress = 0;
		cb.Set([&](mg::serverbox::Task* aTask) {
			MG_COMMON_ASSERT(aTask == tp);
			MG_COMMON_ASSERT(aTask->IsExpired());
			if (timestamp == 0)
			{
				timestamp = mg::common::GetMilliseconds() + 5;
				return sched.PostDeadline(aTask, timestamp);
			}
			MG_COMMON_ASSERT(mg::common::GetMilliseconds() >= timestamp);
			mg::common::AtomicFlagSet(&progress);
		});
		t.SetCallback(cb);
		sched.Post(&t);
		while (!mg::common::AtomicFlagTest(&progress))
			mg::common::Sleep(1);

		// Task can delete itself before return.
		progress = 0;
		tp = new mg::serverbox::Task();
		cb.Set([&](mg::serverbox::Task* aTask) {
			MG_COMMON_ASSERT(aTask == tp);
			MG_COMMON_ASSERT(aTask->IsExpired());
			mg::common::AtomicFlagSet(&progress);
			delete aTask;
		});
		tp->SetCallback(cb);
		sched.Post(tp);
		while (!mg::common::AtomicFlagTest(&progress))
			mg::common::Sleep(1);

		// Task's callback can be created in-place.
		progress = 0;
		tp = new mg::serverbox::Task([&](mg::serverbox::Task* aTask) {
			cb(aTask);
		});
		sched.Post(tp);
		while (!mg::common::AtomicFlagTest(&progress))
			mg::common::Sleep(1);
	}

	static void
	UnitTestTaskSchedulerOrder()
	{
		TestCaseGuard guard("Order");

		// Order is never guaranteed in a multi-thread system. But
		// at least it should be correct when the thread is just
		// one.
		mg::serverbox::TaskScheduler sched("tst", 1, 5);
		mg::serverbox::TaskCallback cb;
		mg::serverbox::Task t1;
		mg::serverbox::Task t2;
		mg::serverbox::Task t3;
		int32_t progress = 0;
		cb.Set([&](mg::serverbox::Task* aTask) {
			MG_COMMON_ASSERT(progress == 0);
			MG_COMMON_ASSERT(aTask->IsExpired());
			mg::common::AtomicIncrement(&progress);
		});
		t1.SetCallback(cb);

		cb.Set([&](mg::serverbox::Task* aTask) {
			MG_COMMON_ASSERT(progress == 1);
			MG_COMMON_ASSERT(aTask->IsExpired());
			mg::common::AtomicIncrement(&progress);
		});
		t2.SetCallback(cb);

		cb.Set([&](mg::serverbox::Task* aTask) {
			MG_COMMON_ASSERT(progress == 2);
			MG_COMMON_ASSERT(aTask->IsExpired());
			mg::common::AtomicIncrement(&progress);
		});
		t3.SetCallback(cb);

		sched.Post(&t1);
		sched.PostDelay(&t2, 3);
		sched.PostDelay(&t3, 5);

		while (mg::common::AtomicLoad(&progress) != 3)
			mg::common::Sleep(1);
	}

	static void
	UnitTestTaskSchedulerDomino()
	{
		TestCaseGuard guard("Domino");

		// Ensure all the workers wakeup each other if necessary.
		// The test is called 'domino', because the worker threads
		// are supposed to wake each other on demand.
		mg::serverbox::TaskScheduler sched("tst", 3, 5);
		mg::serverbox::TaskCallback cb;
		mg::serverbox::Task t1;
		mg::serverbox::Task t2;
		mg::serverbox::Task t3;
		int32_t progres = 0;
		int32_t finish = 0;
		cb.Set([&](mg::serverbox::Task* aTask) {
			MG_COMMON_ASSERT(aTask->IsExpired());
			mg::common::AtomicIncrement(&progres);
			while (!mg::common::AtomicFlagTest(&finish))
				mg::common::Sleep(1);
			mg::common::AtomicIncrement(&progres);
		});
		t1.SetCallback(cb);
		t2.SetCallback(cb);
		t3.SetCallback(cb);

		sched.Post(&t1);
		sched.Post(&t2);
		sched.Post(&t3);

		while (mg::common::AtomicLoad(&progres) != 3)
			mg::common::Sleep(1);
		progres = 0;
		mg::common::AtomicFlagSet(&finish);
		while (mg::common::AtomicLoad(&progres) != 3)
			mg::common::Sleep(1);
	}

	static void
	UnitTestTaskSchedulerWakeup()
	{
		TestCaseGuard guard("Wakeup");

		mg::serverbox::TaskScheduler sched("tst", 1, 5);
		mg::serverbox::TaskCallback cb;
		mg::serverbox::Task t1;
		int32_t progress = 0;
		cb.Set([&](mg::serverbox::Task*) {
			mg::common::AtomicFlagSet(&progress);
		});
		t1.SetCallback(cb);

		// Wakeup while the task is in the front queue. Should be
		// dispatched immediately to the ready queue.
		sched.Post(&t1);
		sched.Wakeup(&t1);
		while (!mg::common::AtomicFlagTest(&progress))
			mg::common::Sleep(1);

		// Works for deadlined tasks too.
		progress = 0;
		sched.PostDeadline(&t1, MG_DEADLINE_INFINITE - 1);
		sched.Wakeup(&t1);
		while (!mg::common::AtomicFlagTest(&progress))
			mg::common::Sleep(1);

		// If a task was woken up, it should not reuse the old
		// deadline after rescheduling.
		progress = 0;
		cb.Set([&](mg::serverbox::Task* aTask) {
			if (mg::common::AtomicIncrement(&progress) == 1)
				sched.Post(aTask);
		});
		t1.SetCallback(cb);
		sched.PostWait(&t1);
		sched.Wakeup(&t1);
		while (mg::common::AtomicLoad(&progress) != 2)
			mg::common::Sleep(1);

		// Wakeup works even if the task is being executed now.
		progress = 0;
		cb.Set([&](mg::serverbox::Task* aTask) {
			if (mg::common::AtomicLoad(&progress) == 0)
			{
				mg::common::AtomicExchange(&progress, 1);
				// Wakeup will happen here, and will make the task
				// executed early despite PostWait() below.
				while (mg::common::AtomicLoad(&progress) != 2)
					mg::common::Sleep(1);
				return sched.PostWait(aTask);
			}
			mg::common::AtomicExchange(&progress, 3);
		});
		t1.SetCallback(cb);
		sched.Post(&t1);
		while (mg::common::AtomicLoad(&progress) != 1)
			mg::common::Sleep(1);
		sched.Wakeup(&t1);
		mg::common::AtomicExchange(&progress, 2);
		while (mg::common::AtomicLoad(&progress) != 3)
			mg::common::Sleep(1);

		// Wakeup for signaled task does not work.
		progress = 0;
		cb.Set([&](mg::serverbox::Task* aTask) {
			if (aTask->IsSignaled())
			{
				mg::common::AtomicExchange(&progress, 1);
				// Wakeup will happen here, and will not affect
				// PostWait(), because the signal is still active.
				while (mg::common::AtomicLoad(&progress) != 2)
					mg::common::Sleep(1);
				MG_COMMON_ASSERT(aTask->ReceiveSignal());
				return sched.PostWait(aTask);
			}
			mg::common::AtomicExchange(&progress, 3);
		});
		t1.SetCallback(cb);
		sched.PostWait(&t1);
		sched.Signal(&t1);
		while (mg::common::AtomicLoad(&progress) != 1)
			mg::common::Sleep(1);

		sched.Wakeup(&t1);
		mg::common::AtomicExchange(&progress, 2);
		mg::common::Sleep(1);
		MG_COMMON_ASSERT(mg::common::AtomicLoad(&progress) == 2);
		while (t1.IsSignaled())
			mg::common::Sleep(1);

		sched.Wakeup(&t1);
		while (mg::common::AtomicLoad(&progress) != 3)
			mg::common::Sleep(1);
	}

	static void
	UnitTestTaskSchedulerExpiration()
	{
		TestCaseGuard guard("Expiration");

		mg::serverbox::TaskScheduler sched("tst", 1, 100);
		mg::serverbox::Task t1;
		int32_t progress;

		// Expiration check for non-woken task not having a
		// deadline.
		progress = 0;
		t1.SetCallback([&](mg::serverbox::Task* aTask) {
			MG_COMMON_ASSERT(!aTask->ReceiveSignal());
			MG_COMMON_ASSERT(aTask->IsExpired());
			mg::common::AtomicFlagSet(&progress);
		});
		sched.Post(&t1);
		while (!mg::common::AtomicFlagTest(&progress))
			mg::common::Sleep(1);

		// Expiration check for woken task not having a deadline.
		progress = 0;
		sched.Wakeup(&t1);
		sched.Post(&t1);
		while (!mg::common::AtomicFlagTest(&progress))
			mg::common::Sleep(1);

		// Expiration check for non-woken task having a deadline.
		progress = 0;
		sched.PostDelay(&t1, 1);
		while (!mg::common::AtomicFlagTest(&progress))
			mg::common::Sleep(1);

		// Expiration check for woken task having a deadline.
		progress = 0;
		t1.SetCallback([&](mg::serverbox::Task* aTask) {
			MG_COMMON_ASSERT(!aTask->ReceiveSignal());
			MG_COMMON_ASSERT(!aTask->IsExpired());
			mg::common::AtomicFlagSet(&progress);
		});
		sched.Wakeup(&t1);
		sched.PostDeadline(&t1, MG_DEADLINE_INFINITE - 1);
		while (!mg::common::AtomicFlagTest(&progress))
			mg::common::Sleep(1);

		// Expiration check for signaled task not having a
		// deadline.
		progress = 0;
		t1.SetCallback([&](mg::serverbox::Task* aTask) {
			MG_COMMON_ASSERT(aTask->ReceiveSignal());
			MG_COMMON_ASSERT(aTask->IsExpired());
			mg::common::AtomicFlagSet(&progress);
		});
		sched.Signal(&t1);
		sched.Post(&t1);
		while (!mg::common::AtomicFlagTest(&progress))
			mg::common::Sleep(1);

		// Expiration check for signaled task having a deadline.
		progress = 0;
		t1.SetCallback([&](mg::serverbox::Task* aTask) {
			MG_COMMON_ASSERT(aTask->ReceiveSignal());
			MG_COMMON_ASSERT(!aTask->IsExpired());
			mg::common::AtomicFlagSet(&progress);
		});
		sched.Signal(&t1);
		sched.PostDeadline(&t1, MG_DEADLINE_INFINITE - 1);
		while (!mg::common::AtomicFlagTest(&progress))
			mg::common::Sleep(1);

		// Expiration check for woken task which was in infinite
		// wait.
		progress = 0;
		t1.SetCallback([&](mg::serverbox::Task* aTask) {
			MG_COMMON_ASSERT(!aTask->ReceiveSignal());
			MG_COMMON_ASSERT(!aTask->IsExpired());
			mg::common::AtomicFlagSet(&progress);
		});
		sched.Wakeup(&t1);
		sched.PostWait(&t1);
		while (!mg::common::AtomicFlagTest(&progress))
			mg::common::Sleep(1);

		// Expiration check for signaled task which was in
		// infinite wait.
		progress = 0;
		t1.SetCallback([&](mg::serverbox::Task* aTask) {
			MG_COMMON_ASSERT(aTask->ReceiveSignal());
			MG_COMMON_ASSERT(!aTask->IsExpired());
			mg::common::AtomicFlagSet(&progress);
		});
		sched.Signal(&t1);
		sched.PostWait(&t1);
		while (!mg::common::AtomicFlagTest(&progress))
			mg::common::Sleep(1);

		// Deadline adjustment.
		progress = 0;
		t1.SetDeadline(MG_DEADLINE_INFINITE);
		MG_COMMON_ASSERT(t1.GetDeadline() == MG_DEADLINE_INFINITE);
		t1.AdjustDeadline(1);
		MG_COMMON_ASSERT(t1.GetDeadline() == 1);
		t1.AdjustDeadline(MG_DEADLINE_INFINITE);
		MG_COMMON_ASSERT(t1.GetDeadline() == 1);
		t1.AdjustDelay(1000000);
		// Does not adjust to a bigger value.
		MG_COMMON_ASSERT(t1.GetDeadline() == 1);
		t1.SetCallback([&](mg::serverbox::Task* aTask) {
			MG_COMMON_ASSERT(aTask->IsExpired());
			mg::common::AtomicFlagSet(&progress);
		});
		sched.Post(&t1);
		while (!mg::common::AtomicFlagTest(&progress))
			mg::common::Sleep(1);
	}

	static void
	UnitTestTaskSchedulerReschedule()
	{
		TestCaseGuard guard("Reschedule");

		mg::serverbox::TaskScheduler sched1("tst1", 2, 100);
		mg::serverbox::TaskScheduler sched2("tst2", 2, 100);
		mg::serverbox::TaskCallback cb;
		mg::serverbox::Task t1;
		mg::serverbox::Task t2;
		mg::serverbox::Task t3;
		int32_t progress = 0;
		// A task can schedule another task into the same or
		// different scheduler.
		cb.Set([&](mg::serverbox::Task* aTask) {
			MG_COMMON_ASSERT(aTask == &t1);
			MG_COMMON_ASSERT(aTask->IsExpired());
			sched1.Post(&t2);
			mg::common::AtomicIncrement(&progress);
		});
		t1.SetCallback(cb);

		cb.Set([&](mg::serverbox::Task* aTask) {
			MG_COMMON_ASSERT(aTask == &t2);
			MG_COMMON_ASSERT(aTask->IsExpired());
			sched2.Post(&t3);
			mg::common::AtomicIncrement(&progress);
		});
		t2.SetCallback(cb);
		t2.SetDelay(3);

		cb.Set([&](mg::serverbox::Task* aTask) {
			MG_COMMON_ASSERT(aTask == &t3);
			MG_COMMON_ASSERT(aTask->IsExpired());
			mg::common::AtomicIncrement(&progress);
		});
		t3.SetCallback(cb);
		t3.SetDelay(5);

		sched1.Post(&t1);

		while (mg::common::AtomicLoad(&progress) != 3)
			mg::common::Sleep(1);

		// A task can be signaled while not scheduled. But the
		// signal won't be consumed until the task is posted
		// again.
		progress = 0;
		t1.SetCallback([&](mg::serverbox::Task* aTask) {
			MG_COMMON_ASSERT(aTask == &t1);
			MG_COMMON_ASSERT(aTask->IsExpired());
			if (mg::common::AtomicFlagSet(&progress) == 1)
				MG_COMMON_ASSERT(aTask->ReceiveSignal());
		});
		sched1.Post(&t1);
		while (mg::common::AtomicFlagTest(&progress) != 1)
			mg::common::Sleep(1);

		sched1.Signal(&t1);
		mg::common::Sleep(1);
		MG_COMMON_ASSERT(t1.IsSignaled());
		sched1.Post(&t1);
		while (t1.IsSignaled())
			mg::common::Sleep(1);

		// A task can be woken up while not scheduled. But the
		// task is not executed until posted again.
		progress = 0;
		t1.SetCallback([&](mg::serverbox::Task* aTask) {
			MG_COMMON_ASSERT(aTask == &t1);
			if (mg::common::AtomicIncrement(&progress) != 1)
				MG_COMMON_ASSERT(!aTask->IsExpired());
			else
				MG_COMMON_ASSERT(aTask->IsExpired());
		});
		sched1.Post(&t1);
		while (mg::common::AtomicLoad(&progress) != 1)
			mg::common::Sleep(1);

		sched1.Wakeup(&t1);
		mg::common::Sleep(1);
		MG_COMMON_ASSERT(mg::common::AtomicLoad(&progress) == 1);
		sched1.PostWait(&t1);
		while (mg::common::AtomicLoad(&progress) != 2)
			mg::common::Sleep(1);
	}

	static void
	UnitTestTaskSchedulerSignal()
	{
		TestCaseGuard guard("Signal");

		mg::serverbox::TaskScheduler sched("tst", 1, 2);

		// Signal works during execution.
		int32_t progress = 0;
		mg::serverbox::Task t([&](mg::serverbox::Task* aTask) {
			bool isSignaled = aTask->ReceiveSignal();
			mg::common::AtomicIncrement(&progress);
			if (isSignaled)
				return;
			// Inject a sleep to ensure the signal works
			// during execution.
			while (mg::common::AtomicLoad(&progress) != 2)
				mg::common::Sleep(1);
			sched.PostWait(aTask);
		});
		sched.Post(&t);
		while (mg::common::AtomicLoad(&progress) != 1)
			mg::common::Sleep(1);
		sched.Signal(&t);
		mg::common::AtomicIncrement(&progress);
		while (mg::common::AtomicLoad(&progress) != 3)
			mg::common::Sleep(1);

		// Signal works for tasks in the front queue.
		progress = 0;
		t.SetCallback([&](mg::serverbox::Task*) {
			MG_COMMON_ASSERT(t.ReceiveSignal());
			mg::common::AtomicFlagSet(&progress);
		});
		sched.PostDeadline(&t, MG_DEADLINE_INFINITE);
		sched.Signal(&t);
		while (mg::common::AtomicFlagTest(&progress) != 1)
			mg::common::Sleep(1);

		// Signal works for tasks in the waiting queue. Without
		// -1 the task won't be saved to the waiting queue.
		progress = 0;
		sched.PostDeadline(&t, MG_DEADLINE_INFINITE - 1);
		mg::common::Sleep(1);
		sched.Signal(&t);
		while (mg::common::AtomicFlagTest(&progress) != 1)
			mg::common::Sleep(1);

		// Signal can be checked before receipt.
		t.SetCallback([&](mg::serverbox::Task* aTask) {
			if (!aTask->IsSignaled())
				return sched.PostWait(aTask);
			MG_COMMON_ASSERT(aTask->ReceiveSignal());
		});
		sched.Post(&t);
		sched.Signal(&t);
		while (t.IsSignaled())
			mg::common::Sleep(1);

		// Double signal works.
		progress = 0;
		t.SetCallback([&](mg::serverbox::Task* aTask) {
			while (mg::common::AtomicFlagTest(&progress) != 1)
				mg::common::Sleep(1);
			MG_COMMON_ASSERT(aTask->IsSignaled());
			MG_COMMON_ASSERT(aTask->ReceiveSignal());
		});
		sched.Post(&t);
		sched.Signal(&t);
		sched.Signal(&t);
		mg::common::AtomicFlagSet(&progress);
		while (t.IsSignaled())
			mg::common::Sleep(1);

		// Task is rescheduled until signal is received.
		progress = 0;
		t.SetCallback([&](mg::serverbox::Task* aTask) {
			MG_COMMON_ASSERT(aTask->IsSignaled());
			int32_t p = mg::common::AtomicIncrement(&progress);
			if (p == 1)
				return sched.PostWait(aTask);
			if (p == 2)
				return sched.PostDeadline(aTask, MG_DEADLINE_INFINITE - 1);
			if (p == 3)
				return sched.PostDelay(aTask, 1000000);
			if (p == 4)
				return sched.Post(aTask);
			MG_COMMON_ASSERT(aTask->ReceiveSignal());
		});
		sched.Signal(&t);
		sched.Post(&t);
		while (t.IsSignaled())
			mg::common::Sleep(1);
		MG_COMMON_ASSERT(mg::common::AtomicLoad(&progress) == 5);
	}

	struct UTTSchedulerTask;

	struct UTTSchedulerTaskCtx
	{
		UTTSchedulerTaskCtx(
			uint32_t aTaskCount,
			uint32_t aExecuteCount,
			mg::serverbox::TaskScheduler* aScheduler);

		void CreateHeavy();

		void CreateMicro();

		void CreateSignaled();

		void WaitAllExecuted();

		void WaitExecuteCount(
			uint64_t aCount);

		void WaitAllStopped();

		void PostAll();

		~UTTSchedulerTaskCtx();

		UTTSchedulerTask* myTasks;
		const uint32_t myTaskCount;
		const uint32_t myExecuteCount;
		int32_t myCurrentParallel;
		int32_t myMaxParallel;
		int32_t myStopCount;
		int64_t myTotalExecuteCount;
		mg::serverbox::TaskScheduler* myScheduler;
	};

	struct UTTSchedulerTask
		: public mg::serverbox::Task
	{
		UTTSchedulerTask()
			: myCtx(nullptr)
		{
		}

		void
		CreateHeavy(
			UTTSchedulerTaskCtx* aCtx)
		{
			myExecuteCount = 0;
			myCtx = aCtx;
			SetCallback(this, &UTTSchedulerTask::ExecuteHeavy);
		}

		void
		CreateMicro(
			UTTSchedulerTaskCtx* aCtx)
		{
			myExecuteCount = 0;
			myCtx = aCtx;
			SetCallback(this, &UTTSchedulerTask::ExecuteMicro);
		}

		void
		CreateSignaled(
			UTTSchedulerTaskCtx* aCtx)
		{
			myExecuteCount = 0;
			myCtx = aCtx;
			SetCallback(this, &UTTSchedulerTask::ExecuteSignaled);
			SetWait();
		}

		void
		ExecuteHeavy(
			mg::serverbox::Task* aTask)
		{
			MG_COMMON_ASSERT(aTask == this);
			aTask->ReceiveSignal();
			++myExecuteCount;
			mg::common::AtomicIncrement64(&myCtx->myTotalExecuteCount);

			uint32_t i;
			// Tracking max parallel helps to ensure the tasks
			// really use all the worker threads.
			int32_t old = mg::common::AtomicIncrement(&myCtx->myCurrentParallel);
			int32_t max = mg::common::AtomicLoad(&myCtx->myMaxParallel);
			if (old > max)
				mg::common::AtomicCompareExchange(&myCtx->myMaxParallel, old, max);
			// Simulate heavy work.
			for (i = 0; i < 100; ++i)
				mg::common::RandomBool();
			mg::common::AtomicDecrement(&myCtx->myCurrentParallel);

			bool isLast = myExecuteCount >= myCtx->myExecuteCount;
			if (myExecuteCount % 10 == 0)
			{
				i = mg::common::RandomUniformuint32_t(0, myCtx->myTaskCount - 1);
				myCtx->myScheduler->Wakeup(&myCtx->myTasks[i]);
				i = mg::common::RandomUniformuint32_t(0, myCtx->myTaskCount - 1);
				myCtx->myScheduler->Signal(&myCtx->myTasks[i]);
				i = mg::common::RandomUniformuint32_t(0, myCtx->myTaskCount - 1);
				myCtx->myScheduler->Wakeup(&myCtx->myTasks[i]);
				i = mg::common::RandomUniformuint32_t(0, myCtx->myTaskCount - 1);
				myCtx->myScheduler->Signal(&myCtx->myTasks[i]);
				return isLast ? Stop() : myCtx->myScheduler->Post(aTask);
			}
			if (myExecuteCount % 3 == 0)
				return isLast ? Stop() : myCtx->myScheduler->PostDelay(aTask, 2);
			if (myExecuteCount % 2 == 0)
				return isLast ? Stop() : myCtx->myScheduler->PostDelay(aTask, 1);
			return isLast ? Stop() : myCtx->myScheduler->Post(aTask);
		}

		void
		ExecuteMicro(
			mg::serverbox::Task* aTask)
		{
			MG_COMMON_ASSERT(aTask == this);
			++myExecuteCount;
			mg::common::AtomicIncrement64(&myCtx->myTotalExecuteCount);
			if (myExecuteCount >= myCtx->myExecuteCount)
				return Stop();
			return myCtx->myScheduler->Post(aTask);
		}

		void
		ExecuteSignaled(
			mg::serverbox::Task* aTask)
		{
			MG_COMMON_ASSERT(aTask == this);
			MG_COMMON_ASSERT(!aTask->IsExpired());
			MG_COMMON_ASSERT(aTask->ReceiveSignal());
			++myExecuteCount;
			mg::common::AtomicIncrement64(&myCtx->myTotalExecuteCount);
			if (myExecuteCount >= myCtx->myExecuteCount)
				return Stop();
			return myCtx->myScheduler->PostWait(aTask);
		}

		void
		Stop()
		{
			mg::common::AtomicIncrement(&myCtx->myStopCount);
		}

		uint32_t myExecuteCount;
		UTTSchedulerTaskCtx* myCtx;
	};

	UTTSchedulerTaskCtx::UTTSchedulerTaskCtx(
		uint32_t aTaskCount,
		uint32_t aExecuteCount,
		mg::serverbox::TaskScheduler* aScheduler)
		: myTasks(new UTTSchedulerTask[aTaskCount])
		, myTaskCount(aTaskCount)
		, myExecuteCount(aExecuteCount)
		, myCurrentParallel(0)
		, myMaxParallel(0)
		, myStopCount(0)
		, myTotalExecuteCount(0)
		, myScheduler(aScheduler)
	{
	}

	void
	UTTSchedulerTaskCtx::CreateHeavy()
	{
		for (uint32_t i = 0; i < myTaskCount; ++i)
			myTasks[i].CreateHeavy(this);
	}

	void
	UTTSchedulerTaskCtx::CreateMicro()
	{
		for (uint32_t i = 0; i < myTaskCount; ++i)
			myTasks[i].CreateMicro(this);
	}

	void
	UTTSchedulerTaskCtx::CreateSignaled()
	{
		for (uint32_t i = 0; i < myTaskCount; ++i)
			myTasks[i].CreateSignaled(this);
	}

	void
	UTTSchedulerTaskCtx::WaitAllExecuted()
	{
		uint64_t total = myExecuteCount * myTaskCount;
		WaitExecuteCount(total);
		MG_COMMON_ASSERT((int64_t)total == mg::common::AtomicLoad64(&myTotalExecuteCount));
	}

	void
	UTTSchedulerTaskCtx::WaitExecuteCount(
		uint64_t aCount)
	{
		while (mg::common::AtomicLoad64(&myTotalExecuteCount) < (int64_t)aCount)
			mg::common::Sleep(1);
	}

	void
	UTTSchedulerTaskCtx::WaitAllStopped()
	{
		while (mg::common::AtomicLoad(&myStopCount) != (int32_t)myTaskCount)
			mg::common::Sleep(1);
		for (uint32_t i = 0; i < myTaskCount; ++i)
			MG_COMMON_ASSERT(myTasks[i].myExecuteCount == myExecuteCount);
	}

	void
	UTTSchedulerTaskCtx::PostAll()
	{
		for (uint32_t i = 0; i < myTaskCount; ++i)
			myScheduler->Post(&myTasks[i]);
	}

	UTTSchedulerTaskCtx::~UTTSchedulerTaskCtx()
	{
		delete[] myTasks;
	}

	static void
	UnitTestTaskSchedulerPrintStat(
		const mg::serverbox::TaskScheduler* aSched)
	{
		uint32_t count;
		mg::serverbox::TaskSchedulerThread*const* threads = aSched->GetThreads(count);
		Report("Scheduler stat:");
		for (uint32_t i = 0; i < count; ++i)
		{
			uint64_t execCount = threads[i]->StatPopExecuteCount();
			uint64_t schedCount = threads[i]->StatPopScheduleCount();
			Report("Thread %2u: exec: %12llu, sched: %9llu", i, (unsigned long long)execCount,
				(unsigned long long)schedCount);
		}
		Report("");
	}

	static void
	UnitTestTaskSchedulerBatch(
		uint32_t aThreadCount,
		uint32_t aTaskCount,
		uint32_t aExecuteCount)
	{
		TestCaseGuard guard("Batch");

		Report("Batch test: %u threads, %u tasks, %u executes", aThreadCount, aTaskCount,
			aExecuteCount);
		mg::serverbox::TaskScheduler sched("tst", aThreadCount, 5000);
		UTTSchedulerTaskCtx ctx(aTaskCount, aExecuteCount, &sched);

		ctx.CreateHeavy();
		ctx.PostAll();
		ctx.WaitAllStopped();

		UnitTestTaskSchedulerPrintStat(&sched);
	}

	static void
	UnitTestTaskSchedulerMicro(
		uint32_t aThreadCount,
		uint32_t aTaskCount)
	{
		TestCaseGuard guard("Micro");

		// Micro test uses super lightweight tasks to check how
		// fast is the scheduler itself, almost not affected by
		// the task bodies.
		Report("Micro test: %u threads, %u tasks", aThreadCount, aTaskCount);
		mg::serverbox::TaskScheduler sched("tst", aThreadCount, 5000);
		UTTSchedulerTaskCtx ctx(aTaskCount, 1, &sched);
		mg::common::QPTimer timer;

		ctx.CreateMicro();
		timer.Start();
		ctx.PostAll();
		ctx.WaitAllExecuted();
		double duration = timer.GetMilliSeconds();
		Report("Duration: %lf ms", duration);
		ctx.WaitAllStopped();

		UnitTestTaskSchedulerPrintStat(&sched);
	}

	static void
	UnitTestTaskSchedulerMicroNew(
		uint32_t aThreadCount,
		uint32_t aTaskCount)
	{
		TestCaseGuard guard("Micro new");

		// Checkout speed of one-shot tasks allocated manually, so
		// it is -1 virtual call compared to automatic one-shot
		// tasks.
		Report("Micro new test: %u threads, %u tasks", aThreadCount, aTaskCount);
		mg::serverbox::TaskScheduler sched("tst", aThreadCount, 5000);
		int64_t executeCount = 0;
		mg::serverbox::TaskCallback cb(
			[&](mg::serverbox::Task* aTask) {
				mg::common::AtomicIncrement64(&executeCount);
				delete aTask;
			}
		);
		mg::common::QPTimer timer;

		timer.Start();
		for (uint32_t i = 0; i < aTaskCount; ++i)
			sched.Post(new mg::serverbox::Task(cb));
		while (mg::common::AtomicLoad64(&executeCount) != aTaskCount)
			mg::common::Sleep(1);
		double duration = timer.GetMilliSeconds();
		Report("Duration: %lf ms", duration);

		UnitTestTaskSchedulerPrintStat(&sched);
	}

	static void
	UnitTestTaskSchedulerMicroOneShot(
		uint32_t aThreadCount,
		uint32_t aTaskCount)
	{
		TestCaseGuard guard("Micro one shot");

		// Checkout speed of one-shot tasks, which are allocated
		// automatically inside of the scheduler.
		Report("Micro one shot test: %u threads, %u tasks", aThreadCount, aTaskCount);
		mg::serverbox::TaskScheduler sched("tst", aThreadCount, 5000);
		int64_t executeCount = 0;
		mg::serverbox::TaskCallbackOneShot cb([&](void) -> void {
			mg::common::AtomicIncrement64(&executeCount);
		});
		mg::common::QPTimer timer;

		timer.Start();
		for (uint32_t i = 0; i < aTaskCount; ++i)
			sched.PostOneShot(cb);
		while (mg::common::AtomicLoad64(&executeCount) != aTaskCount)
			mg::common::Sleep(1);
		double duration = timer.GetMilliSeconds();
		Report("Duration: %lf ms", duration);

		UnitTestTaskSchedulerPrintStat(&sched);
	}

	static void
	UnitTestTaskSchedulerPortions(
		uint32_t aThreadCount,
		uint32_t aTaskCount,
		uint32_t aExecuteCount)
	{
		TestCaseGuard guard("Portions");

		// See what happens when tasks are added in multiple
		// steps, not in a single sleep-less loop.
		Report("Portions test: %u threads, %u tasks, %u executes", aThreadCount,
			aTaskCount, aExecuteCount);
		mg::serverbox::TaskScheduler sched("tst", aThreadCount, 5000);
		UTTSchedulerTaskCtx ctx(aTaskCount, aExecuteCount, &sched);

		ctx.CreateHeavy();
		for (uint32_t i = 0; i < aTaskCount; ++i)
		{
			sched.Post(&ctx.myTasks[i]);
			if (i % 10000 == 0)
				mg::common::Sleep(5);
		}
		ctx.WaitAllStopped();

		Report("Max parallel: %u", ctx.myMaxParallel);
		UnitTestTaskSchedulerPrintStat(&sched);
	}

	static void
	UnitTestTaskSchedulerMildLoad(
		uint32_t aThreadCount,
		uint32_t aTaskCount,
		uint32_t aExecuteCount,
		uint32_t aDuration)
	{
		TestCaseGuard guard("Mild load");

		// Try to simulate load close to reality, when RPS may be
		// relatively stable, and not millions.
		MG_COMMON_ASSERT(aTaskCount % aDuration == 0);
		uint32_t tasksPer50ms = aTaskCount / aDuration * 50;
		Report("Mild load test: %u threads, %u tasks, %u executes, %u per 50ms",
			aThreadCount, aTaskCount, aExecuteCount, tasksPer50ms);
		mg::serverbox::TaskScheduler sched("tst", aThreadCount, 5000);
		UTTSchedulerTaskCtx ctx(aTaskCount, aExecuteCount, &sched);

		ctx.CreateHeavy();
		for (uint32_t i = 0; i < aTaskCount; ++i)
		{
			sched.Post(&ctx.myTasks[i]);
			if (i % tasksPer50ms == 0)
				mg::common::Sleep(50);
		}
		ctx.WaitAllStopped();

		Report("Max parallel: %u", ctx.myMaxParallel);
		UnitTestTaskSchedulerPrintStat(&sched);
	}

	static void
	UnitTestTaskSchedulerTimeouts(
		uint32_t aTaskCount)
	{
		TestCaseGuard guard("Timeouts");

		// The test checks how slow is the scheduler on the
		// slowest case - when tons of tasks are woken up in the
		// order reversed from their insertion. It would cost
		// logarithmic time to remove each of them from the
		// waiting tasks queue's root, which is a binary heap.
		Report("Timeouts test: %u tasks", aTaskCount);
		mg::serverbox::TaskScheduler sched("tst", 1, 5000);
		UTTSchedulerTaskCtx ctx(aTaskCount, 1, &sched);

		ctx.CreateMicro();
		for (uint32_t i = 0; i < aTaskCount; ++i)
			ctx.myTasks[i].SetDelay(1000000 + i);

		// Create one special task pushed last, whose execution
		// would mean all the previous tasks surely ended up in
		// the waiting queue.
		mg::serverbox::Task* t = new mg::serverbox::Task(
			[&](mg::serverbox::Task* aTask) {
				mg::common::AtomicExchangePtr(&t, (mg::serverbox::Task*)nullptr);
				delete aTask;
			}
		);

		mg::common::QPTimer timer;
		timer.Start();
		ctx.PostAll();
		sched.Post(t);

		while (mg::common::AtomicLoadPtr(&t) != nullptr)
			mg::common::Sleep(1);

		// Wakeup so as the scheduler would remove tasks from the
		// binary heap from its root - the slowest case. For that
		// wakeup from the end, because the front queue is
		// reversed.
		for (int i = aTaskCount - 1; i >= 0; --i)
			sched.Wakeup(&ctx.myTasks[i]);
		ctx.WaitAllExecuted();

		double duration = timer.GetMilliSeconds();
		Report("Duration: %lf ms", duration);

		ctx.WaitAllStopped();
		UnitTestTaskSchedulerPrintStat(&sched);
	}

	static void
	UnitTestTaskSchedulerSignalStress(
		uint32_t aThreadCount,
		uint32_t aTaskCount,
		uint32_t aExecuteCount)
	{
		TestCaseGuard guard("Signal stress");

		// Ensure the tasks never stuck when signals are used.
		Report("Signal stress test: %u threads, %u tasks, %u executes", aThreadCount,
			aTaskCount, aExecuteCount);
		mg::serverbox::TaskScheduler sched("tst", aThreadCount, 5000);
		UTTSchedulerTaskCtx ctx(aTaskCount, aExecuteCount, &sched);

		ctx.CreateSignaled();
		ctx.PostAll();
		for (uint32_t i = 0; i < aExecuteCount; ++i)
		{
			for (uint32_t j = 0; j < aTaskCount; ++j)
				sched.Signal(&ctx.myTasks[j]);
			uint64_t total = aTaskCount * (i + 1);
			ctx.WaitExecuteCount(total);
		}
		ctx.WaitAllStopped();

		UnitTestTaskSchedulerPrintStat(&sched);
	}

	void
	UnitTestTaskScheduler()
	{
		TestSuiteGuard suite("TaskScheduler");

		UnitTestTaskSchedulerBasic();
		UnitTestTaskSchedulerOrder();
		UnitTestTaskSchedulerDomino();
		UnitTestTaskSchedulerWakeup();
		UnitTestTaskSchedulerExpiration();
		UnitTestTaskSchedulerReschedule();
		UnitTestTaskSchedulerSignal();
		UnitTestTaskSchedulerMicro(5, 10000000);
		UnitTestTaskSchedulerMicroNew(5, 10000000);
		UnitTestTaskSchedulerMicroOneShot(5, 10000000);
		UnitTestTaskSchedulerBatch(5, 100000, 100);
		UnitTestTaskSchedulerPortions(5, 100000, 100);
		UnitTestTaskSchedulerMildLoad(5, 100000, 1, 10000);
		UnitTestTaskSchedulerTimeouts(1000000);
		UnitTestTaskSchedulerSignalStress(5, 1000000, 5);
	}

}
}
