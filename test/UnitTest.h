#pragma once

#include "mg/common/QPTimer.h"
#include "mg/common/Assert.h"

#undef MG_COMMON_ASSERT
#undef MG_COMMON_ASSERT_F

#define MG_COMMON_ASSERT_F(X, ...) do {														\
	if (MG_TEST(!(X)))																		\
		mg::common::AssertF(#X, __FILE__, __LINE__, __VA_ARGS__);							\
} while(false)

#define MG_COMMON_ASSERT(X) do {															\
	if (MG_TEST(!(X)))																		\
		mg::common::AssertS(#X, __FILE__, __LINE__);										\
} while(false)

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
