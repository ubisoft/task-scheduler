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

#include "Bench.h"

#include "mg/common/Array.h"
#include "mg/common/Mutex.h"
#include "mg/common/Random.h"
#include "mg/common/Thread.h"

#include <algorithm>

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

		uint64 StatGetCount() const;

	private:
		void Run() override;

		BenchQueueConsumer myConsumer;
		int64 myCount;
		int32 myPauseState;
		BenchLoadType myLoadType;
	};

	//////////////////////////////////////////////////////////////////////////////////////

	struct BenchThreadReport
	{
		BenchThreadReport();

		uint64 myPopCount;
	};

	struct BenchRunReport
	{
		BenchRunReport();

		bool operator<(
			const BenchRunReport& aOther) const;

		void Print() const;

		uint64 myItemsPerSec;
		uint64 myItemsPerSecPerThread;
		uint64 myMutexContentionCount;
		mg::common::HybridArray<BenchThreadReport, 10> myThreads;
	};

	//////////////////////////////////////////////////////////////////////////////////////

	static void BenchQueueWarmup(
		BenchQueue& aQueue,
		mg::common::IArray<BenchConsumerThread*>& aWorkers);

	static BenchRunReport BenchQueueRun(
		CommandLine& aCmdLine);

	static BenchRunReport BenchQueueRunPush(
		uint32 aItemCount,
		uint32 aSubQueueSize);

	static BenchRunReport BenchQueueRunPop(
		BenchLoadType aLoadType,
		uint32 aItemCount,
		uint32 aThreadCount,
		uint32 aSubQueueSize);

	static BenchRunReport BenchQueueRunPushPop(
		BenchLoadType aLoadType,
		uint32 aItemCount,
		uint32 aThreadCount,
		uint32 aSubQueueSize);

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
		int32 old = mg::common::AtomicExchange(&myPauseState, 1);
		MG_COMMON_ASSERT(old == 0);
		while (mg::common::AtomicLoad(&myPauseState) != 2)
			mg::common::Sleep(1);
	}

	void
	BenchConsumerThread::Continue()
	{
		int32 old = mg::common::AtomicExchange(&myPauseState, 0);
		MG_COMMON_ASSERT(old == 2);
	}

	void
	BenchConsumerThread::StatClear()
	{
		mg::common::AtomicExchange64(&myCount, 0);
	}

	uint64
	BenchConsumerThread::StatGetCount() const
	{
		return (uint64) mg::common::AtomicLoad64((int64*)&myCount);
	}

	void
	BenchConsumerThread::Run()
	{
		while (!StopRequested())
		{
			while (myConsumer.Pop() != nullptr)
			{
				mg::common::AtomicIncrement64(&myCount);
				if (myLoadType == BENCH_LOAD_MICRO)
					BenchMakeMicroWork();
			}
			int32 pause = mg::common::AtomicLoad(&myPauseState);
			if (pause != 0)
			{
				MG_COMMON_ASSERT(pause == 1);
				mg::common::AtomicExchange(&myPauseState, 2);
				while (mg::common::AtomicLoad(&myPauseState) != 0)
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
		if (myThreads.Count() > 1)
		{
			Report("Items per sec per thread: %12llu",
				(unsigned long long)myItemsPerSecPerThread);
		}
		Report("Mutex contention count:   %12llu",
			(unsigned long long)myMutexContentionCount);
		for (uint32 i = 0; i < myThreads.Count(); ++i)
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
		mg::common::IArray<BenchConsumerThread*>& aWorkers)
	{
		BenchValue* vals = new BenchValue[MG_WARMUP_ITEM_COUNT];
		for (uint32 i = 0; i < MG_WARMUP_ITEM_COUNT; ++i)
			aQueue.Push(&vals[i]);
		uint64 popCount = 0;
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
		uint32 itemCount = aCmdLine.GetU32("items");
		// It is optional for non-block-based queues.
		uint32 subQueueSize = 0;
		if (aCmdLine.IsPresent("subqsize"))
			subQueueSize = aCmdLine.GetU32("subqsize");
		if (mg::common::Strcmp(operation, "push") == 0)
			return BenchQueueRunPush(itemCount, subQueueSize);

		BenchLoadType loadType = BenchLoadTypeFromString(aCmdLine.GetStr("load"));
		uint32 threadCount = aCmdLine.GetU32("threads");
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
		uint32 aItemCount,
		uint32 aSubQueueSize)
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
			for (uint32 i = 0; i < aItemCount; ++i)
				queue.Push(&vals[i]);
			timed.Stop();
			report.myMutexContentionCount = mg::common::MutexStatContentionCount();
			timed.Report();

			double durationMs = timed.GetMilliseconds();
			report.myItemsPerSec = (uint64) ((uint64) aItemCount * 1000 / durationMs);
		}
		delete[] vals;
		report.Print();
		return report;
	}

	static BenchRunReport
	BenchQueueRunPop(
		BenchLoadType aLoadType,
		uint32 aItemCount,
		uint32 aThreadCount,
		uint32 aSubQueueSize)
	{
		BenchCaseGuard guard("Operation pop, item=%u, thread=%u, subq=%u, load=%s",
			aItemCount, aThreadCount, aSubQueueSize, BenchLoadTypeToString(aLoadType));
		BenchQueue queue(aSubQueueSize);
		queue.Reserve(aItemCount);
		mg::common::Array<BenchConsumerThread*> workers;
		workers.SetCount(aThreadCount);
		for (BenchConsumerThread*& w : workers)
			w = new BenchConsumerThread(&queue, aLoadType);
		BenchQueueWarmup(queue, workers);

		// Pause the consumers to isolate push and pop phases.
		for (BenchConsumerThread*& w : workers)
			w->Pause();
		BenchValue* vals = new BenchValue[aItemCount];
		for (uint32 i = 0; i < aItemCount; ++i)
			queue.Push(&vals[i]);

		BenchRunReport report;
		{
			mg::common::MutexStatClear();
			TimedGuard timed("Consume");
			// Unleash the consumers. Recreating them wouldn't be a good idea as thread
			// start takes a lot of time. Better pause + continue existing threads.
			for (BenchConsumerThread*& w : workers)
				w->Continue();
			uint64 popCount = 0;
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

			report.myItemsPerSec = (uint64)((uint64)aItemCount * 1000 / durationMs);
			report.myItemsPerSecPerThread = report.myItemsPerSec / aThreadCount;
			report.myThreads.SetCount(aThreadCount);
			for (uint32 i = 0; i < aThreadCount; ++i)
			{
				BenchThreadReport& r = report.myThreads[i];
				const BenchConsumerThread* w = workers[i];
				r.myPopCount = w->StatGetCount();
			}
		}
		delete[] vals;
		workers.DeleteAll();
		report.Print();
		return report;
	}

	static BenchRunReport
	BenchQueueRunPushPop(
		BenchLoadType aLoadType,
		uint32 aItemCount,
		uint32 aThreadCount,
		uint32 aSubQueueSize)
	{
		BenchCaseGuard guard("Operation push-pop, item=%u, thread=%u, subq=%u, load=%s",
			aItemCount, aThreadCount, aSubQueueSize, BenchLoadTypeToString(aLoadType));
		BenchQueue queue(aSubQueueSize);
		queue.Reserve(aItemCount);
		mg::common::Array<BenchConsumerThread*> workers;
		workers.SetCount(aThreadCount);
		for (BenchConsumerThread*& w : workers)
			w = new BenchConsumerThread(&queue, aLoadType);
		BenchQueueWarmup(queue, workers);

		BenchValue* vals = new BenchValue[aItemCount];
		BenchRunReport report;
		{
			mg::common::MutexStatClear();
			TimedGuard timed("Push and pop");
			for (uint32 i = 0; i < aItemCount; ++i)
				queue.Push(&vals[i]);
			uint64 popCount = 0;
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

			report.myItemsPerSec = (uint64)((uint64)aItemCount * 1000 / durationMs);
			report.myItemsPerSecPerThread = report.myItemsPerSec / aThreadCount;
			report.myThreads.SetCount(aThreadCount);
			for (uint32 i = 0; i < aThreadCount; ++i)
			{
				BenchThreadReport& r = report.myThreads[i];
				const BenchConsumerThread* w = workers[i];
				r.myPopCount = w->StatGetCount();
			}
		}
		delete[] vals;
		workers.DeleteAll();
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
	uint32 runCount = 1;
	if (cmdLine.IsPresent("runs"))
		runCount = cmdLine.GetU32("runs");

	mg::common::HybridArray<BenchRunReport, 10> reports;
	reports.SetCount(runCount);
	for (BenchRunReport& r : reports)
		r = BenchQueueRun(cmdLine);
	if (runCount < 3)
		return -1;
	std::sort(reports.GetBuffer(), reports.GetBuffer() + runCount);
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
