#pragma once

#include "Bench.h"

#include "mg/common/Mutex.h"
#include "mg/common/Random.h"

#include <algorithm>

#define MG_WARMUP_TASK_COUNT 10000

namespace mg {
namespace bench {

	struct BenchTask;

	struct BenchTaskCtl
	{
		BenchTaskCtl(
			uint32_t aTaskCount,
			uint32_t aExecuteCount,
			TaskScheduler* aScheduler);

		~BenchTaskCtl();

		void Warmup();

		void CreateNano();

		void CreateMicro();

		void CreateHeavy();

		void WaitAllExecuted();

		void WaitExecuteCount(
			uint64_t aCount);

		void WaitAllStopped();

		void PostAll();

		BenchTask* myTasks;
		const uint32_t myTaskCount;
		const uint32_t myExecuteCount;
		int32_t myStopCount;
		int64_t myTotalExecuteCount;
		TaskScheduler* myScheduler;
	};

	struct BenchTask
		: public Task
	{
		BenchTask();

		void CreateNano(
			BenchTaskCtl* aCtx);

		void CreateMicro(
			BenchTaskCtl* aCtx);

		void CreateHeavy(
			BenchTaskCtl* aCtx);

		void ExecuteNano(
			Task* aTask);

		void ExecuteMicro(
			Task* aTask);

		void ExecuteHeavy(
			Task* aTask);

		void Stop();

		uint32_t myExecuteCount;
		BenchTaskCtl* myCtx;
	};

	//////////////////////////////////////////////////////////////////////////////////////

	struct BenchThreadReport
	{
		BenchThreadReport();

		uint64_t myExecCount;
		uint64_t mySchedCount;
	};

	struct BenchRunReport
	{
		BenchRunReport();

		bool operator<(
			const BenchRunReport& aOther) const;

		void Print() const;

		uint64_t myExecPerSec;
		uint64_t myExecPerSecPerThread;
		double myUsPerExec;
		uint64_t myMutexContentionCount;
		mg::common::HybridArray<BenchThreadReport, 10> myThreads;
	};

	//////////////////////////////////////////////////////////////////////////////////////

	static BenchRunReport BenchTaskSchedulerRun(
		BenchLoadType aType,
		uint32_t aThreadCount,
		uint32_t aTaskCount,
		uint32_t aExecuteCount);

	//////////////////////////////////////////////////////////////////////////////////////

	BenchTaskCtl::BenchTaskCtl(
		uint32_t aTaskCount,
		uint32_t aExecuteCount,
		TaskScheduler* aScheduler)
		: myTasks(new BenchTask[aTaskCount])
		, myTaskCount(aTaskCount)
		, myExecuteCount(aExecuteCount)
		, myStopCount(0)
		, myTotalExecuteCount(0)
		, myScheduler(aScheduler)
	{
	}

	void
	BenchTaskCtl::Warmup()
	{
		int32_t execCount = 0;
		for (int i = 0; i < MG_WARMUP_TASK_COUNT; ++i)
		{
			myScheduler->PostOneShot([&]() {
				mg::common::AtomicIncrement(&execCount);
			});
		}
		while (mg::common::AtomicLoad(&execCount) != MG_WARMUP_TASK_COUNT)
			mg::common::Sleep(1);
		// Cleanup the stats to make the bench's results clean.
		uint32_t count = 0;
		TaskSchedulerThread*const* threads = myScheduler->GetThreads(count);
		for (uint32_t i = 0; i < count; ++i)
		{
			threads[i]->StatPopExecuteCount();
			threads[i]->StatPopScheduleCount();
		}
	}

	void
	BenchTaskCtl::CreateNano()
	{
		for (uint32_t i = 0; i < myTaskCount; ++i)
			myTasks[i].CreateNano(this);
	}

	void
	BenchTaskCtl::CreateMicro()
	{
		for (uint32_t i = 0; i < myTaskCount; ++i)
			myTasks[i].CreateMicro(this);
	}

	void
	BenchTaskCtl::CreateHeavy()
	{
		for (uint32_t i = 0; i < myTaskCount; ++i)
			myTasks[i].CreateHeavy(this);
	}

