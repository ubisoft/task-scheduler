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

#include "Bench.h"

#include "mg/common/Atomic.h"
#include "mg/common/Random.h"

#include <ctype.h>
#include <stdio.h>

namespace mg {
namespace bench {

	MG_STRFORMAT_PRINTF(1, 2)
	void
	Report(
		const char *aFormat,
		...)
	{
		va_list va;
		va_start(va, aFormat);
		ReportV(aFormat, va);
		va_end(va);
	}

	MG_STRFORMAT_PRINTF(1, 0)
	void
	ReportV(
		const char *aFormat,
		va_list aArg)
	{
		mg::common::ScratchBuffer buf = mg::common::ScratchVsprintf(aFormat, aArg);
		printf("%s\n", buf.GetBuffer());
	}

	BenchSuiteGuard::BenchSuiteGuard(
		const char* aName)
	{
		Report("======== [%s] suite", aName);
	}

	MG_STRFORMAT_PRINTF(2, 3)
	BenchCaseGuard::BenchCaseGuard(
		const char* aFormat,
		...)
	{
		va_list va;
		va_start(va, aFormat);
		mg::common::ScratchBuffer buf = mg::common::ScratchVsprintf(aFormat, va);
		va_end(va);
		Report("==== [%s] case", buf.GetBuffer());
	}

	MG_STRFORMAT_PRINTF(2, 3)
	TimedGuard::TimedGuard(
		const char* aFormat,
		...)
		: myIsStopped(false)
		, myDuration(0)
	{
		va_list va;
		va_start(va, aFormat);
		myName = mg::common::ScratchVsprintf(aFormat, va);
		va_end(va);
		myTimer.Start();
	}

	void
	TimedGuard::Stop()
	{
		MG_COMMON_ASSERT(!myIsStopped);
		myDuration = myTimer.GetMilliSeconds();
		myIsStopped = true;
	}

	void
	TimedGuard::Report()
	{
		MG_COMMON_ASSERT(myIsStopped);
		mg::bench::Report("== [%s] took %.6lf ms",
			myName.GetBuffer(), myDuration);
	}

	double
	TimedGuard::GetMilliseconds()
	{
		MG_COMMON_ASSERT(myIsStopped);
		return myTimer.GetMilliSeconds();
	}

	CommandLine::CommandLine(
		int aArgc,
		const char* const* aArgv)
	{
		int i = 0;
		while (i < aArgc)
		{
			const char* arg = aArgv[i];
			++i;
			if (arg[0] != '-')
				continue;
			++arg;
			const char* pos = arg;
			MG_COMMON_ASSERT_F(*pos != 0, "Argument %d is empty", i);
			while (*pos != 0)
			{
				char c = *pos;
				MG_COMMON_ASSERT_F(isalpha(c) || isdigit(c) || c == '-' || c == '_',
					"Argument %d has invalid chars", i);
				++pos;
			}
			uint32 argi = myArgs.Count();
			myArgs.SetCount(argi + 1);
			Pair& p = myArgs[argi];
			p.myKey = arg;
			if (i < aArgc)
			{
				p.myValue = aArgv[i];
				++i;
			}
		}
	}

	bool
	CommandLine::IsPresent(
		const char* aName) const
	{
		return PrivFind(aName) != nullptr;
	}

	const char*
	CommandLine::GetStr(
		const char* aName) const
	{
		return PrivGet(aName).myValue;
	}

	uint64
	CommandLine::GetU64(
		const char* aName) const
	{
		uint64 res = 0;
		MG_COMMON_ASSERT_F(mg::common::StringToNumber(PrivGet(aName).myValue, res),
			"Couldn't convert arg %s to uint64", aName);
		return res;
	}

	uint32
	CommandLine::GetU32(
		const char* aName) const
	{
		uint32 res = 0;
		MG_COMMON_ASSERT_F(mg::common::StringToNumber(PrivGet(aName).myValue, res),
			"Couldn't convert arg %s to uint64", aName);
		return res;
	}

	const CommandLine::Pair*
	CommandLine::PrivFind(
		const char* aName) const
	{
		for (const Pair& p : myArgs)
		{
			if (p.myKey == aName)
				return &p;
		}
		return nullptr;
	}

	const CommandLine::Pair&
	CommandLine::PrivGet(
		const char* aName) const
	{
		const Pair* p = PrivFind(aName);
		MG_COMMON_ASSERT_F(p != nullptr, "Couldn't get arg %s", aName);
		return *p;
	}

	const char*
	BenchLoadTypeToString(
		BenchLoadType aVal)
	{
		switch (aVal)
		{
		case BENCH_LOAD_EMPTY: return "empty";
		case BENCH_LOAD_NANO: return "nano";
		case BENCH_LOAD_MICRO: return "micro";
		case BENCH_LOAD_HEAVY: return "heavy";
		default: MG_COMMON_ASSERT(false); return nullptr;
		}
	}

	BenchLoadType
	BenchLoadTypeFromString(
		const char* aVal)
	{
		if (mg::common::Strcmp(aVal, "empty") == 0)
			return BENCH_LOAD_EMPTY;
		if (mg::common::Strcmp(aVal, "nano") == 0)
			return BENCH_LOAD_NANO;
		if (mg::common::Strcmp(aVal, "micro") == 0)
			return BENCH_LOAD_MICRO;
		if (mg::common::Strcmp(aVal, "heavy") == 0)
			return BENCH_LOAD_HEAVY;
		MG_COMMON_ASSERT_F(false, "Uknown load type '%s'", aVal);
		return BENCH_LOAD_EMPTY;
	}

	static inline void
	BenchMakeWork(
		int aCount)
	{
		for (int i = 0; i < aCount; ++i)
		{
			int32 flag = mg::common::RandomBool();
			mg::common::AtomicFlagSet(&flag);
		}
	}

	void
	BenchMakeNanoWork()
	{
		BenchMakeWork(20);
	}

	void
	BenchMakeMicroWork()
	{
		BenchMakeWork(100);
	}

	void
	BenchMakeHeavyWork()
	{
		BenchMakeWork(500);
	}

}
}
