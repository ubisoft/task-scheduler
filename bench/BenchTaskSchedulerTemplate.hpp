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
			uint32 aTaskCount,
			uint32 aExecuteCount,
			TaskScheduler* aScheduler);

		~BenchTaskCtl();

		void Warmup();

		void CreateNano();

		void CreateMicro();

		void CreateHeavy();

		void WaitAllExecuted();

		void WaitExecuteCount(
			uint64 aCount);

		void WaitAllStopped();

		void PostAll();

		BenchTask* myTasks;
		const uint32 myTaskCount;
		const uint32 myExecuteCount;
		std::atomic<uint32> myStopCount;
		std::atomic<uint64> myTotalExecuteCount;
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

		uint32 myExecuteCount;
		BenchTaskCtl* myCtx;
	};

	//////////////////////////////////////////////////////////////////////////////////////

	struct BenchThreadReport
	{
		BenchThreadReport();

		uint64 myExecCount;
		uint64 mySchedCount;
	};

	struct BenchRunReport
	{
		BenchRunReport();

		bool operator<(
			const BenchRunReport& aOther) const;

		void Print() const;

		uint64 myExecPerSec;
		uint64 myExecPerSecPerThread;
		double myUsPerExec;
		uint64 myMutexContentionCount;
		mg::common::HybridArray<BenchThreadReport, 10> myThreads;
	};

	//////////////////////////////////////////////////////////////////////////////////////

	static BenchRunReport BenchTaskSchedulerRun(
		BenchLoadType aType,
		uint32 aThreadCount,
		uint32 aTaskCount,
		uint32 aExecuteCount);

	//////////////////////////////////////////////////////////////////////////////////////

	BenchTaskCtl::BenchTaskCtl(
		uint32 aTaskCount,
		uint32 aExecuteCount,
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
		std::atomic<uint32> execCount(0);
		for (int i = 0; i < MG_WARMUP_TASK_COUNT; ++i)
		{
			myScheduler->PostOneShot([&]() {
				execCount.fetch_add(1);
			});
		}
		while (execCount.load() != MG_WARMUP_TASK_COUNT)
			mg::common::Sleep(1);
		// Cleanup the stats to make the bench's results clean.
		uint32 count = 0;
		TaskSchedulerThread*const* threads = myScheduler->GetThreads(count);
		for (uint32 i = 0; i < count; ++i)
		{
			threads[i]->StatPopExecuteCount();
			threads[i]->StatPopScheduleCount();
		}
	}

	void
	BenchTaskCtl::CreateNano()
	{
		for (uint32 i = 0; i < myTaskCount; ++i)
			myTasks[i].CreateNano(this);
	}

	void
	BenchTaskCtl::CreateMicro()
	{
		for (uint32 i = 0; i < myTaskCount; ++i)
			myTasks[i].CreateMicro(this);
	}

	void
	BenchTaskCtl::CreateHeavy()
	{
		for (uint32 i = 0; i < myTaskCount; ++i)
			myTasks[i].CreateHeavy(this);
	}

	void
	BenchTaskCtl::WaitAllExecuted()
	{
		uint64 total = myExecuteCount * myTaskCount;
		WaitExecuteCount(total);
		MG_COMMON_ASSERT(total == myTotalExecuteCount.load());
	}

	void
	BenchTaskCtl::WaitExecuteCount(
		uint64 aCount)
	{
		while (myTotalExecuteCount.load() < aCount)
			mg::common::Sleep(1);
	}

	void
	BenchTaskCtl::WaitAllStopped()
	{
		while (myStopCount.load() != myTaskCount)
			mg::common::Sleep(1);
		for (uint32 i = 0; i < myTaskCount; ++i)
			MG_COMMON_ASSERT(myTasks[i].myExecuteCount == myExecuteCount);
	}

	void
	BenchTaskCtl::PostAll()
	{
		for (uint32 i = 0; i < myTaskCount; ++i)
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
		myCtx->myTotalExecuteCount.fetch_add(1);
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
		myCtx->myTotalExecuteCount.fetch_add(1);
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
		myCtx->myTotalExecuteCount.fetch_add(1);
		BenchMakeHeavyWork();
		bool isLast = myExecuteCount >= myCtx->myExecuteCount;
		if (myExecuteCount % 10 == 0)
		{
			uint32 i;
			i = mg::common::RandomUniformUInt32(0, myCtx->myTaskCount - 1);
			myCtx->myScheduler->Wakeup(&myCtx->myTasks[i]);
			i = mg::common::RandomUniformUInt32(0, myCtx->myTaskCount - 1);
			myCtx->myScheduler->Signal(&myCtx->myTasks[i]);
			i = mg::common::RandomUniformUInt32(0, myCtx->myTaskCount - 1);
			myCtx->myScheduler->Wakeup(&myCtx->myTasks[i]);
			i = mg::common::RandomUniformUInt32(0, myCtx->myTaskCount - 1);
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
		myCtx->myStopCount.fetch_add(1);
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
		for (uint32 i = 0; i < myThreads.Count(); ++i)
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
		uint32 aThreadCount,
		uint32 aTaskCount,
		uint32 aExecuteCount)
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

		report.myExecPerSec = (uint64)(ctl.myTotalExecuteCount * 1000 / durationMs);
		report.myExecPerSecPerThread = report.myExecPerSec / aThreadCount;
		report.myUsPerExec = durationMs * 1000 / ctl.myTotalExecuteCount * aThreadCount;

		TaskSchedulerThread*const* threads = sched.GetThreads(aThreadCount);
		report.myThreads.SetCount(aThreadCount);
		for (uint32 i = 0; i < aThreadCount; ++i)
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
	uint32 threadCount = cmdLine.GetU32("threads");
	uint32 taskCount = cmdLine.GetU32("tasks");
	uint32 exeCount = cmdLine.GetU32("exes");
	uint32 runCount = 1;
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
