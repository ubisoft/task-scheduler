#include "Assert.h"

#include "mg/common/String.h"

#include <cstdio>

namespace mg {
namespace common {

	void
	AssertS(
		const char* aExpression,
		const char* aFile,
		int aLine)
	{
		fprintf(stderr, "assertion failed: (%s) %s %u\n", aExpression, aFile, aLine);
#if IS_PLATFORM_POSIX
		abort();
#else
		int* t = NULL;
		*t = 0;
#endif
	}

	void
	AssertF(
		const char* aExpression,
		const char* aFile,
		int aLine,
		const char* aFormat,
		...)
	{
		mg::common::HybridString<1024> expression;
		expression.Format("(%s): ", aExpression);

		va_list	ap;
		va_start(ap, aFormat);
		expression.AppendVFormat(aFormat, ap);
		va_end(ap);
		AssertS(expression, aFile, aLine);
	}

}
}
