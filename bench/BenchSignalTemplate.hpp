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

#include "mg/common/Atomic.h"
#include "mg/common/Thread.h"

#include <algorithm>

namespace mg {
namespace bench {

	class BenchThreadSender
		: public mg::common::Thread
	{
	public:
		BenchThreadSender(
			Signal& aSignal,
			int64& aQueue,
			BenchLoadType aLoadType,
			bool aDoSignalOnEachItem,
			int64& aSharedSigCount);

		~BenchThreadSender() override;

		void Begin();

		uint64 StatPopSigCount();

		uint64 StatPopItemCount();

	private:
		void Run() override;

		Signal& mySignal;
		MG_BENCH_FALSE_SHARING_BARRIER(myPadding1);
		int64& myQueue;
		MG_BENCH_FALSE_SHARING_BARRIER(myPadding2);
		int64 mySigCount;
		MG_BENCH_FALSE_SHARING_BARRIER(myPadding3);
		int64 myItemCount;
		MG_BENCH_FALSE_SHARING_BARRIER(myPadding4);
		int64& mySharedSigCount;
		MG_BENCH_FALSE_SHARING_BARRIER(myPadding5);
		BenchLoadType myLoadType;
		const bool myDoSignalOnEachItem;
		int32 myIsPaused;
	};

	class BenchThreadReceiver
		: public mg::common::Thread
	{
	public:
		BenchThreadReceiver(
			Signal& aSignal,
			int64& aQueue,
			BenchLoadType aLoadType,
			int64& aSharedSigCount);

		~BenchThreadReceiver() override;

		void Begin();

		uint64 StatPopSigCount();

		uint64 StatPopItemCount();

	private:
		void Run() override;

		Signal& mySignal;
		MG_BENCH_FALSE_SHARING_BARRIER (myPadding1);
		int64& myQueue;
		MG_BENCH_FALSE_SHARING_BARRIER (myPadding2);
		int64 mySigCount;
		MG_BENCH_FALSE_SHARING_BARRIER (myPadding3);
		int64 myItemCount;
		MG_BENCH_FALSE_SHARING_BARRIER (myPadding4);
		int64& mySharedSigCount;
		MG_BENCH_FALSE_SHARING_BARRIER (myPadding5);
		BenchLoadType myLoadType;
		int32 myIsPaused;
	};

	//////////////////////////////////////////////////////////////////////////////////////

	struct BenchThreadReport
	{
		BenchThreadReport();

		uint64 mySigCount;
		uint64 myItemCount;
	};

	struct BenchRunReport
	{
		BenchRunReport();

		bool operator<(
			const BenchRunReport& aOther) const;

		void Print() const;

		uint64 myItemsPerSec;
		uint64 mySigPerSec;
		uint64 myMutexContentionCountPerSec;
		mg::common::HybridArray<BenchThreadReport, 10> mySenders;
		mg::common::HybridArray<BenchThreadReport, 10> myReceivers;
	};

	//////////////////////////////////////////////////////////////////////////////////////

	static BenchRunReport BenchSignalRun(
		BenchLoadType aLoadType,
		bool aDoSignalOnEachItem,
		uint32 aSenderCount,
		uint32 aReceiverCount,
		uint64 aSignalCount);

	//////////////////////////////////////////////////////////////////////////////////////

	BenchThreadSender::BenchThreadSender(
		Signal& aSignal,
		int64& aQueue,
		BenchLoadType aLoadType,
		bool aDoSignalOnEachItem,
		int64& aSharedSigCount)
		: mySignal(aSignal)
		, myQueue(aQueue)
		, mySigCount(0)
		, myItemCount(0)
		, mySharedSigCount(aSharedSigCount)
		, myLoadType(aLoadType)
		, myDoSignalOnEachItem(aDoSignalOnEachItem)
		, myIsPaused(0)
	{
		Start();
		while (mg::common::AtomicFlagTest(&myIsPaused) == 0)
			mg::common::Sleep(1);
	}

	BenchThreadSender::~BenchThreadSender()
	{
		BlockingStop();
	}

