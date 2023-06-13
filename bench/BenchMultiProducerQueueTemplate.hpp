#pragma once

#include "Bench.h"

#include "mg/common/ForwardList.h"
#include "mg/common/Signal.h"
#include "mg/common/Thread.h"

#include <algorithm>
#include <vector>

namespace mg {
namespace bench {

	using BenchValueList = mg::common::ForwardList<BenchValue>;

	class BenchThreadSender
		: public mg::common::Thread
	{
	public:
		BenchThreadSender(
			MultiProducerQueue& aQueue,
			mg::common::Signal* aSignal,
			mg::common::AtomicU64& aSharedQueueSize,
			uint64_t aItemCount,
			BenchLoadType aLoadType);

		~BenchThreadSender() override;

		void Begin();

	private:
		void Run() override;

		MultiProducerQueue& myQueue;
		MG_BENCH_FALSE_SHARING_BARRIER(myPadding1);
		mg::common::AtomicU64& mySharedQueueSize;
		MG_BENCH_FALSE_SHARING_BARRIER(myPadding2);
		const uint64_t myItemCount;
		BenchValueList myItems;
		MG_BENCH_FALSE_SHARING_BARRIER(myPadding3);
		mg::common::Signal* mySignal;
		BenchLoadType myLoadType;
		mg::common::AtomicBool myIsPaused;
	};

	class BenchThreadReceiver
		: public mg::common::Thread
	{
	public:
		BenchThreadReceiver(
			MultiProducerQueue& aQueue,
			mg::common::Signal* aSignal,
			mg::common::AtomicU64& aSharedQueueSize,
			BenchLoadType aLoadType);

		~BenchThreadReceiver() override;

		void Begin();

		uint64_t StatPopItemCount();

	private:
		void Run() override;

		MultiProducerQueue& myQueue;
		MG_BENCH_FALSE_SHARING_BARRIER(myPadding1);
		mg::common::AtomicU64& mySharedQueueSize;
		MG_BENCH_FALSE_SHARING_BARRIER(myPadding2);
		mg::common::AtomicU64 myItemCount;
		BenchValueList myItems;
		MG_BENCH_FALSE_SHARING_BARRIER(myPadding3);
		mg::common::Signal* mySignal;
		BenchLoadType myLoadType;
		mg::common::AtomicBool myIsPaused;
	};

	//////////////////////////////////////////////////////////////////////////////////////

	struct BenchRunReport
	{
		BenchRunReport();

		bool operator<(
			const BenchRunReport& aOther) const;

		void Print() const;

		uint64_t myItemsPerSec;
		uint64_t myMutexContentionCountPerSec;
	};

	//////////////////////////////////////////////////////////////////////////////////////

	static BenchRunReport BenchQueueRun(
		BenchLoadType aLoadType,
		bool aDoUseSignal,
		uint64_t aItemCount,
		uint32_t aSenderCount);

	//////////////////////////////////////////////////////////////////////////////////////

	BenchThreadSender::BenchThreadSender(
		MultiProducerQueue& aQueue,
		mg::common::Signal* aSignal,
		mg::common::AtomicU64& aSharedQueueSize,
		uint64_t aItemCount,
		BenchLoadType aLoadType)
		: myQueue(aQueue)
		, mySharedQueueSize(aSharedQueueSize)
		, myItemCount(aItemCount)
		, mySignal(aSignal)
		, myLoadType(aLoadType)
		, myIsPaused(false)
	{
		Start();
		while (!myIsPaused.LoadAcquire())
			mg::common::Sleep(1);
	}

	BenchThreadSender::~BenchThreadSender()
	{
		BlockingStop();
	}

	void
	BenchThreadSender::Begin()
	{
		MG_COMMON_ASSERT(myIsPaused.ExchangeRelease(false));
	}

	void
	BenchThreadSender::Run()
	{
		for (uint64_t i = 0; i < myItemCount; ++i)
			myItems.Append(new BenchValue());
		MG_COMMON_ASSERT(!myIsPaused.ExchangeRelease(true));
		while (myIsPaused.LoadAcquire());

		while (!StopRequested() && !myItems.IsEmpty())
		{
			switch(myLoadType)
			{
				case BENCH_LOAD_EMPTY:
					break;
				case BENCH_LOAD_NANO:
					BenchMakeNanoWork();
					break;
				case BENCH_LOAD_MICRO:
					BenchMakeMicroWork();
					break;
				case BENCH_LOAD_HEAVY:
				default:
					MG_COMMON_ASSERT(false);
					break;
			}
			mySharedQueueSize.IncrementRelaxed();
			if (myQueue.Push(myItems.PopFirst()) && mySignal != nullptr)
				mySignal->Send();
		}
	}

	BenchThreadReceiver::BenchThreadReceiver(
		MultiProducerQueue& aQueue,
		mg::common::Signal* aSignal,
		mg::common::AtomicU64& aSharedQueueSize,
		BenchLoadType aLoadType)
		: myQueue(aQueue)
		, mySharedQueueSize(aSharedQueueSize)
		, myItemCount(0)
		, mySignal(aSignal)
		, myLoadType(aLoadType)
		, myIsPaused(false)
	{
		Start();
		while (!myIsPaused.LoadAcquire())
			mg::common::Sleep(1);
	}

