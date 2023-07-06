#include "Bench.h"

#include "mg/common/Assert.h"
#include "mg/common/Atomic.h"
#include "mg/common/Util.h"

#include "mg/test/Random.h"

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
		std::string buf = mg::common::StringVFormat(aFormat, aArg);
		printf("%s\n", buf.c_str());
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
		std::string buf = mg::common::StringVFormat(aFormat, va);
		va_end(va);
		Report("==== [%s] case", buf.c_str());
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
		myName = mg::common::StringVFormat(aFormat, va);
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
			myName.c_str(), myDuration);
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
			uint32_t argi = (uint32_t)myArgs.size();
			myArgs.resize(argi + 1);
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
		return PrivGet(aName).myValue.c_str();
	}

	uint64_t
	CommandLine::GetU64(
		const char* aName) const
	{
		uint64_t res = 0;
		MG_COMMON_ASSERT_F(mg::common::StringToNumber(
			PrivGet(aName).myValue.c_str(), res),
			"Couldn't convert arg %s to uint64", aName);
		return res;
	}

	uint32_t
	CommandLine::GetU32(
		const char* aName) const
	{
		uint32_t res = 0;
		MG_COMMON_ASSERT_F(mg::common::StringToNumber(
			PrivGet(aName).myValue.c_str(), res),
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
			mg::common::AtomicBool flag(mg::test::RandomBool());
			flag.Store(true);
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