	void
	BenchThreadSender::Begin()
	{
		MG_COMMON_ASSERT(mg::common::AtomicFlagClear(&myIsPaused) == 1);
	}

	uint64
	BenchThreadSender::StatPopSigCount()
	{
		return (uint64)mg::common::AtomicExchange64(&mySigCount, 0);
	}

	uint64
	BenchThreadSender::StatPopItemCount()
	{
		return (uint64)mg::common::AtomicExchange64(&myItemCount, 0);
	}

	void
	BenchThreadSender::Run()
	{
		MG_COMMON_ASSERT(mg::common::AtomicFlagSet(&myIsPaused) == 0);
		while (mg::common::AtomicFlagTest(&myIsPaused) != 0);
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
			mg::common::AtomicIncrement64(&myItemCount);
			if (mg::common::AtomicIncrement64(&myQueue) != 1 && !myDoSignalOnEachItem)
				continue;
			if (!mySignal.Send())
				continue;
			if (mg::common::AtomicDecrement64(&mySharedSigCount) < 0)
				break;
			mg::common::AtomicIncrement64(&mySigCount);
		}
	}

	BenchThreadReceiver::BenchThreadReceiver(
		Signal& aSignal,
		int64& aQueue,
		BenchLoadType aLoadType,
		int64& aSharedSigCount)
		: mySignal(aSignal)
		, myQueue(aQueue)
		, mySigCount(0)
		, myItemCount(0)
		, mySharedSigCount(aSharedSigCount)
		, myLoadType(aLoadType)
		, myIsPaused(0)
	{
		Start();
		while (mg::common::AtomicFlagTest(&myIsPaused) == 0)
			mg::common::Sleep(1);
	}

	BenchThreadReceiver::~BenchThreadReceiver()
	{
		BlockingStop();
	}

	void
	BenchThreadReceiver::Begin()
	{
		MG_COMMON_ASSERT(mg::common::AtomicFlagClear(&myIsPaused) == 1);
	}

	uint64
	BenchThreadReceiver::StatPopSigCount()
	{
		return (uint64)mg::common::AtomicExchange64(&mySigCount, 0);
	}

	uint64
	BenchThreadReceiver::StatPopItemCount()
	{
		return (uint64)mg::common::AtomicExchange64(&myItemCount, 0);
	}

	void
	BenchThreadReceiver::Run()
	{
		MG_COMMON_ASSERT(mg::common::AtomicFlagSet(&myIsPaused) == 0);
		while (mg::common::AtomicFlagTest(&myIsPaused) != 0);
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
			mySignal.ReceiveBlocking();
			mg::common::AtomicAdd64(&myItemCount,
				mg::common::AtomicExchange64(&myQueue, 0));

			if (mg::common::AtomicDecrement64(&mySharedSigCount) < 0)
				break;
			mg::common::AtomicIncrement64(&mySigCount);
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////

	BenchThreadReport::BenchThreadReport()
		: mySigCount(0)
		, myItemCount(0)
	{
	}

	BenchRunReport::BenchRunReport()
		: myItemsPerSec(0)
		, mySigPerSec(0)
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
		Report("Signals/sec:                %12llu",
			(unsigned long long)mySigPerSec);
		Report("Mutex contentions/sec:      %12llu",
			(unsigned long long)myMutexContentionCountPerSec);
		for (uint32 i = 0; i < mySenders.Count(); ++i)
		{
			const BenchThreadReport& tr = mySenders[i];
			Report("Thread sender %2u:   sigs: %12llu, items: %12llu", i,
				(unsigned long long)tr.mySigCount,
				(unsigned long long)tr.myItemCount);
		}
		for (uint32 i = 0; i < myReceivers.Count(); ++i)
		{
			const BenchThreadReport& tr = myReceivers[i];
			Report("Thread receiver %2u: sigs: %12llu, items: %12llu", i,
				(unsigned long long)tr.mySigCount,
				(unsigned long long)tr.myItemCount);
		}
		Report("");
	}

