#include "mg/common/Assert.h"
#include "mg/common/Atomic.h"

#include "UnitTest.h"

#define TEST_CHECK MG_COMMON_ASSERT

namespace mg {
namespace unittests {

	static void
	UnitTestAtomicConstructor()
	{
		// Default.
		{
			mg::common::Atomic<int> value;
			value.Store(123);
			TEST_CHECK(value.Load() == 123);
		}
		// With a value
		{
			mg::common::Atomic<int> value(123);
			TEST_CHECK(value.Load() == 123);
		}
	}

	static void
	UnitTestAtomicLoad()
	{
		mg::common::Atomic<int> value(123);
		TEST_CHECK(value.LoadRelaxed() == 123);
		TEST_CHECK(value.LoadAcquire() == 123);
		TEST_CHECK(value.Load() == 123);
	}

	static void
	UnitTestAtomicStore()
	{
		mg::common::Atomic<int> value(123);
		value.StoreRelaxed(4);
		TEST_CHECK(value.LoadRelaxed() == 4);
		value.StoreRelease(5);
		TEST_CHECK(value.LoadRelaxed() == 5);
		value.Store(6);
		TEST_CHECK(value.LoadRelaxed() == 6);
	}

	static void
	UnitTestAtomicExchange()
	{
		mg::common::Atomic<int> value(123);
		TEST_CHECK(value.ExchangeRelaxed(4) == 123);
		TEST_CHECK(value.ExchangeAcqRel(5) == 4);
		TEST_CHECK(value.ExchangeRelease(6) == 5);
		TEST_CHECK(value.Exchange(7) == 6);
		TEST_CHECK(value.LoadRelaxed() == 7);
	}

	static void
	UnitTestAtomicFetchAdd()
	{
		mg::common::Atomic<int> value(123);
		TEST_CHECK(value.FetchAddRelaxed(2) == 123);
		TEST_CHECK(value.FetchAddAcqRel(3) == 125);
		TEST_CHECK(value.FetchAddRelease(4) == 128);
		TEST_CHECK(value.FetchAdd(5) == 132);
		TEST_CHECK(value.LoadRelaxed() == 137);
	}

	static void
	UnitTestAtomicAddFetch()
	{
		mg::common::Atomic<int> value(123);
		TEST_CHECK(value.AddFetchRelaxed(2) == 125);
		TEST_CHECK(value.AddFetchAcqRel(3) == 128);
		TEST_CHECK(value.AddFetchRelease(4) == 132);
		TEST_CHECK(value.AddFetch(5) == 137);
		TEST_CHECK(value.LoadRelaxed() == 137);
	}

	static void
	UnitTestAtomicAdd()
	{
		mg::common::Atomic<int> value(123);
		value.AddRelaxed(1);
		TEST_CHECK(value.LoadRelaxed() == 124);
		value.AddAcqRel(2);
		TEST_CHECK(value.LoadRelaxed() == 126);
		value.AddRelease(3);
		TEST_CHECK(value.LoadRelaxed() == 129);
		value.Add(4);
		TEST_CHECK(value.LoadRelaxed() == 133);
	}

	static void
	UnitTestAtomicFetchSub()
	{
		mg::common::Atomic<int> value(123);
		TEST_CHECK(value.FetchSubRelaxed(2) == 123);
		TEST_CHECK(value.FetchSubAcqRel(3) == 121);
		TEST_CHECK(value.FetchSubRelease(4) == 118);
		TEST_CHECK(value.FetchSub(5) == 114);
		TEST_CHECK(value.LoadRelaxed() == 109);
	}

	static void
	UnitTestAtomicSubFetch()
	{
		mg::common::Atomic<int> value(123);
		TEST_CHECK(value.SubFetchRelaxed(2) == 121);
		TEST_CHECK(value.SubFetchAcqRel(3) == 118);
		TEST_CHECK(value.SubFetchRelease(4) == 114);
		TEST_CHECK(value.SubFetch(5) == 109);
		TEST_CHECK(value.LoadRelaxed() == 109);
	}

	static void
	UnitTestAtomicSub()
	{
		mg::common::Atomic<int> value(123);
		value.SubRelaxed(1);
		TEST_CHECK(value.LoadRelaxed() == 122);
		value.SubAcqRel(2);
		TEST_CHECK(value.LoadRelaxed() == 120);
		value.SubRelease(3);
		TEST_CHECK(value.LoadRelaxed() == 117);
		value.Sub(4);
		TEST_CHECK(value.LoadRelaxed() == 113);
	}

	static void
	UnitTestAtomicIncrement()
	{
		mg::common::Atomic<int> value(123);
		value.IncrementRelaxed();
		TEST_CHECK(value.LoadRelaxed() == 124);
		value.IncrementAcqRel();
		TEST_CHECK(value.LoadRelaxed() == 125);
		value.IncrementRelease();
		TEST_CHECK(value.LoadRelaxed() == 126);
		value.Increment();;
		TEST_CHECK(value.LoadRelaxed() == 127);
	}

