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

#include "mg/common/HybridArray.h"
#include "mg/common/QPTimer.h"
#include "mg/common/ScratchPad.h"

#include <stdarg.h>

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
		mg::common::ScratchBuffer myName;
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
			mg::common::HybridString<32> myKey;
			mg::common::HybridString<32> myValue;
		};

		const Pair* PrivFind(
			const char* aName) const;

		const Pair& PrivGet(
			const char* aName) const;

		mg::common::HybridArray<Pair, 10> myArgs;
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

	void BenchMakeMicroWork();

	void BenchMakeHeavyWork();

}
}