	//////////////////////////////////////////////////////////////////////////////////////

	static BenchRunReport
	BenchSignalRun(
		BenchLoadType aLoadType,
		bool aDoSignalOnEachItem,
		uint32 aSenderCount,
		uint32 aReceiverCount,
		uint64 aSignalCount)
	{
		Signal signal;
		mg::common::HybridArray<BenchThreadReceiver*, 10> receivers;
		receivers.SetCount(aReceiverCount);
		mg::common::HybridArray<BenchThreadSender*, 10> senders;
		senders.SetCount(aSenderCount);

		int64 sharedSendSigCount = aSignalCount;
		int64 sharedRecvSigCount = aSignalCount;
		int64 queue = 0;
		for (BenchThreadReceiver*& r : receivers)
		{
			r = new BenchThreadReceiver(signal, queue, aLoadType, sharedRecvSigCount);
		}
		for (BenchThreadSender*& s : senders)
		{
			s = new BenchThreadSender(signal, queue, aLoadType, aDoSignalOnEachItem,
				sharedSendSigCount);
		}

		BenchCaseGuard guard("Load %s, senders=%u, receivers=%u, sigcount=%llu",
			BenchLoadTypeToString(aLoadType), aSenderCount, aReceiverCount,
			(unsigned long long) aSignalCount);
		BenchRunReport report;

		mg::common::MutexStatClear();
		TimedGuard timed("Wait signals");
		for (BenchThreadSender* s : senders)
			s->Begin();
		for (BenchThreadReceiver* r : receivers)
			r->Begin();
		while (mg::common::AtomicLoad64(&sharedRecvSigCount) > 0 ||
			mg::common::AtomicLoad64(&sharedSendSigCount) > 0)
			mg::common::Sleep(1);
		timed.Stop();
		uint64 mutexContentionCount = mg::common::MutexStatContentionCount();
		timed.Report();
		double durationMs = timed.GetMilliseconds();

		report.mySigPerSec = (uint64)(aSignalCount * 1000 / durationMs);
		report.myMutexContentionCountPerSec =
			(uint64)(mutexContentionCount * 1000 / durationMs);
		report.mySenders.SetCount(aSenderCount);
		report.myReceivers.SetCount(aReceiverCount);
		for (uint32 i = 0; i < aSenderCount; ++i)
		{
			BenchThreadReport& tr = report.mySenders[i];
			BenchThreadSender* s = senders[i];
			tr.mySigCount = s->StatPopSigCount();
			tr.myItemCount = s->StatPopItemCount();
		}
		uint64 itemCount = 0;
		for (uint32 i = 0; i < aReceiverCount; ++i)
		{
			BenchThreadReport& tr = report.myReceivers[i];
			BenchThreadReceiver* r = receivers[i];
			tr.mySigCount = r->StatPopSigCount();
			tr.myItemCount = r->StatPopItemCount();
			itemCount += tr.myItemCount;
			r->Stop();
			while (r->IsRunning())
			{
				signal.Send();
				mg::common::Sleep(1);
			}
		}
		report.myItemsPerSec = (uint64)(itemCount * 1000 / durationMs);
		report.Print();
		senders.DeleteAll();
		receivers.DeleteAll();
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
	bool doSigEachOnItem = cmdLine.GetU32("each") != 0;
	uint32 senderCount = cmdLine.GetU32("senders");
	uint32 receiverCount = cmdLine.GetU32("receivers");
	uint64 sigCount = cmdLine.GetU64("sigs");
	uint32 runCount = 1;
	if (cmdLine.IsPresent("runs"))
		runCount = cmdLine.GetU32("runs");

	mg::common::HybridArray<BenchRunReport, 10> reports;
	reports.SetCount(runCount);
	for (BenchRunReport& r : reports)
	{
		r = BenchSignalRun(loadType, doSigEachOnItem, senderCount, receiverCount,
			sigCount);
	}
	if (runCount < 3)
		return -1;
	std::sort(reports.GetBuffer(), reports.GetBuffer() + runCount);
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
