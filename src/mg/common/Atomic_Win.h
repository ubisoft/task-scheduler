#pragma once

#include "mg/common/Types.h"

namespace mg {
namespace common {

	static_assert(sizeof(LONG) == 4, "long size");
	static_assert(sizeof(LONGLONG) == 8, "longlong size");

	static inline int32_t
	AtomicIncrement(
		int32_t* aValue)
	{
		return InterlockedIncrement((LONG*) aValue);
	}

	static inline int32_t
	AtomicDecrement(
		int32_t* aValue)
	{
		return InterlockedDecrement((LONG*) aValue);
	}

	static inline int32_t
	AtomicCompareExchange(
		int32_t* aDest,
		int32_t aExchange,
		int32_t aCompare)
	{
		return InterlockedCompareExchange((LONG*) aDest, (LONG) aExchange,
			(LONG) aCompare);
	}

	static inline int32_t
	AtomicExchange(
		int32_t* aDest,
		int32_t aValue)
	{
		return InterlockedExchange((LONG*) aDest, (LONG) aValue);
	}

	static inline uint32_t
	AtomicExchangeU32(
		uint32_t* aDest,
		uint32_t aValue)
	{
		return InterlockedExchange((LONG*) aDest, (LONG) aValue);
	}

	static inline int64_t
	AtomicExchange64(
		int64_t* aDest,
		int64_t aValue)
	{
		return InterlockedExchange64((LONG64*) aDest, aValue);
	}

	static inline uint64_t
	AtomicExchangeU64(
		uint64_t* aDest,
		uint64_t aValue)
	{
		union { int64_t i64; uint64_t u64; };
		u64 = aValue;
		i64 = InterlockedExchange64((LONG64*) aDest, i64);
		return u64;
	}

	template<typename T>
	static inline T*
	AtomicExchangePtr(
		T** aDest,
		T* aExchange)
	{
		return (T*)InterlockedExchangePointer((void**)aDest, (void*)aExchange);
	}

	static inline int64_t
	AtomicCompareExchange64(
		int64_t* aDest,
		int64_t aExchange,
		int64_t aCompare)
	{
		return InterlockedCompareExchange64(aDest, aExchange, aCompare);
	}

	template<typename T>
	static inline T*
	AtomicCompareExchangePtr(
		T** aDest,
		T* aExchange,
		T* aCompare)
	{
		return (T*)InterlockedCompareExchangePointer((void**) aDest, (void*) aExchange,
			(void*) aCompare);
	}

	static inline int64_t
	AtomicIncrement64(
		int64_t* aValue)
	{
		return InterlockedIncrement64(aValue);
	}

	static inline int32_t
	AtomicAdd(
		int32_t* aDest,
		int32_t aValue)
	{
		return InterlockedExchangeAdd((LONG*) aDest, aValue);
	}

	static inline uint32_t
	AtomicAddU32(
		uint32_t* aDest,
		uint32_t aValue)
	{
		return InterlockedExchangeAdd((LONG*) aDest, (LONG)aValue);
	}

	static inline int64_t
	AtomicAdd64(
		int64_t* aDest,
		int64_t aValue)
	{
		return InterlockedExchangeAdd64((LONG64*) aDest, aValue);
	}

	// See a big comment on how to use these functions in Posix
	// version of this.
	static inline int32_t
	AtomicFlagTest(
		const int32_t* aDest)
	{
		return InterlockedCompareExchangeAcquire((LONG*) aDest, 1, 1);
	}

	static inline int32_t
	AtomicFlagClear(
		int32_t* aDest)
	{
		// Msvc does not provide a command to emit combined
		// Acquire+Release barrier. So a full one is used here
		// and in the flag setter below.
		return InterlockedExchange((LONG*) aDest, 0);
	}

	static inline int32_t
	AtomicFlagSet(
		int32_t* aDest)
	{
		return InterlockedCompareExchange((LONG*) aDest, 1, 0);
	}

	static inline int32_t
	AtomicLoad(
		int32_t* aSrc)
	{
		return InterlockedOr((LONG*) aSrc, 0);
	}

	static inline uint32_t
	AtomicLoadU32(
		uint32_t* aSrc)
	{
		return InterlockedOr((LONG*) aSrc, 0);
	}

	static inline int64_t
	AtomicLoad64(
		int64_t* aSrc)
	{
		return InterlockedOr64(aSrc, 0);
	}

	static inline uint64_t
	AtomicLoadU64(
		uint64_t* aSrc)
	{
		union { int64_t i64; uint64_t u64; };
		i64 = InterlockedOr64((LONG64*) aSrc, 0);
		return u64;
	}

	template<typename T>
	static inline T*
	AtomicLoadPtr(
		T** aSrc)
	{
		return (T*)InterlockedCompareExchangePointer((void**)aSrc, nullptr, nullptr);
	}

	static inline void
	CompilerBarrier()
	{
		_ReadWriteBarrier();
	}

}
}
