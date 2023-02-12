#include "Assert.h"

#include "mg/common/Util.h"

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
		std::string expression = mg::common::StringFormat(
			"(%s): ", aExpression);

		va_list	ap;
		va_start(ap, aFormat);
		expression += mg::common::StringVFormat(aFormat, ap);
		va_end(ap);
		AssertS(expression.c_str(), aFile, aLine);
	}

}
}