	BenchThreadReceiver::~BenchThreadReceiver()
	{
		Stop();
		while (IsRunning())
		{
			if (mySignal != nullptr)
				mySignal->Send();
			mg::common::Sleep(1);
		}
		BlockingStop();
		while (!myItems.IsEmpty())
			delete myItems.PopFirst();
	}

	void
	BenchThreadReceiver::Begin()
	{
		MG_COMMON_ASSERT(myIsPaused.ExchangeRelease(false));
	}

	uint64_t
	BenchThreadReceiver::StatPopItemCount()
	{
		return myItemCount.ExchangeRelaxed(0);
	}

	void
	BenchThreadReceiver::Run()
	{
		MG_COMMON_ASSERT(!myIsPaused.ExchangeRelease(true));
		while (myIsPaused.LoadAcquire());

		while (!StopRequested())
		{
			switch(myLoadType)
			{
				case BENCH_LOAD_EMPTY:
					break;
				case BENCH_LOAD_NANO:
					BenchMakeNanoWork();
					break;
				case BENCH_LOAD_MICRO:
					BenchMakeMicroWork();
					break;
				case BENCH_LOAD_HEAVY:
				default:
					MG_COMMON_ASSERT(false);
					break;
			}
			if (mySignal != nullptr)
				mySignal->ReceiveBlocking();
			BenchValue* tail;
			BenchValue* head = myQueue.PopAll(tail);
			myItems.Append(head, tail);
			myItemCount.AddRelaxed(mySharedQueueSize.ExchangeRelaxed(0));
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////

	BenchRunReport::BenchRunReport()
		: myItemsPerSec(0)
		, myMutexContentionCountPerSec(0)
	{
	}

	inline bool
	BenchRunReport::operator<(
		const BenchRunReport& aOther) const
	{
		return myItemsPerSec < aOther.myItemsPerSec;
	}

	void
	BenchRunReport::Print() const
	{
		Report("Items/sec:                  %12llu",
			(unsigned long long)myItemsPerSec);
		Report("Mutex contentions/sec:      %12llu",
			(unsigned long long)myMutexContentionCountPerSec);
		Report("");
	}

	//////////////////////////////////////////////////////////////////////////////////////

	static BenchRunReport
	BenchQueueRun(
		BenchLoadType aLoadType,
		bool aDoUseSignal,
		uint64_t aItemCount,
		uint32_t aSenderCount)
	{
		mg::common::Signal signal;
		mg::common::Signal* signalPtr = nullptr;
		if (aDoUseSignal)
			signalPtr = &signal;

		MultiProducerQueue queue;
		mg::common::AtomicU64 sharedQueueSize(0);
		BenchThreadReceiver* receiver = new BenchThreadReceiver(queue, signalPtr,
			sharedQueueSize, aLoadType);
		std::vector<BenchThreadSender*> senders;
		senders.resize(aSenderCount);
		for (BenchThreadSender*& s : senders)
		{
			// +1 to deal with [item count % thread count != 0].
			s = new BenchThreadSender(queue, signalPtr, sharedQueueSize,
				aItemCount / aSenderCount + 1, aLoadType);
		}

		BenchCaseGuard guard("Load %s, senders=%u, items total=%llu, with signal=%u",
			BenchLoadTypeToString(aLoadType), aSenderCount,
			(unsigned long long)aItemCount, (int)aDoUseSignal);
		BenchRunReport report;

		mg::common::MutexStatClear();
		TimedGuard timed("Wait receipt");
		receiver->Begin();
		for (BenchThreadSender* s : senders)
			s->Begin();
		uint64_t progress = 0;
		while (progress < aItemCount)
		{
			progress += receiver->StatPopItemCount();
			mg::common::Sleep(1);
		}
		timed.Stop();
		uint64_t mutexContentionCount = mg::common::MutexStatContentionCount();
		timed.Report();
		double durationMs = timed.GetMilliseconds();

		delete receiver;
		receiver = nullptr;
		for (BenchThreadSender* s : senders)
			delete s;

		report.myItemsPerSec = (uint64_t)(aItemCount * 1000 / durationMs);
		report.myMutexContentionCountPerSec =
			(uint64_t)(mutexContentionCount * 1000 / durationMs);
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
	uint32_t senderCount = cmdLine.GetU32("senders");
	uint32_t itemCount = cmdLine.GetU32("items");
	bool doUseSignal = cmdLine.GetU32("signal") != 0;
	uint32_t runCount = 1;
	if (cmdLine.IsPresent("runs"))
		runCount = cmdLine.GetU32("runs");

	std::vector<BenchRunReport> reports;
	reports.resize(runCount);
	for (BenchRunReport& r : reports)
		r = BenchQueueRun(loadType, doUseSignal, itemCount, senderCount);
	if (runCount < 3)
		return -1;
	std::sort(reports.begin(), reports.end());
	Report("");

	Report("== Aggregated report:");
	BenchRunReport* rMin = &reports[0];
	// If the count is even, then intentionally print the lower middle.
	BenchRunReport* rMed = &reports[runCount / 2];
	BenchRunReport* rMax = &reports[runCount - 1];
	Report("Items/sec min:              %12llu",
		(unsigned long long)rMin->myItemsPerSec);
	Report("Items/sec median:           %12llu",
		(unsigned long long)rMed->myItemsPerSec);
	Report("Items/sec max:              %12llu",
		(unsigned long long)rMax->myItemsPerSec);
	Report("");

	Report("== Median report:");
	rMed->Print();
	return 0;
}
