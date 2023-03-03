#pragma once

#include "mg/common/Types.h"

namespace mg {
namespace common {

	// __atomic_* functions are used instead of __sync_* because
	// they allow to explicitly set a memory model. Most of
	// __sync_* operations provide Sequential Consistency by
	// default, except __sync_lock_test_and_set. This function is
	// supposed to be used in pair with __sync_lock_release to
	// implement a simple lock (for example, a spinlock). And in
	// fact it is really faster than two full memory barriers on
	// both acquire and release of a lock. But
	//
	// 1) Acquire-Release does not protect from a case when
	//    there are more than 2 threads and more than 2 variables
	//    getting updated. In other words - multiple consumers
	//    and multiple producers case. A full memory barrier is
	//    inevitable here.
	//
	// 2) Windows' version uses a full memory barrier-ed function
	//    InterlockedExchange. Linux implementation should be
	//    consistent.
	//
	// Note, that it is impossible to write a test for acquire/
	// release memory models on x86 which would fail. Because x86
	// on hardware level provides acquire for all loads and
	// release for all stores.

	static inline int32_t
	AtomicIncrement(
		int32_t* aValue)
	{
		return __atomic_add_fetch(aValue, 1, __ATOMIC_SEQ_CST);
	}

	static inline int64_t
	AtomicIncrement64(
		int64_t* aValue)
	{
		return __atomic_add_fetch(aValue, 1, __ATOMIC_SEQ_CST);
	}

	static inline int32_t
	AtomicDecrement(
		int32_t* aValue)
	{
		return __atomic_sub_fetch(aValue, 1, __ATOMIC_SEQ_CST);
	}

	static inline int32_t
	AtomicExchange(
		int32_t* aDest,
		int32_t aExchange)
	{
		return __atomic_exchange_n(aDest, aExchange, __ATOMIC_SEQ_CST);
	}

	static inline uint32_t
	AtomicExchangeU32(
		uint32_t* aDest,
		uint32_t aExchange)
	{
		return __atomic_exchange_n(aDest, aExchange, __ATOMIC_SEQ_CST);
	}

	static inline int64_t
	AtomicExchange64(
		int64_t* aDest,
		int64_t aExchange)
	{
		return __atomic_exchange_n(aDest, aExchange, __ATOMIC_SEQ_CST);
	}

	static inline uint64_t
	AtomicExchangeU64(
		uint64_t* aDest,
		uint64_t aExchange)
	{
		return __atomic_exchange_n(aDest, aExchange, __ATOMIC_SEQ_CST);
	}

	template<typename T>
	static inline T*
	AtomicExchangePtr(
		T** aDest,
		T* aExchange)
	{
		return __atomic_exchange_n(aDest, aExchange, __ATOMIC_SEQ_CST);
	}

	static inline int32_t
	AtomicCompareExchange(
		int32_t* aDest,
		int32_t aExchange,
		int32_t aCompare)
	{
		__atomic_compare_exchange(aDest, &aCompare, &aExchange, false, __ATOMIC_SEQ_CST,
			__ATOMIC_SEQ_CST);
		// Need to return the old value of aDest.
		// In case aDest == aCompare the result is correct. In
		// case aDest was != aCompare the aDest is copied to
		// aCompare, so it is legal to return aCompare in any
		// case.
		return aCompare;
	}

	static inline int64_t
	AtomicCompareExchange64(
		int64_t* aDest,
		int64_t aExchange,
		int64_t aCompare)
	{
		__atomic_compare_exchange(aDest, &aCompare, &aExchange, false, __ATOMIC_SEQ_CST,
			__ATOMIC_SEQ_CST);
		return aCompare;
	}

	template<typename T>
	static inline T*
	AtomicCompareExchangePtr(
		T** aDest,
		T* aExchange,
		T* aCompare)
	{
		__atomic_compare_exchange(aDest, &aCompare, &aExchange, false, __ATOMIC_SEQ_CST,
			__ATOMIC_SEQ_CST);
		return aCompare;
	}

	static inline int32_t
	AtomicAdd(
		int32_t* aDest,
		int32_t aValue)
	{
		return __atomic_fetch_add(aDest, aValue, __ATOMIC_SEQ_CST);
	}

	static inline uint32_t
	AtomicAddU32(
		uint32_t* aDest,
		uint32_t aValue)
	{
		return __atomic_fetch_add(aDest, aValue, __ATOMIC_SEQ_CST);
	}

