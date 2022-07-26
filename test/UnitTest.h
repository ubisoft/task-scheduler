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

#include "mg/common/QPTimer.h"
#include "mg/common/Types.h"

#include <cstdarg>

#if IS_COMPILER_CLANG
// Clang does not put namespaces into __FUNCTION__.
// __PRETTY_FUNCTION__ contains the whole signature, including
// namespaces. Unfortunately, Clang does not feature a macros,
// which would print function name + namespaces without arguments.
#define FUNCTION_NAME_WITH_NAMESPACE __PRETTY_FUNCTION__
#else
#define FUNCTION_NAME_WITH_NAMESPACE __FUNCTION__
#endif

namespace mg {
namespace unittests {

	MG_STRFORMAT_PRINTF(1, 2)
	void Report(
		const char *aFormat,
		...);

	MG_STRFORMAT_PRINTF(1, 0)
	void ReportV(
		const char *aFormat,
		va_list aArg);

	struct TestSuiteGuard
	{
		TestSuiteGuard(
			const char* aName);

		~TestSuiteGuard();

	private:
		mg::common::QPTimer myTimer;
	};

	struct TestCaseGuard
	{
		MG_STRFORMAT_PRINTF(2, 3)
		TestCaseGuard(
			const char* aFormat,
			...);

		~TestCaseGuard();

	private:
		mg::common::QPTimer myTimer;
	};

}
}
