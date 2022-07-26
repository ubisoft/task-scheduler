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

#include "UnitTest.h"

#include "mg/common/ScratchPad.h"

#include <cstdio>

namespace mg {
namespace unittests {

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

	TestSuiteGuard::TestSuiteGuard(
		const char* aName)
	{
		Report("======== [%s] start", aName);
		myTimer.Start();
	}

	TestSuiteGuard::~TestSuiteGuard()
	{
		Report("======== took %.6lf ms", myTimer.GetMilliSeconds());
	}

	MG_STRFORMAT_PRINTF(2, 3)
	TestCaseGuard::TestCaseGuard(
		const char* aFormat,
		...)
	{
		va_list va;
		va_start(va, aFormat);
		ReportV(aFormat, va);
		va_end(va);
		myTimer.Start();
	}

	TestCaseGuard::~TestCaseGuard()
	{
		Report("took %.6lf ms", myTimer.GetMilliSeconds());
	}

}
}