	void
	BenchTaskCtl::WaitAllExecuted()
	{
		uint64_t total = myExecuteCount * myTaskCount;
		WaitExecuteCount(total);
		MG_COMMON_ASSERT((int64_t)total == mg::common::AtomicLoad64(&myTotalExecuteCount));
	}

	void
	BenchTaskCtl::WaitExecuteCount(
		uint64_t aCount)
	{
		while (mg::common::AtomicLoad64(&myTotalExecuteCount) < (int64_t)aCount)
			mg::common::Sleep(1);
	}

	void
	BenchTaskCtl::WaitAllStopped()
	{
		while (mg::common::AtomicLoad(&myStopCount) != (int32_t)myTaskCount)
			mg::common::Sleep(1);
		for (uint32_t i = 0; i < myTaskCount; ++i)
			MG_COMMON_ASSERT(myTasks[i].myExecuteCount == myExecuteCount);
	}

	void
	BenchTaskCtl::PostAll()
	{
		for (uint32_t i = 0; i < myTaskCount; ++i)
			myScheduler->Post(&myTasks[i]);
	}

	BenchTaskCtl::~BenchTaskCtl()
	{
		delete[] myTasks;
	}

	//////////////////////////////////////////////////////////////////////////////////////

	BenchTask::BenchTask()
		: myCtx(nullptr)
	{
	}

	void
	BenchTask::CreateNano(
		BenchTaskCtl* aCtx)
	{
		myExecuteCount = 0;
		myCtx = aCtx;
		SetCallback(this, &BenchTask::ExecuteNano);
	}

	void
	BenchTask::CreateMicro(
		BenchTaskCtl* aCtx)
	{
		myExecuteCount = 0;
		myCtx = aCtx;
		SetCallback(this, &BenchTask::ExecuteMicro);
	}

	void
	BenchTask::CreateHeavy(
		BenchTaskCtl* aCtx)
	{
		myExecuteCount = 0;
		myCtx = aCtx;
		SetCallback(this, &BenchTask::ExecuteHeavy);
	}

	void
	BenchTask::ExecuteNano(
		Task* aTask)
	{
		MG_COMMON_ASSERT(aTask == this);
		++myExecuteCount;
		mg::common::AtomicIncrement64(&myCtx->myTotalExecuteCount);
		if (myExecuteCount >= myCtx->myExecuteCount)
			return Stop();
		return myCtx->myScheduler->Post(aTask);
	}

	void
	BenchTask::ExecuteMicro(
		Task* aTask)
	{
		MG_COMMON_ASSERT(aTask == this);
		++myExecuteCount;
		mg::common::AtomicIncrement64(&myCtx->myTotalExecuteCount);
		BenchMakeMicroWork();
		if (myExecuteCount >= myCtx->myExecuteCount)
			return Stop();
		return myCtx->myScheduler->Post(aTask);
	}