	static inline int64_t
	AtomicAdd64(
		int64_t* aDest,
		int64_t aValue)
	{
		return __atomic_fetch_add(aDest, aValue, __ATOMIC_SEQ_CST);
	}

	// The following three functions can be used to create a flag
	// shared by two threads to signal about a completed action.
	// These functions being used correctly guarantee, that if one
	// thread does some stores and sets the flag, then other
	// thread will see all the stores not earlier than the flag.
	// Example:
	//
	//         A = 0;
	//
	//     Thread1:
	//         A = 1;
	//         AtomicFlagSet(&flag);
	//
	//     Thread2:
	//         if (AtomicFlagTest(&flag))
	//             B = A;
	//
	// It is guaranteed, that B will be 1.
	// Note, that these functions doesn't guarantee anything, if
	// there are more than 2 threads and more than 2 flags. In
	// such a case full memory barrier-ed functions should be
	// used. A widely known example, where Acquire-Release does
	// not help:
	//
	//     x = false;
	//     y = false;
	//     z = 0;
	//
	//     Thread1:
	//         AtomicFlagSet(&x);
	//     Thread2:
	//         AtomicFlagSet(&y);
	//
	//     Thread3:
	//         while (!AtomicTest(&x));
	//         if (AtomicTest(&y))
	//             AtomicIncrement(&z);
	//     Thread4:
	//          while (!AtomicTest(&y));
	//          if (AtomicTest(&x))
	//              AtomicIncrement(&z);
	//
	// In the end 'z' can stay 0. To avoid that problem in a
	// similar case a full barrier is the only solution.
	//
	// Why not to use a full barrier always? Because on x86
	// Acquire and Release are nop, that is almost free. The only
	// thing these functions do is prevent compiler-side
	// reorderings. For a full barrier x86 uses a special
	// operation 'mfence', which is quite heavy - all the writes
	// are propagated to cache of all other cores. Even writes,
	// not related to the accessed memory.
	//
	static inline int32_t
	AtomicFlagTest(
		const int32_t* aDest)
	{
		// Acquire means that all loads and stores following this
		// instruction won't be reordered before.
		return __atomic_load_n(aDest, __ATOMIC_ACQUIRE);
	}

	static inline int32_t
	AtomicFlagClear(
		int32_t* aDest)
	{
		// Both Acquire and Release are needed, because usually
		// when a flag is explicitly cleared, it is going to be
		// used again. And a thread, which is going to set it
		// back, should see all the changes before the clear
		// without explicitly calling Acquire. It would look
		// ridiculous. So this thread does the job.
		return __atomic_exchange_n(aDest, 0, __ATOMIC_ACQ_REL);
	}

	static inline int32_t
	AtomicFlagSet(
		int32_t* aDest)
	{
		// Why both Acquire and Release here? Why not only
		// Release. Because there are two usage cases for Set():
		//
		//     - The flag is used as a spinlock. Then code will
		//     try to lock it like this:
		//
		//         while (AtomicFlagSet(&flag) != 0) { };
		//
		//     Set() here is used to both read and write the
		//     flag. For read it needs Acquire, for write -
		//     Release.
		//
		//     - The flag is used as a signal. Then only one
		//     thread does Set(), and other threads do Test().
		//     In that case Release would be enough for Set().
		//     But anyway, the spinlock case requires both.
		//
		return __atomic_exchange_n(aDest, 1, __ATOMIC_ACQ_REL);
	}

	static inline int32_t
	AtomicLoad(
		int32_t* aSrc)
	{
		return __atomic_load_n(aSrc, __ATOMIC_SEQ_CST);
	}

	static inline uint32_t
	AtomicLoadU32(
		uint32_t* aSrc)
	{
		return __atomic_load_n(aSrc, __ATOMIC_SEQ_CST);
	}

	static inline int64_t
	AtomicLoad64(
		int64_t* aSrc)
	{
		return __atomic_load_n(aSrc, __ATOMIC_SEQ_CST);
	}

	static inline uint64_t
	AtomicLoadU64(
		uint64_t* aSrc)
	{
		return __atomic_load_n(aSrc, __ATOMIC_SEQ_CST);
	}

	template<typename T>
	static inline T*
	AtomicLoadPtr(
		T** aSrc)
	{
		return __atomic_load_n(aSrc, __ATOMIC_SEQ_CST);
	}

	static inline void
	CompilerBarrier()
	{
		asm volatile("" ::: "memory");
	}

}
}
