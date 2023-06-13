#pragma once

#include "Bench.h"

#include "mg/common/Atomic.h"
#include "mg/common/Mutex.h"
#include "mg/common/Random.h"
#include "mg/common/Thread.h"

#include <algorithm>
#include <vector>

#define MG_WARMUP_ITEM_COUNT 10000

namespace mg {
namespace bench {

	class BenchConsumerThread
		: private mg::common::Thread
	{
	public:
		BenchConsumerThread(
			BenchQueue* aQueue,
			BenchLoadType aLoadType);

		~BenchConsumerThread() override;

		void Pause();

		void Continue();

		void StatClear();

		uint64_t StatGetCount() const;

	private:
		void Run() override;

		BenchQueueConsumer myConsumer;
		mg::common::AtomicU64 myCount;
		mg::common::AtomicU32 myPauseState;
		BenchLoadType myLoadType;
	};

	//////////////////////////////////////////////////////////////////////////////////////

	struct BenchThreadReport
	{
		BenchThreadReport();

		uint64_t myPopCount;
	};

	struct BenchRunReport
	{
		BenchRunReport();

		bool operator<(
			const BenchRunReport& aOther) const;

		void Print() const;

		uint64_t myItemsPerSec;
		uint64_t myItemsPerSecPerThread;
		uint64_t myMutexContentionCount;
		std::vector<BenchThreadReport> myThreads;
	};

	//////////////////////////////////////////////////////////////////////////////////////

	static void BenchQueueWarmup(
		BenchQueue& aQueue,
		std::vector<BenchConsumerThread*>& aWorkers);

	static BenchRunReport BenchQueueRun(
		CommandLine& aCmdLine);

	static BenchRunReport BenchQueueRunPush(
		uint32_t aItemCount,
		uint32_t aSubQueueSize);

	static BenchRunReport BenchQueueRunPop(
		BenchLoadType aLoadType,
		uint32_t aItemCount,
		uint32_t aThreadCount,
		uint32_t aSubQueueSize);

	static BenchRunReport BenchQueueRunPushPop(
		BenchLoadType aLoadType,
		uint32_t aItemCount,
		uint32_t aThreadCount,
		uint32_t aSubQueueSize);

	//////////////////////////////////////////////////////////////////////////////////////

	BenchConsumerThread::BenchConsumerThread(
		BenchQueue* aQueue,
		BenchLoadType aLoadType)
		: myCount(0)
		, myPauseState(0)
		, myLoadType(aLoadType)
	{
		myConsumer.Attach(aQueue);
		Start();
	}

	BenchConsumerThread::~BenchConsumerThread()
	{
		BlockingStop();
		myConsumer.Detach();
	}

	void
	BenchConsumerThread::Pause()
	{
		uint32_t old = myPauseState.ExchangeRelease(1);
		MG_COMMON_ASSERT(old == 0);
		while (myPauseState.LoadAcquire() != 2)
			mg::common::Sleep(1);
	}

	void
	BenchConsumerThread::Continue()
	{
		uint32_t old = myPauseState.ExchangeRelease(0);
		MG_COMMON_ASSERT(old == 2);
	}

	void
	BenchConsumerThread::StatClear()
	{
		myCount.StoreRelaxed(0);
	}

	uint64_t
	BenchConsumerThread::StatGetCount() const
	{
		return myCount.LoadRelaxed();
	}