	void
	BenchTask::ExecuteHeavy(
		Task* aTask)
	{
		MG_COMMON_ASSERT(aTask == this);
		aTask->ReceiveSignal();
		++myExecuteCount;
		mg::common::AtomicIncrement64(&myCtx->myTotalExecuteCount);
		BenchMakeHeavyWork();
		bool isLast = myExecuteCount >= myCtx->myExecuteCount;
		if (myExecuteCount % 10 == 0)
		{
			uint32_t i;
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
	BenchTask::Stop()
	{
		mg::common::AtomicIncrement(&myCtx->myStopCount);
	}

	//////////////////////////////////////////////////////////////////////////////////////

	BenchThreadReport::BenchThreadReport()
		: myExecCount(0)
		, mySchedCount(0)
	{
	}

	BenchRunReport::BenchRunReport()
		: myExecPerSec(0)
		, myExecPerSecPerThread(0)
		, myUsPerExec(0)
		, myMutexContentionCount(0)
	{
	}

	inline bool
	BenchRunReport::operator<(
		const BenchRunReport& aOther) const
	{
		return myExecPerSec < aOther.myExecPerSec;
	}

	void
	BenchRunReport::Print() const
	{
		Report("Microseconds per exec:      %12.6lf",
			myUsPerExec);
		Report("Exec per second:            %12llu",
			(unsigned long long)myExecPerSec);
		if (myThreads.Count() > 1)
		{
			Report("Exec per second per thread: %12llu",
				(unsigned long long)myExecPerSecPerThread);
		}
		Report("Mutex contention count:     %12llu",
			(unsigned long long)myMutexContentionCount);
		for (uint32_t i = 0; i < myThreads.Count(); ++i)
		{
			const BenchThreadReport& tr = myThreads[i];
			Report("Thread %2u: exec: %12llu, sched: %9llu", i,
				(unsigned long long)tr.myExecCount, (unsigned long long)tr.mySchedCount);
		}
		Report("");
	}

	//////////////////////////////////////////////////////////////////////////////////////

	static BenchRunReport
	BenchTaskSchedulerRun(
		BenchLoadType aType,
		uint32_t aThreadCount,
		uint32_t aTaskCount,
		uint32_t aExecuteCount)
	{
		TaskScheduler sched("bench", aThreadCount, 5000);
		sched.Reserve(aTaskCount);
		BenchTaskCtl ctl(aTaskCount, aExecuteCount, &sched);
		ctl.Warmup();

		switch (aType) {
		case BENCH_LOAD_NANO:
			ctl.CreateNano();
			break;
		case BENCH_LOAD_MICRO:
			ctl.CreateMicro();
			break;
		case BENCH_LOAD_HEAVY:
			ctl.CreateHeavy();
			break;
		case BENCH_LOAD_EMPTY:
		default:
			MG_COMMON_ASSERT(!"Unsupported load type");
			break;
		}
		BenchCaseGuard guard("Load %s, thread=%u, task=%u, exec=%u",
			BenchLoadTypeToString(aType), aThreadCount, aTaskCount, aExecuteCount);
		BenchRunReport report;

		mg::common::MutexStatClear();
		TimedGuard timed("Post and wait");
		ctl.PostAll();
		ctl.WaitAllStopped();
		timed.Stop();
		report.myMutexContentionCount = mg::common::MutexStatContentionCount();
		timed.Report();
		double durationMs = timed.GetMilliseconds();

		report.myExecPerSec = (uint64_t)(ctl.myTotalExecuteCount * 1000 / durationMs);
		report.myExecPerSecPerThread = report.myExecPerSec / aThreadCount;
		report.myUsPerExec = durationMs * 1000 / ctl.myTotalExecuteCount * aThreadCount;

		TaskSchedulerThread*const* threads = sched.GetThreads(aThreadCount);
		report.myThreads.SetCount(aThreadCount);
		for (uint32_t i = 0; i < aThreadCount; ++i)
		{
			BenchThreadReport& tr = report.myThreads[i];
			tr.myExecCount = threads[i]->StatPopExecuteCount();
			tr.mySchedCount = threads[i]->StatPopScheduleCount();
		}
		report.Print();
		return report;
	}

}
}

int
main(
	int aArgc,
	char** aArgv)
{
	using namespace mg::bench;
	CommandLine cmdLine(aArgc - 1, aArgv + 1);
	BenchLoadType loadType = BenchLoadTypeFromString(cmdLine.GetStr("load"));
	uint32_t threadCount = cmdLine.GetU32("threads");
	uint32_t taskCount = cmdLine.GetU32("tasks");
	uint32_t exeCount = cmdLine.GetU32("exes");
	uint32_t runCount = 1;
	if (cmdLine.IsPresent("runs"))
		runCount = cmdLine.GetU32("runs");

	mg::common::HybridArray<BenchRunReport, 10> reports;
	reports.SetCount(runCount);
	for (BenchRunReport& r : reports)
		r = BenchTaskSchedulerRun(loadType, threadCount, taskCount, exeCount);
	if (runCount < 3)
		return -1;
	std::sort(reports.GetBuffer(), reports.GetBuffer() + runCount);
	Report("");

	Report("== Aggregated report:");
	BenchRunReport* rMin = &reports[0];
	// If the count is even, then intentionally print the lower middle.
	BenchRunReport* rMed = &reports[runCount / 2];
	BenchRunReport* rMax = &reports[runCount - 1];
	Report("Exec per second min:        %12llu",
		(unsigned long long)rMin->myExecPerSec);
	Report("Exec per second median:     %12llu",
		(unsigned long long)rMed->myExecPerSec);
	Report("Exec per second max:        %12llu",
		(unsigned long long)rMax->myExecPerSec);
	Report("");

	Report("== Median report:");
	rMed->Print();
	return 0;
}