	static void
	UnitTestAtomicFetchIncrement()
	{
		mg::common::Atomic<int> value(123);
		TEST_CHECK(value.FetchIncrementRelaxed() == 123);
		TEST_CHECK(value.FetchIncrementAcqRel() == 124);
		TEST_CHECK(value.FetchIncrementRelease() == 125);
		TEST_CHECK(value.FetchIncrement() == 126);
		TEST_CHECK(value.LoadRelaxed() == 127);
	}

	static void
	UnitTestAtomicIncrementFetch()
	{
		mg::common::Atomic<int> value(123);
		TEST_CHECK(value.IncrementFetchRelaxed() == 124);
		TEST_CHECK(value.IncrementFetchAcqRel() == 125);
		TEST_CHECK(value.IncrementFetchRelease() == 126);
		TEST_CHECK(value.IncrementFetch() == 127);
		TEST_CHECK(value.LoadRelaxed() == 127);
	}

	static void
	UnitTestAtomicDecrement()
	{
		mg::common::Atomic<int> value(123);
		value.DecrementRelaxed();
		TEST_CHECK(value.LoadRelaxed() == 122);
		value.DecrementAcqRel();
		TEST_CHECK(value.LoadRelaxed() == 121);
		value.DecrementRelease();
		TEST_CHECK(value.LoadRelaxed() == 120);
		value.Decrement();;
		TEST_CHECK(value.LoadRelaxed() == 119);
	}

	static void
	UnitTestAtomicFetchDecrement()
	{
		mg::common::Atomic<int> value(123);
		TEST_CHECK(value.FetchDecrementRelaxed() == 123);
		TEST_CHECK(value.FetchDecrementAcqRel() == 122);
		TEST_CHECK(value.FetchDecrementRelease() == 121);
		TEST_CHECK(value.FetchDecrement() == 120);
		TEST_CHECK(value.LoadRelaxed() == 119);
	}

	static void
	UnitTestAtomicDecrementFetch()
	{
		mg::common::Atomic<int> value(123);
		TEST_CHECK(value.DecrementFetchRelaxed() == 122);
		TEST_CHECK(value.DecrementFetchAcqRel() == 121);
		TEST_CHECK(value.DecrementFetchRelease() == 120);
		TEST_CHECK(value.DecrementFetch() == 119);
		TEST_CHECK(value.LoadRelaxed() == 119);
	}

	static void
	UnitTestAtomicCmpExchgWeak()
	{
		mg::common::Atomic<int> value(123);
		int expected = 100;
		TEST_CHECK(!value.CmpExchgWeakAcquire(expected, 124));
		TEST_CHECK(value.LoadRelaxed() == 123);
		TEST_CHECK(expected == 123);

		TEST_CHECK(value.CmpExchgWeakAcquire(expected, 124));
		TEST_CHECK(value.LoadRelaxed() == 124);
		TEST_CHECK(expected == 123);

		value.StoreRelaxed(123);
		expected = 100;
		TEST_CHECK(!value.CmpExchgWeakAcqRel(expected, 124));
		TEST_CHECK(value.LoadRelaxed() == 123);
		TEST_CHECK(expected == 123);

		TEST_CHECK(value.CmpExchgWeakAcqRel(expected, 124));
		TEST_CHECK(value.LoadRelaxed() == 124);
		TEST_CHECK(expected == 123);

		value.StoreRelaxed(123);
		expected = 100;
		TEST_CHECK(!value.CmpExchgWeakRelease(expected, 124));
		TEST_CHECK(value.LoadRelaxed() == 123);
		TEST_CHECK(expected == 123);

		TEST_CHECK(value.CmpExchgWeakRelease(expected, 124));
		TEST_CHECK(value.LoadRelaxed() == 124);
		TEST_CHECK(expected == 123);

		value.StoreRelaxed(123);
		expected = 100;
		TEST_CHECK(!value.CmpExchgWeak(expected, 124));
		TEST_CHECK(value.LoadRelaxed() == 123);
		TEST_CHECK(expected == 123);

		TEST_CHECK(value.CmpExchgWeak(expected, 124));
		TEST_CHECK(value.LoadRelaxed() == 124);
		TEST_CHECK(expected == 123);
	}

