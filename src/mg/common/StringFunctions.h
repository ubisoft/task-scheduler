#pragma once

#include "mg/common/Types.h"

#include <stdarg.h>
#include <string.h>

namespace mg {
namespace common {

	static inline uint32
	Strlen(
		const char* aString)
	{
		return (uint32) strlen(aString);
	}

	static inline int
	Strcmp(
		const char* aA,
		const char* aB)
	{
		return strcmp(aA, aB);
	}

	static inline int
	Strncmp(
		const char* aA,
		const char* aB,
		uint32 aCount)
	{
		return strncmp(aA, aB, aCount);
	}

	MG_STRFORMAT_PRINTF(2, 0)
	uint32 Vsprintf(
		char* aBuffer,
		const char* aFmtString,
		va_list aArgList);

	MG_STRFORMAT_PRINTF(3, 0)
	uint32 Vsnprintf(
		char* aBuffer,
		uint32 aBufferSize,
		const char* aFmtString,
		va_list aArgList);

	bool StringToNumber(
		const char* aString,
		uint32& aOutNumber);

	bool StringToNumber(
		const char* aString,
		uint64& aOutNumber);

}
}
