#include "StringFunctions.h"

#include "mg/common/Assert.h"

#include <cctype>
#include <cstdio>

namespace mg {
namespace common {

	uint32_t
	Vsprintf(
		char* aBuffer,
		const char* aFmtString,
		va_list aArgList)
	{
		int rc = vsprintf(aBuffer, aFmtString, aArgList);
		MG_DEV_ASSERT(rc >= 0);
		return (uint32_t)rc;
	}

	uint32_t
	Vsnprintf(
		char* aBuffer,
		uint32_t aBufferSize,
		const char* aFmtString,
		va_list aArgList)
	{
		int rc = vsnprintf(aBuffer, aBufferSize, aFmtString, aArgList);
		MG_DEV_ASSERT(rc >= 0);
		return (uint32_t)rc;
	}

	bool
	StringToNumber(
		const char* aString,
		uint64_t& aOutNumber)
	{
		// Need to check for minus manually, because strtoull()
		// applies negation to the result. Despite the fact it
		// returns an unsigned type. Result is garbage.
		while (isspace(*aString))
			++aString;
		if (*aString == '-')
			return false;
		char* end;
		errno = 0;
		aOutNumber = strtoull(aString, &end, 10);
		return errno == 0 && *aString != 0 && *end == 0;
	}

	bool
	StringToNumber(
		const char* aString,
		uint32_t& aOutNumber)
	{
		uint64_t res;
		if (StringToNumber(aString, res) && res <= UINT32_MAX)
		{
			aOutNumber = (uint32_t)res;
			return true;
		}
		return false;
	}

}
}