	static void
	UnitTestAtomicCmpExchgStrong()
	{
		mg::common::Atomic<int> value(123);
		int expected = 100;
		TEST_CHECK(!value.CmpExchgStrongAcquire(expected, 124));
		TEST_CHECK(value.LoadRelaxed() == 123);
		TEST_CHECK(expected == 123);

		TEST_CHECK(value.CmpExchgStrongAcquire(expected, 124));
		TEST_CHECK(value.LoadRelaxed() == 124);
		TEST_CHECK(expected == 123);

		value.StoreRelaxed(123);
		expected = 100;
		TEST_CHECK(!value.CmpExchgStrongAcqRel(expected, 124));
		TEST_CHECK(value.LoadRelaxed() == 123);
		TEST_CHECK(expected == 123);

		TEST_CHECK(value.CmpExchgStrongAcqRel(expected, 124));
		TEST_CHECK(value.LoadRelaxed() == 124);
		TEST_CHECK(expected == 123);

		value.StoreRelaxed(123);
		expected = 100;
		TEST_CHECK(!value.CmpExchgStrongRelease(expected, 124));
		TEST_CHECK(value.LoadRelaxed() == 123);
		TEST_CHECK(expected == 123);

		TEST_CHECK(value.CmpExchgStrongRelease(expected, 124));
		TEST_CHECK(value.LoadRelaxed() == 124);
		TEST_CHECK(expected == 123);

		value.StoreRelaxed(123);
		expected = 100;
		TEST_CHECK(!value.CmpExchgStrong(expected, 124));
		TEST_CHECK(value.LoadRelaxed() == 123);
		TEST_CHECK(expected == 123);

		TEST_CHECK(value.CmpExchgStrong(expected, 124));
		TEST_CHECK(value.LoadRelaxed() == 124);
		TEST_CHECK(expected == 123);
	}

	static void
	UnitTestAtomicBool()
	{
		mg::common::Atomic<bool> value(true);
		TEST_CHECK(value.LoadRelaxed());

		value.StoreRelaxed(false);
		TEST_CHECK(!value.LoadRelaxed());

		TEST_CHECK(!value.ExchangeRelaxed(true));
		TEST_CHECK(value.LoadRelaxed());

		bool expected = false;
		TEST_CHECK(!value.CmpExchgWeakRelaxed(expected, false));
		TEST_CHECK(expected);
		TEST_CHECK(value.LoadRelaxed());

		TEST_CHECK(value.CmpExchgWeakRelaxed(expected, false));
		TEST_CHECK(expected);
		TEST_CHECK(!value.LoadRelaxed());

		expected = false;
		value.StoreRelaxed(true);
		TEST_CHECK(!value.CmpExchgStrongRelaxed(expected, false));
		TEST_CHECK(expected);
		TEST_CHECK(value.LoadRelaxed());

		TEST_CHECK(value.CmpExchgStrongRelaxed(expected, false));
		TEST_CHECK(expected);
		TEST_CHECK(!value.LoadRelaxed());
	}

	static void
	UnitTestAtomicPtr()
	{
		int i1, i2;
		void* val1 = (void*)&i1;
		void* val2 = (void*)&i2;

		mg::common::Atomic<void*> value(val1);
		TEST_CHECK(value.LoadRelaxed() == val1);

		value.StoreRelaxed(val2);
		TEST_CHECK(value.LoadRelaxed() == val2);

		TEST_CHECK(value.ExchangeRelaxed(val1) == val2);
		TEST_CHECK(value.LoadRelaxed() == val1);

		void* expected = val2;
		TEST_CHECK(!value.CmpExchgWeakRelaxed(expected, val2));
		TEST_CHECK(expected == val1);
		TEST_CHECK(value.LoadRelaxed() == val1);

		TEST_CHECK(value.CmpExchgWeakRelaxed(expected, val2));
		TEST_CHECK(expected == val1);
		TEST_CHECK(value.LoadRelaxed() == val2);

		expected = val2;
		value.StoreRelaxed(val1);
		TEST_CHECK(!value.CmpExchgStrongRelaxed(expected, val2));
		TEST_CHECK(expected == val1);
		TEST_CHECK(value.LoadRelaxed() == val1);

		TEST_CHECK(value.CmpExchgStrongRelaxed(expected, val2));
		TEST_CHECK(expected == val1);
		TEST_CHECK(value.LoadRelaxed() == val2);
	}

	void
	UnitTestAtomic()
	{
		TestSuiteGuard suite("Atomic");

		UnitTestAtomicConstructor();
		UnitTestAtomicLoad();
		UnitTestAtomicStore();
		UnitTestAtomicExchange();
		UnitTestAtomicFetchAdd();
		UnitTestAtomicAddFetch();
		UnitTestAtomicAdd();
		UnitTestAtomicFetchSub();
		UnitTestAtomicSubFetch();
		UnitTestAtomicSub();
		UnitTestAtomicIncrement();
		UnitTestAtomicFetchIncrement();
		UnitTestAtomicIncrementFetch();
		UnitTestAtomicDecrement();
		UnitTestAtomicFetchDecrement();
		UnitTestAtomicDecrementFetch();
		UnitTestAtomicCmpExchgWeak();
		UnitTestAtomicCmpExchgStrong();
		UnitTestAtomicBool();
		UnitTestAtomicPtr();
	}

}
}
