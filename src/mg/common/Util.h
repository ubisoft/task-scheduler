#pragma once

#include "mg/common/Types.h"

#if !defined(_DEBUG) || IS_ORBIS_PLATFORM
#define MG_TEST(X) (mg::common::PassThrough(X))
#else
#define MG_TEST(X) ((&mg::common::glob_TrueBool) && (X))
#endif

enum
{
	// 16 alignment is the biggest alignment which satisfies all
	// needs. It is ok for stack-related operations (need 16
	// alignment), it is ok for SIMD instructions (need 16), ok
	// for any class and scalar types (need at most 8).
	MG_COMMON_ALIGN_DEFAULT = 16,
};

namespace mg {
namespace common {

	extern const bool glob_TrueBool;

	template<typename T>
	inline T
	Min(const T& aFirst, const T& aSecond)
	{
		return (aFirst < aSecond ? aFirst : aSecond);
	}

	template<typename T>
	inline T
	Max(const T& aFirst, const T& aSecond)
	{
		return (aFirst > aSecond ? aFirst : aSecond);
	}

	static inline bool
	PassThrough(
		bool aValue)
	{
		return aValue;
	}

	// Memory alignment is an important optimization. Consider the
	// following case: the CPU is x64_86 arch, therefore it
	// actively uses 8 byte registers and instructions. Assume
	// there is a pointer at a variable, not aligned by 8 bytes.
	// For example something like 8x + 4, where 'x' is some
	// number. And assume it is a uint64_t number. To read these 8
	// bytes CPU will need to do multiple fetches from memory,
	// because typically it works in words of a fixed size aligned
	// by a certain value. And then merge them into one register.
	// To write to this address CPU may need two operations with 2
	// registers to upload the new data. This is basically *at
	// least* x2 slowdown. On some architectures memory alignment
	// is obligatory and its violation may lead to a crash.
	// Also alignment is a kind of atomicity guarantee - the CPU
	// is able to read/write up to 8 bytes atomically if they are
	// aligned.
	MG_FORCEINLINE size_t
	Align(
		size_t aValue,
		int aAlignment)
	{
		return (aValue + aAlignment - 1) & ~(aAlignment - 1);
	}

}
}