	void
	BenchConsumerThread::Run()
	{
		while (!StopRequested())
		{
			while (myConsumer.Pop() != nullptr)
			{
				myCount.IncrementRelaxed();
				if (myLoadType == BENCH_LOAD_MICRO)
					BenchMakeMicroWork();
			}
			uint32_t pause = myPauseState.LoadAcquire();
			if (pause != 0)
			{
				MG_COMMON_ASSERT(pause == 1);
				myPauseState.StoreRelease(2);
				while (myPauseState.LoadAcquire() != 0)
					mg::common::Sleep(1);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////

	BenchThreadReport::BenchThreadReport()
		: myPopCount(0)
	{
	}

	BenchRunReport::BenchRunReport()
		: myItemsPerSec(0)
		, myItemsPerSecPerThread(0)
		, myMutexContentionCount(0)
	{
	}

	bool
	BenchRunReport::operator<(
		const BenchRunReport& aOther) const
	{
		return myItemsPerSec < aOther.myItemsPerSec;
	}

	void
	BenchRunReport::Print() const
	{
		Report("Items per sec:            %12llu",
			(unsigned long long)myItemsPerSec);
		if (myThreads.size() > 1)
		{
			Report("Items per sec per thread: %12llu",
				(unsigned long long)myItemsPerSecPerThread);
		}
		Report("Mutex contention count:   %12llu",
			(unsigned long long)myMutexContentionCount);
		for (uint32_t i = 0; i < myThreads.size(); ++i)
		{
			const BenchThreadReport& r = myThreads[i];
			Report("Thread %2u: pop: %12llu", i, (unsigned long long)r.myPopCount);
		}
		Report("");
	}

	//////////////////////////////////////////////////////////////////////////////////////

	static void
	BenchQueueWarmup(
		BenchQueue& aQueue,
		std::vector<BenchConsumerThread*>& aWorkers)
	{
		BenchValue* vals = new BenchValue[MG_WARMUP_ITEM_COUNT];
		for (uint32_t i = 0; i < MG_WARMUP_ITEM_COUNT; ++i)
			aQueue.Push(&vals[i]);
		uint64_t popCount = 0;
		while (popCount < MG_WARMUP_ITEM_COUNT)
		{
			mg::common::Sleep(1);
			popCount = 0;
			for (BenchConsumerThread* w : aWorkers)
				popCount += w->StatGetCount();
		}
		// Cleanup the stats to make the bench's results clean.
		for (BenchConsumerThread* w : aWorkers)
			w->StatClear();
		delete[] vals;
	}

	static BenchRunReport
	BenchQueueRun(
		CommandLine& aCmdLine)
	{
		const char* operation = aCmdLine.GetStr("op");
		uint32_t itemCount = aCmdLine.GetU32("items");
		// It is optional for non-block-based queues.
		uint32_t subQueueSize = 0;
		if (aCmdLine.IsPresent("subqsize"))
			subQueueSize = aCmdLine.GetU32("subqsize");
		if (mg::common::Strcmp(operation, "push") == 0)
			return BenchQueueRunPush(itemCount, subQueueSize);

		BenchLoadType loadType = BenchLoadTypeFromString(aCmdLine.GetStr("load"));
		uint32_t threadCount = aCmdLine.GetU32("threads");
		if (mg::common::Strcmp(operation, "pop") == 0)
			return BenchQueueRunPop(loadType, itemCount, threadCount, subQueueSize);
		else if (mg::common::Strcmp(operation, "push-pop") == 0)
			return BenchQueueRunPushPop(loadType, itemCount, threadCount, subQueueSize);
		else
			MG_COMMON_ASSERT_F(false, "Uknown operation '%s'", operation);
		return BenchRunReport();
	}

	static BenchRunReport
	BenchQueueRunPush(
		uint32_t aItemCount,
		uint32_t aSubQueueSize)
	{
		BenchCaseGuard guard("Operation push, item=%u, subq=%u", aItemCount,
			aSubQueueSize);
		BenchQueue queue(aSubQueueSize);
		queue.Reserve(aItemCount);
		BenchValue* vals = new BenchValue[aItemCount];
		BenchRunReport report;
		{
			mg::common::MutexStatClear();
			TimedGuard timed("Populate");
			for (uint32_t i = 0; i < aItemCount; ++i)
				queue.Push(&vals[i]);
			timed.Stop();
			report.myMutexContentionCount = mg::common::MutexStatContentionCount();
			timed.Report();

			double durationMs = timed.GetMilliseconds();
			report.myItemsPerSec = (uint64_t)((uint64_t)aItemCount * 1000 / durationMs);
		}
		delete[] vals;
		report.Print();
		return report;
	}

	static BenchRunReport
	BenchQueueRunPop(
		BenchLoadType aLoadType,
		uint32_t aItemCount,
		uint32_t aThreadCount,
		uint32_t aSubQueueSize)
	{
		BenchCaseGuard guard("Operation pop, item=%u, thread=%u, subq=%u, load=%s",
			aItemCount, aThreadCount, aSubQueueSize, BenchLoadTypeToString(aLoadType));
		BenchQueue queue(aSubQueueSize);
		queue.Reserve(aItemCount);
		std::vector<BenchConsumerThread*> workers;
		workers.resize(aThreadCount);
		for (BenchConsumerThread*& w : workers)
			w = new BenchConsumerThread(&queue, aLoadType);
		BenchQueueWarmup(queue, workers);

		// Pause the consumers to isolate push and pop phases.
		for (BenchConsumerThread*& w : workers)
			w->Pause();
		BenchValue* vals = new BenchValue[aItemCount];
		for (uint32_t i = 0; i < aItemCount; ++i)
			queue.Push(&vals[i]);

		BenchRunReport report;
		{
			mg::common::MutexStatClear();
			TimedGuard timed("Consume");
			// Unleash the consumers. Recreating them wouldn't be a good idea as thread
			// start takes a lot of time. Better pause + continue existing threads.
			for (BenchConsumerThread*& w : workers)
				w->Continue();
			uint64_t popCount = 0;
			while (popCount != aItemCount)
			{
				mg::common::Sleep(1);
				popCount = 0;
				for (BenchConsumerThread*& w : workers)
					popCount += w->StatGetCount();
			}
			timed.Stop();
			report.myMutexContentionCount = mg::common::MutexStatContentionCount();
			timed.Report();

			double durationMs = timed.GetMilliseconds();

			report.myItemsPerSec = (uint64_t)((uint64_t)aItemCount * 1000 / durationMs);
			report.myItemsPerSecPerThread = report.myItemsPerSec / aThreadCount;
			report.myThreads.resize(aThreadCount);
			for (uint32_t i = 0; i < aThreadCount; ++i)
			{
				BenchThreadReport& r = report.myThreads[i];
				const BenchConsumerThread* w = workers[i];
				r.myPopCount = w->StatGetCount();
			}
		}
		delete[] vals;
		for (BenchConsumerThread* w : workers)
			delete w;
		report.Print();
		return report;
	}

	static BenchRunReport
	BenchQueueRunPushPop(
		BenchLoadType aLoadType,
		uint32_t aItemCount,
		uint32_t aThreadCount,
		uint32_t aSubQueueSize)
	{
		BenchCaseGuard guard("Operation push-pop, item=%u, thread=%u, subq=%u, load=%s",
			aItemCount, aThreadCount, aSubQueueSize, BenchLoadTypeToString(aLoadType));
		BenchQueue queue(aSubQueueSize);
		queue.Reserve(aItemCount);
		std::vector<BenchConsumerThread*> workers;
		workers.resize(aThreadCount);
		for (BenchConsumerThread*& w : workers)
			w = new BenchConsumerThread(&queue, aLoadType);
		BenchQueueWarmup(queue, workers);

		BenchValue* vals = new BenchValue[aItemCount];
		BenchRunReport report;
		{
			mg::common::MutexStatClear();
			TimedGuard timed("Push and pop");
			for (uint32_t i = 0; i < aItemCount; ++i)
				queue.Push(&vals[i]);
			uint64_t popCount = 0;
			while (popCount != aItemCount)
			{
				mg::common::Sleep(1);
				popCount = 0;
				for (BenchConsumerThread*& w : workers)
					popCount += w->StatGetCount();
			}
			timed.Stop();
			report.myMutexContentionCount = mg::common::MutexStatContentionCount();
			timed.Report();

			double durationMs = timed.GetMilliseconds();

			report.myItemsPerSec = (uint64_t)((uint64_t)aItemCount * 1000 / durationMs);
			report.myItemsPerSecPerThread = report.myItemsPerSec / aThreadCount;
			report.myThreads.resize(aThreadCount);
			for (uint32_t i = 0; i < aThreadCount; ++i)
			{
				BenchThreadReport& r = report.myThreads[i];
				const BenchConsumerThread* w = workers[i];
				r.myPopCount = w->StatGetCount();
			}
		}
		delete[] vals;
		for (BenchConsumerThread* w : workers)
			delete w;
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
	uint32_t runCount = 1;
	if (cmdLine.IsPresent("runs"))
		runCount = cmdLine.GetU32("runs");

	std::vector<BenchRunReport> reports;
	reports.resize(runCount);
	for (BenchRunReport& r : reports)
		r = BenchQueueRun(cmdLine);
	if (runCount < 3)
		return -1;
	std::sort(reports.begin(), reports.end());
	Report("");

	Report("== Aggregated report:");
	BenchRunReport* rMin = &reports[0];
	// If the count is even, then intentionally print the lower middle.
	BenchRunReport* rMed = &reports[runCount / 2];
	BenchRunReport* rMax = &reports[runCount - 1];
	Report("Items per second min:        %12llu",
		(unsigned long long)rMin->myItemsPerSec);
	Report("Items per second median:     %12llu",
		(unsigned long long)rMed->myItemsPerSec);
	Report("Items per second max:        %12llu",
		(unsigned long long)rMax->myItemsPerSec);
	Report("");

	Report("== Median report:");
	rMed->Print();
	return 0;
}
