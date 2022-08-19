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

#pragma once

#include "mg/common/Types.h"

namespace mg {
namespace common {

	static_assert(sizeof(LONG) == 4, "long size");
	static_assert(sizeof(LONGLONG) == 8, "longlong size");

	static inline int32
	AtomicIncrement(
		int32* aValue)
	{
		return InterlockedIncrement((LONG*) aValue);
	}

	static inline int32
	AtomicDecrement(
		int32* aValue)
	{
		return InterlockedDecrement((LONG*) aValue);
	}

	static inline int32
	AtomicCompareExchange(
		int32* aDest,
		int32 aExchange,
		int32 aCompare)
	{
		return InterlockedCompareExchange((LONG*) aDest, (LONG) aExchange,
			(LONG) aCompare);
	}

	static inline int32
	AtomicExchange(
		int32* aDest,
		int32 aValue)
	{
		return InterlockedExchange((LONG*) aDest, (LONG) aValue);
	}

	static inline uint32
	AtomicExchangeU32(
		uint32* aDest,
		uint32 aValue)
	{
		return InterlockedExchange((LONG*) aDest, (LONG) aValue);
	}

	static inline int64
	AtomicExchange64(
		int64* aDest,
		int64 aValue)
	{
		return InterlockedExchange64((LONG64*) aDest, aValue);
	}

	static inline uint64
	AtomicExchangeU64(
		uint64* aDest,
		uint64 aValue)
	{
		union { int64 i64; uint64 u64; };
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

	static inline int64
	AtomicCompareExchange64(
		int64* aDest,
		int64 aExchange,
		int64 aCompare)
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

	static inline int64
	AtomicIncrement64(
		int64* aValue)
	{
		return InterlockedIncrement64(aValue);
	}

	static inline int64
	AtomicDecrement64(
		int64* aValue)
	{
		return InterlockedDecrement64(aValue);
	}

	static inline int32
	AtomicAdd(
		int32* aDest,
		int32 aValue)
	{
		return InterlockedExchangeAdd((LONG*) aDest, aValue);
	}

	static inline uint32
	AtomicAddU32(
		uint32* aDest,
		uint32 aValue)
	{
		return InterlockedExchangeAdd((LONG*) aDest, (LONG)aValue);
	}

	static inline int64
	AtomicAdd64(
		int64* aDest,
		int64 aValue)
	{
		return InterlockedExchangeAdd64((LONG64*) aDest, aValue);
	}

	// See a big comment on how to use these functions in Posix
	// version of this.
	static inline int32
	AtomicFlagTest(
		const int32* aDest)
	{
		return InterlockedCompareExchangeAcquire((LONG*) aDest, 1, 1);
	}

	static inline int32
	AtomicFlagClear(
		int32* aDest)
	{
		// Msvc does not provide a command to emit combined
		// Acquire+Release barrier. So a full one is used here
		// and in the flag setter below.
		return InterlockedExchange((LONG*) aDest, 0);
	}

	static inline int32
	AtomicFlagSet(
		int32* aDest)
	{
		return InterlockedCompareExchange((LONG*) aDest, 1, 0);
	}

	static inline int32
	AtomicLoad(
		int32* aSrc)
	{
		return InterlockedOr((LONG*) aSrc, 0);
	}

	static inline uint32
	AtomicLoadU32(
		uint32* aSrc)
	{
		return InterlockedOr((LONG*) aSrc, 0);
	}

	static inline int64
	AtomicLoad64(
		int64* aSrc)
	{
		return InterlockedOr64(aSrc, 0);
	}

	static inline uint64
	AtomicLoadU64(
		uint64* aSrc)
	{
		union { int64 i64; uint64 u64; };
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
