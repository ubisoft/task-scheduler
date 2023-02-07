#include "mg/common/Atomic.h"
#include "mg/common/HybridArray.h"

#include "UnitTest.h"

namespace mg {
namespace unittests {

	static void
	UnitTestAtomicIncrement()
	{
		int32 vi32 = 5;
		MG_COMMON_ASSERT(mg::common::AtomicIncrement(&vi32) == 6);
		MG_COMMON_ASSERT(vi32 == 6);

		int64 vi64 = (uint64) UINT32_MAX;
		MG_COMMON_ASSERT(mg::common::AtomicIncrement64(&vi64) == (int64) UINT32_MAX + 1);
		MG_COMMON_ASSERT(vi64 == (int64) UINT32_MAX + 1);
	}

	static void
	UnitTestAtomicDecrement()
	{
		int32 vi32 = 5;
		MG_COMMON_ASSERT(mg::common::AtomicDecrement(&vi32) == 4);
		MG_COMMON_ASSERT(vi32 == 4);
	}

	static void
	UnitTestAtomicExchange()
	{
		int32 vi32 = 5;
		MG_COMMON_ASSERT(mg::common::AtomicExchange(&vi32, 10) == 5);
		MG_COMMON_ASSERT(vi32 == 10);

		uint32 vu32 = 5;
		MG_COMMON_ASSERT(mg::common::AtomicExchangeU32(&vu32, 10) == 5);
		MG_COMMON_ASSERT(vu32 == 10);

		int64 vi64 = (int64) UINT32_MAX + 1;
		MG_COMMON_ASSERT(mg::common::AtomicExchange64(&vi64, (int64) UINT32_MAX + 10) ==
			(int64) UINT32_MAX + 1);
		MG_COMMON_ASSERT(vi64 == (int64) UINT32_MAX + 10);

		uint64 vu64 = (uint64) UINT32_MAX + 1;
		MG_COMMON_ASSERT(mg::common::AtomicExchangeU64(&vu64, (uint64) UINT32_MAX + 10) ==
			(uint64) UINT32_MAX + 1);
		MG_COMMON_ASSERT(vu64 == (uint64) UINT32_MAX + 10);

		void* vptr = &vu64;
		MG_COMMON_ASSERT(mg::common::AtomicExchangePtr(&vptr, (void*) &vi64) ==
			(void*) &vu64);
		MG_COMMON_ASSERT(vptr == (void*) &vi64);
	}

	static void
	UnitTestAtomicCompareExchange()
	{
		int32 vi32 = 5;
		MG_COMMON_ASSERT(mg::common::AtomicCompareExchange(&vi32, 10, 6) == 5);
		MG_COMMON_ASSERT(vi32 == 5);
		MG_COMMON_ASSERT(mg::common::AtomicCompareExchange(&vi32, 10, 5) == 5);
		MG_COMMON_ASSERT(vi32 == 10);

		int64 vi64 = (int64)UINT32_MAX + 1;
		MG_COMMON_ASSERT(mg::common::AtomicCompareExchange64(&vi64, (int64)UINT32_MAX + 10,
			(int64)UINT32_MAX + 5) == (int64)UINT32_MAX + 1);
		MG_COMMON_ASSERT(vi64 == (int64)UINT32_MAX + 1);
		MG_COMMON_ASSERT(mg::common::AtomicCompareExchange64(&vi64, (int64)UINT32_MAX + 10,
			(int64)UINT32_MAX + 1) == (int64)UINT32_MAX + 1);
		MG_COMMON_ASSERT(vi64 == (int64)UINT32_MAX + 10);

		int32 dummyVal;
		void* vptr = &vi64;
		MG_COMMON_ASSERT(mg::common::AtomicCompareExchangePtr(&vptr, (void*) &vi32,
			(void*) &dummyVal) == (void*)&vi64);
		MG_COMMON_ASSERT(vptr == (void*) &vi64);
		MG_COMMON_ASSERT(mg::common::AtomicCompareExchangePtr(&vptr, (void*) &vi32,
			(void*) &vi64) == (void*) &vi64);
		MG_COMMON_ASSERT(vptr == (void*) &vi32);
	}

	static void
	UnitTestAtomicAdd()
	{
		int32 vi32 = 1;
		MG_COMMON_ASSERT(mg::common::AtomicAdd(&vi32, 2) == 1);
		MG_COMMON_ASSERT(vi32 == 3);

		uint32 vu32 = 1;
		MG_COMMON_ASSERT(mg::common::AtomicAddU32(&vu32, 2) == 1);
		MG_COMMON_ASSERT(vu32 == 3);

		int64 vi64 = (int64)UINT32_MAX + 1;
		MG_COMMON_ASSERT(mg::common::AtomicAdd64(&vi64, 2) == (int64)UINT32_MAX + 1);
		MG_COMMON_ASSERT(vi64 == (int64) UINT32_MAX + 3);
	}

	static void
	UnitTestAtomicFlag()
	{
		int32 flag = 0;
		MG_COMMON_ASSERT(mg::common::AtomicFlagTest(&flag) == 0);
		MG_COMMON_ASSERT(mg::common::AtomicFlagClear(&flag) == 0);
		MG_COMMON_ASSERT(flag == 0);

		MG_COMMON_ASSERT(mg::common::AtomicFlagSet(&flag) == 0);
		MG_COMMON_ASSERT(flag == 1);
		MG_COMMON_ASSERT(mg::common::AtomicFlagTest(&flag) == 1);
		MG_COMMON_ASSERT(mg::common::AtomicFlagSet(&flag) == 1);
		MG_COMMON_ASSERT(flag == 1);

		MG_COMMON_ASSERT(mg::common::AtomicFlagClear(&flag) == 1);
		MG_COMMON_ASSERT(flag == 0);
	}

	static void
	UnitTestAtomicLoad()
	{
		int32 vi32 = 100;
		MG_COMMON_ASSERT(mg::common::AtomicLoad(&vi32) == 100);
		uint32 vu32 = 200;
		MG_COMMON_ASSERT(mg::common::AtomicLoadU32(&vu32) == 200);
		int64 vi64 = 300;
		MG_COMMON_ASSERT(mg::common::AtomicLoad64(&vi64) == 300);
		uint64 vu64 = 400;
		MG_COMMON_ASSERT(mg::common::AtomicLoadU64(&vu64) == 400);
		void* vptr = &vu64;
		MG_COMMON_ASSERT(mg::common::AtomicLoadPtr(&vptr) == (void*) &vu64);
	}

	void
	UnitTestAtomic()
	{
		TestSuiteGuard suite("Atomic");

		UnitTestAtomicIncrement();
		UnitTestAtomicDecrement();
		UnitTestAtomicExchange();
		UnitTestAtomicCompareExchange();
		UnitTestAtomicAdd();
		UnitTestAtomicFlag();
		UnitTestAtomicLoad();
	}

}
}
