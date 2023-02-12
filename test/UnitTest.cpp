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
		vprintf(aFormat, aArg);
		printf("\n");
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
