#pragma once

#include "mg/common/QPTimer.h"
#include "mg/common/StringFunctions.h"

#include <stdarg.h>
#include <string>
#include <vector>

#define MG_BENCH_FALSE_SHARING_BARRIER(name) \
	MG_UNUSED_MEMBER char name[128]

namespace mg {
namespace bench {

	MG_STRFORMAT_PRINTF(1, 2)
	void Report(
		const char *aFormat,
		...);

	MG_STRFORMAT_PRINTF(1, 0)
	void ReportV(
		const char *aFormat,
		va_list aArg);

	struct BenchSuiteGuard
	{
		BenchSuiteGuard(
			const char* aName);
	};

	struct BenchCaseGuard
	{
		MG_STRFORMAT_PRINTF(2, 3)
		BenchCaseGuard(
			const char* aFormat,
			...);
	};

	struct TimedGuard
	{
		MG_STRFORMAT_PRINTF(2, 3)
		TimedGuard(
			const char* aFormat,
			...);

		void Stop();

		void Report();

		double GetMilliseconds();

	private:
		mg::common::QPTimer myTimer;
		std::string myName;
		bool myIsStopped;
		double myDuration;
	};

	class CommandLine
	{
	public:
		CommandLine(
			int aArgc,
			const char* const* aArgv);

		bool IsPresent(
			const char* aName) const;

		const char* GetStr(
			const char* aName) const;

		uint64 GetU64(
			const char* aName) const;

		uint32 GetU32(
			const char* aName) const;

	private:
		struct Pair
		{
			std::string myKey;
			std::string myValue;
		};

		const Pair* PrivFind(
			const char* aName) const;

		const Pair& PrivGet(
			const char* aName) const;

		std::vector<Pair> myArgs;
	};

	enum BenchLoadType
	{
		BENCH_LOAD_EMPTY,
		BENCH_LOAD_NANO,
		BENCH_LOAD_MICRO,
		BENCH_LOAD_HEAVY,
	};

	const char* BenchLoadTypeToString(
		BenchLoadType aVal);

	BenchLoadType BenchLoadTypeFromString(
		const char* aVal);

	void BenchMakeNanoWork();

	void BenchMakeMicroWork();

	void BenchMakeHeavyWork();

}
}
