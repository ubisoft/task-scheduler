#pragma once

#include "mg/common/String.h"

namespace mg {
namespace common {

	// Simplified version. In the original code the scratch pad is
	// a thread-local buffer which helps to avoid heap allocations
	// for temporary scoped data of big or unknown size.

	using ScratchBuffer = mg::common::HybridString<256>;

	MG_STRFORMAT_PRINTF(1, 2)
	static inline ScratchBuffer
	ScratchSprintf(
		const char* aFormat,
		...)
	{
		va_list va;
		va_start(va, aFormat);
		ScratchBuffer res;
		res.VFormat(aFormat, va);
		va_end(va);
		return res;
	}

	MG_STRFORMAT_PRINTF(1, 0)
	static inline ScratchBuffer
	ScratchVsprintf(
		const char* aFormat,
		va_list aArg)
	{
		ScratchBuffer res;
		res.VFormat(aFormat, aArg);
		return res;
	}

}
}
