#include "mg/common/Callback.h"
#include "mg/common/HybridArray.h"

#include "UnitTest.h"

#define UTAVALUE_MAGIC 0xdeadbeef

namespace mg {
namespace unittests {

	struct UTAValue
	{
		inline
		UTAValue(
			int aValue)
			: myValue(aValue)
			, myPtr(this)
			, myMagic(UTAVALUE_MAGIC)
		{
			++ourConstrCount;
		}

		inline
		UTAValue()
			: myValue(-1)
			, myPtr(this)
			, myMagic(UTAVALUE_MAGIC)
		{
			++ourConstrCount;
		}

		inline
		~UTAValue()
		{
			// Ptr helps to ensure there were no memcpys called on
			// the value. It was properly constructed.
			MG_COMMON_ASSERT(myPtr == this);
			// Magic is a double-check for the same.
			MG_COMMON_ASSERT(myMagic == UTAVALUE_MAGIC);
			++ourDestrCount;
			myValue = -2;
			myMagic = 0;
		}

		inline
		UTAValue(
			const UTAValue& aSrc)
			: myPtr(this)
			, myMagic(UTAVALUE_MAGIC)
		{
			myValue = aSrc.myValue;
			++ourCopyConstrCount;
		}

		inline
		UTAValue(
			UTAValue&& aSrc)
			: myPtr(this)
			, myMagic(UTAVALUE_MAGIC)
		{
			myValue = aSrc.myValue;
			++ourMoveConstrCount;
		}

		inline UTAValue&
		operator=(
			const UTAValue& aSrc)
		{
			myValue = aSrc.myValue;
			++ourCopyAssignCount;
			return *this;
		}

		inline UTAValue&
		operator=(
			UTAValue&& aSrc)
		{
			myValue = aSrc.myValue;
			++ourMoveAssignCount;
			return *this;
		}

		inline bool
		operator<(
			const UTAValue& aValue) const
		{
			return myValue < aValue.myValue;
		}

		inline bool
		operator>(
			const UTAValue& aValue) const
		{
			return myValue > aValue.myValue;
		}

		inline bool
		operator==(
			const UTAValue& aOther) const
		{
			return myValue == aOther.myValue;
		}

		inline void
		operator delete(
			void* aPtr)
		{
			++ourDelCount;
			::operator delete(aPtr);
		}

		static void
		ResetCounters()
		{
			ourCopyConstrCount = 0;
			ourCopyAssignCount = 0;
			ourMoveConstrCount = 0;
			ourMoveAssignCount = 0;
			ourConstrCount = 0;
			ourDestrCount = 0;
			ourDelCount = 0;
		}

		static void
		CheckCounters()
		{
			MG_COMMON_ASSERT(
				ourCopyConstrCount == 0 && ourCopyAssignCount == 0 &&
				ourMoveConstrCount == 0 && ourMoveAssignCount == 0 &&
				ourConstrCount == 0 && ourDestrCount == 0 && ourDelCount == 0
			);
		}

		static inline void
		UseCopyConstrCount(
			int aToUse)
		{
			ourCopyConstrCount -= aToUse;
			MG_COMMON_ASSERT(ourCopyConstrCount >= 0);
		}

		static inline void
		UseCopyAssignCount(
			int aToUse)
		{
			ourCopyAssignCount -= aToUse;
			MG_COMMON_ASSERT(ourCopyAssignCount >= 0);
		}

		static inline void
		UseMoveConstrCount(
			int aToUse)
		{
			ourMoveConstrCount -= aToUse;
			MG_COMMON_ASSERT(ourMoveConstrCount >= 0);
		}

		static inline void
		UseMoveAssignCount(
			int aToUse)
		{
			ourMoveAssignCount -= aToUse;
			MG_COMMON_ASSERT(ourMoveAssignCount >= 0);
		}

		static inline void
		UseConstrCount(
			int aToUse)
		{
			ourConstrCount -= aToUse;
			MG_COMMON_ASSERT(ourConstrCount >= 0);
		}

		static inline void
		UseDestrCount(
			int aToUse)
		{
			ourDestrCount -= aToUse;
			MG_COMMON_ASSERT(ourDestrCount >= 0);
		}

		static inline void
		UseDelCount(
			int aToUse)
		{
			ourDelCount -= aToUse;
			MG_COMMON_ASSERT(ourDelCount >= 0);
		}

		int myValue;
		UTAValue* myPtr;
		uint32_t myMagic;

		static int ourCopyConstrCount;
		static int ourCopyAssignCount;
		static int ourMoveConstrCount;
		static int ourMoveAssignCount;
		static int ourConstrCount;
		static int ourDestrCount;
		static int ourDelCount;
	};

	int UTAValue::ourCopyConstrCount = 0;
	int UTAValue::ourCopyAssignCount = 0;
	int UTAValue::ourMoveConstrCount = 0;
	int UTAValue::ourMoveAssignCount = 0;
	int UTAValue::ourConstrCount = 0;
	int UTAValue::ourDestrCount = 0;
	int UTAValue::ourDelCount = 0;

	struct UTAValueReverseCmp
	{
		inline bool
		operator()(
			const UTAValue& aA,
			const UTAValue& aB)
		{
			// Non-reversed would use '<'.
			return aA > aB;
		}
	};

	using UTAArray = mg::common::Array<UTAValue>;
	using UTAHArray1 = mg::common::HybridArray<UTAValue, 1>;
	using UTAHArray2 = mg::common::HybridArray<UTAValue, 2>;
	using UTAArrayCreateCb = mg::common::Callback<UTAArray*(void)>;
	using UTAArrayVariants = mg::common::Array<UTAArrayCreateCb>;

	// An array of functions which can construct arrays of various
	// types and sizes. It is helpful when want to try all
	// possible combinations of array couples for swap,
	// assignment, etc.
	// The arrays are created as new because usually are expected
	// to be changed after creation. This helps to isolate the
	// test cases from each other.
	//
	static UTAArrayVariants*
	UnitTestArrayCreateVariants()
	{
		mg::common::HybridArray<UTAArrayCreateCb, 100>* combs =
			new mg::common::HybridArray<UTAArrayCreateCb, 100>();

		// Dynamic, cap 0, size 0.
		combs->Add({[]() {
			return new UTAArray();
		}});
		// Dynamic, cap 1, size 0.
		combs->Add({[]() {
			UTAArray* a = new UTAArray();
			a->Reserve(1);
			return a;
		}});
		// Dynamic, cap 1, size 1.
		combs->Add({[]() {
			UTAArray* a = new UTAArray();
			a->Add(UTAValue(0));
			return a;
		}});
		// Dynamic, cap 2, size 0.
		combs->Add({[]() {
			UTAArray* a = new UTAArray();
			a->Reserve(2);
			return a;
		}});
		// Dynamic, cap 2, size 1.
		combs->Add({[]() {
			UTAArray* a = new UTAArray();
			a->Reserve(2);
			a->Add(UTAValue(0));
			return a;
		}});
		// Dynamic, cap 2, size 2.
		combs->Add({[]() {
			UTAArray* a = new UTAArray();
			a->Reserve(2);
			a->Add(UTAValue(0));
			a->Add(UTAValue(1));
			return a;
		}});
		// Hybrid, tail 1, cap 1, size 0.
		combs->Add({[]() {
			return (UTAArray*) (new UTAHArray1());
		}});
		// Hybrid, tail 1, cap 1, size 1.
		combs->Add({[]() {
			UTAArray* a = new UTAHArray1();
			a->Add(UTAValue(0));
			return a;
		}});
		// Hybrid, tail 1, cap 2, size 0.
		combs->Add({[]() {
			UTAArray* a = new UTAHArray1();
			a->Reserve(2);
			return a;
		}});
		// Hybrid, tail 1, cap 2, size 1.
		combs->Add({[]() {
			UTAArray* a = new UTAHArray1();
			a->Reserve(2);
			a->Add(UTAValue(0));
			return a;
		}});
		// Hybrid, tail 1, cap 2, size 2.
		combs->Add({[]() {
			UTAArray* a = new UTAHArray1();
			a->Reserve(2);
			a->Add(UTAValue(0));
			a->Add(UTAValue(1));
			return a;
		}});
		// Hybrid, tail 2, cap 2, size 0.
		combs->Add({[]() {
			return (UTAArray*) (new UTAHArray2());
		}});
		// Hybrid, tail 2, cap 2, size 1.
		combs->Add({[]() {
			UTAArray* a = new UTAHArray2();
			a->Add(UTAValue(0));
			return a;
		}});
		// Hybrid, tail 2, cap 2, size 2.
		combs->Add({[]() {
			UTAArray* a = new UTAHArray2();
			a->Add(UTAValue(0));
			a->Add(UTAValue(1));
			return a;
		}});
		// Hybrid, tail 2, cap 4, size 0.
		combs->Add({[]() {
			UTAArray* a = new UTAHArray2();
			a->Reserve(4);
			return a;
		}});
		// Hybrid, tail 2, cap 4, size 1.
		combs->Add({[]() {
			UTAArray* a = new UTAHArray2();
			a->Reserve(4);
			a->Add(UTAValue(0));
			return a;
		}});
		// Hybrid, tail 2, cap 4, size 2.
		combs->Add({[]() {
			UTAArray* a = new UTAHArray2();
			a->Reserve(4);
			a->Add(UTAValue(0));
			a->Add(UTAValue(1));
			return a;
		}});
		// Hybrid, tail 2, cap 4, size 3.
		combs->Add({[]() {
			UTAArray* a = new UTAHArray2();
			a->Reserve(4);
			a->Add(UTAValue(0));
			a->Add(UTAValue(1));
			a->Add(UTAValue(2));
			return a;
		}});
		// Hybrid, tail 2, cap 4, size 4.
		combs->Add({[]() {
			UTAArray* a = new UTAHArray2();
			a->Reserve(4);
			a->Add(UTAValue(0));
			a->Add(UTAValue(1));
			a->Add(UTAValue(2));
			a->Add(UTAValue(3));
			return a;
		}});

		return combs;
	}

	static void
	UnitTestArrayAdd()
	{
		UTAValue::ResetCounters();
		{
			mg::common::Array<UTAValue> arr;
			// Nothing is created in an empty array.
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr.GetBuffer() == nullptr);

			UTAValue v(1);
			UTAValue::UseConstrCount(1);

			MG_COMMON_ASSERT(arr.Add(v) == 0);
			MG_COMMON_ASSERT(arr.Count() == 1);
			MG_COMMON_ASSERT(arr.GetCapacity() == 1);
			MG_COMMON_ASSERT(arr[0].myValue == 1);
			UTAValue::UseCopyConstrCount(1);

			v.myValue = 2;
			MG_COMMON_ASSERT(arr.Add(v) == 1);
			MG_COMMON_ASSERT(arr.Count() == 2);
			MG_COMMON_ASSERT(arr.GetCapacity() == 2);
			MG_COMMON_ASSERT(arr[0].myValue == 1);
			MG_COMMON_ASSERT(arr[1].myValue == 2);
			UTAValue::UseCopyConstrCount(1);
			// Because of realloc.
			UTAValue::UseMoveConstrCount(1);
			UTAValue::UseDestrCount(1);

			v.myValue = 3;
			MG_COMMON_ASSERT(arr.Add(v) == 2);
			MG_COMMON_ASSERT(arr.Count() == 3);
			MG_COMMON_ASSERT(arr.GetCapacity() == 4);
			MG_COMMON_ASSERT(arr[0].myValue == 1);
			MG_COMMON_ASSERT(arr[1].myValue == 2);
			MG_COMMON_ASSERT(arr[2].myValue == 3);
			UTAValue::UseCopyConstrCount(1);
			// Because of realloc.
			UTAValue::UseMoveConstrCount(2);
			UTAValue::UseDestrCount(2);

			// Move-add.
			MG_COMMON_ASSERT(arr.Add(UTAValue(4)) == 3);
			MG_COMMON_ASSERT(arr.Count() == 4);
			MG_COMMON_ASSERT(arr.GetCapacity() == 4);
			MG_COMMON_ASSERT(arr[0].myValue == 1);
			MG_COMMON_ASSERT(arr[1].myValue == 2);
			MG_COMMON_ASSERT(arr[2].myValue == 3);
			MG_COMMON_ASSERT(arr[3].myValue == 4);
			UTAValue::UseMoveConstrCount(1);
			// The temporary object.
			UTAValue::UseConstrCount(1);
			UTAValue::UseDestrCount(1);
		}
		// 1 value on the stack + 4 values in the array.
		UTAValue::UseDestrCount(5);
		UTAValue::CheckCounters();

		// All the same with plain type to ensure the policies
		// work correctly.
		{
			mg::common::Array<int> arr;
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr.GetBuffer() == nullptr);

			MG_COMMON_ASSERT(arr.Add(1) == 0);
			MG_COMMON_ASSERT(arr.Count() == 1);
			MG_COMMON_ASSERT(arr.GetCapacity() == 1);
			MG_COMMON_ASSERT(arr[0] == 1);

			MG_COMMON_ASSERT(arr.Add(2) == 1);
			MG_COMMON_ASSERT(arr.Count() == 2);
			MG_COMMON_ASSERT(arr.GetCapacity() == 2);
			MG_COMMON_ASSERT(arr[0] == 1);
			MG_COMMON_ASSERT(arr[1] == 2);

			MG_COMMON_ASSERT(arr.Add(3) == 2);
			MG_COMMON_ASSERT(arr.Count() == 3);
			MG_COMMON_ASSERT(arr.GetCapacity() == 4);
			MG_COMMON_ASSERT(arr[0] == 1);
			MG_COMMON_ASSERT(arr[1] == 2);
			MG_COMMON_ASSERT(arr[2] == 3);
		}
	}

	static void
	UnitTestArrayAddAtIndex()
	{
		UTAValue::ResetCounters();
		{
			mg::common::Array<UTAValue> arr;
			UTAValue v(1);
			UTAValue::UseConstrCount(1);

			MG_COMMON_ASSERT(arr.AddAtIndex(v, 0) == 0);
			MG_COMMON_ASSERT(arr.Count() == 1);
			MG_COMMON_ASSERT(arr.GetCapacity() == 1);
			MG_COMMON_ASSERT(arr[0].myValue == 1);
			UTAValue::UseCopyConstrCount(1);

			v.myValue = 2;
			MG_COMMON_ASSERT(arr.AddAtIndex(v, 0) == 0);
			MG_COMMON_ASSERT(arr.Count() == 2);
			MG_COMMON_ASSERT(arr.GetCapacity() == 2);
			MG_COMMON_ASSERT(arr[0].myValue == 2);
			MG_COMMON_ASSERT(arr[1].myValue == 1);
			UTAValue::UseCopyAssignCount(1);
			// Because of realloc.
			UTAValue::UseMoveConstrCount(1);
			UTAValue::UseDestrCount(1);
			// Create the new last cell from the previous last
			// cell.
			UTAValue::UseMoveConstrCount(1);

			v.myValue = 3;
			MG_COMMON_ASSERT(arr.AddAtIndex(v, 2) == 2);
			MG_COMMON_ASSERT(arr.Count() == 3);
			MG_COMMON_ASSERT(arr.GetCapacity() == 4);
			MG_COMMON_ASSERT(arr[0].myValue == 2);
			MG_COMMON_ASSERT(arr[1].myValue == 1);
			MG_COMMON_ASSERT(arr[2].myValue == 3);
			// Because of realloc.
			UTAValue::UseMoveConstrCount(2);
			UTAValue::UseDestrCount(2);
			// Addition with an index = count turns into a plain
			// append.
			UTAValue::UseCopyConstrCount(1);

			v.myValue = 4;
			MG_COMMON_ASSERT(arr.AddAtIndex(v, 1) == 1);
			MG_COMMON_ASSERT(arr.Count() == 4);
			MG_COMMON_ASSERT(arr.GetCapacity() == 4);
			MG_COMMON_ASSERT(arr[0].myValue == 2);
			MG_COMMON_ASSERT(arr[1].myValue == 4);
			MG_COMMON_ASSERT(arr[2].myValue == 1);
			MG_COMMON_ASSERT(arr[3].myValue == 3);
			UTAValue::UseCopyAssignCount(1);
			// Create the new last cell from the previous last
			// cell.
			UTAValue::UseMoveConstrCount(1);
			// Move the old 1-index element.
			UTAValue::UseMoveAssignCount(1);

			v.myValue = 5;
			MG_COMMON_ASSERT(arr.AddAtIndex(v, 1) == 1);
			MG_COMMON_ASSERT(arr.Count() == 5);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			MG_COMMON_ASSERT(arr[0].myValue == 2);
			MG_COMMON_ASSERT(arr[1].myValue == 5);
			MG_COMMON_ASSERT(arr[2].myValue == 4);
			MG_COMMON_ASSERT(arr[3].myValue == 1);
			MG_COMMON_ASSERT(arr[4].myValue == 3);
			UTAValue::UseCopyAssignCount(1);
			// Because of realloc.
			UTAValue::UseMoveConstrCount(4);
			UTAValue::UseDestrCount(4);
			// Create the new last cell from the previous last
			// cell.
			UTAValue::UseMoveConstrCount(1);
			// Move old elements with indexes 1 and 2.
			UTAValue::UseMoveAssignCount(2);

			// Add bigger than count turns into append.
			v.myValue = 6;
			MG_COMMON_ASSERT(arr.AddAtIndex(v, 100) == 5);
			MG_COMMON_ASSERT(arr.Count() == 6);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			MG_COMMON_ASSERT(arr[0].myValue == 2);
			MG_COMMON_ASSERT(arr[1].myValue == 5);
			MG_COMMON_ASSERT(arr[2].myValue == 4);
			MG_COMMON_ASSERT(arr[3].myValue == 1);
			MG_COMMON_ASSERT(arr[4].myValue == 3);
			MG_COMMON_ASSERT(arr[5].myValue == 6);
			UTAValue::UseCopyConstrCount(1);
		}
		// 1 value on the stack + 6 values in the array.
		UTAValue::UseDestrCount(7);
		UTAValue::CheckCounters();

		// All the same with plain type to ensure the policies
		// work correctly.
		{
			mg::common::Array<int> arr;

			MG_COMMON_ASSERT(arr.AddAtIndex(1, 0) == 0);
			MG_COMMON_ASSERT(arr.Count() == 1);
			MG_COMMON_ASSERT(arr.GetCapacity() == 1);
			MG_COMMON_ASSERT(arr[0] == 1);

			MG_COMMON_ASSERT(arr.AddAtIndex(2, 0) == 0);
			MG_COMMON_ASSERT(arr.Count() == 2);
			MG_COMMON_ASSERT(arr.GetCapacity() == 2);
			MG_COMMON_ASSERT(arr[0] == 2);
			MG_COMMON_ASSERT(arr[1] == 1);

			MG_COMMON_ASSERT(arr.AddAtIndex(3, 2) == 2);
			MG_COMMON_ASSERT(arr.Count() == 3);
			MG_COMMON_ASSERT(arr.GetCapacity() == 4);
			MG_COMMON_ASSERT(arr[0] == 2);
			MG_COMMON_ASSERT(arr[1] == 1);
			MG_COMMON_ASSERT(arr[2] == 3);

			MG_COMMON_ASSERT(arr.AddAtIndex(4, 1) == 1);
			MG_COMMON_ASSERT(arr.Count() == 4);
			MG_COMMON_ASSERT(arr.GetCapacity() == 4);
			MG_COMMON_ASSERT(arr[0] == 2);
			MG_COMMON_ASSERT(arr[1] == 4);
			MG_COMMON_ASSERT(arr[2] == 1);
			MG_COMMON_ASSERT(arr[3] == 3);

			MG_COMMON_ASSERT(arr.AddAtIndex(5, 1) == 1);
			MG_COMMON_ASSERT(arr.Count() == 5);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			MG_COMMON_ASSERT(arr[0] == 2);
			MG_COMMON_ASSERT(arr[1] == 5);
			MG_COMMON_ASSERT(arr[2] == 4);
			MG_COMMON_ASSERT(arr[3] == 1);
			MG_COMMON_ASSERT(arr[4] == 3);

			MG_COMMON_ASSERT(arr.AddAtIndex(6, 100) == 5);
			MG_COMMON_ASSERT(arr.Count() == 6);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			MG_COMMON_ASSERT(arr[0] == 2);
			MG_COMMON_ASSERT(arr[1] == 5);
			MG_COMMON_ASSERT(arr[2] == 4);
			MG_COMMON_ASSERT(arr[3] == 1);
			MG_COMMON_ASSERT(arr[4] == 3);
			MG_COMMON_ASSERT(arr[5] == 6);
		}
	}

	static void
	UnitTestArrayRemoveAtIndex()
	{
		{
			mg::common::Array<UTAValue> arr;
			for (int i = 1; i <= 5; ++i)
				arr.Add(UTAValue(i));
			UTAValue::ResetCounters();

			// Try to remove last.
			arr.RemoveAtIndex(4);
			MG_COMMON_ASSERT(arr.Count() == 4);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			MG_COMMON_ASSERT(arr[0].myValue == 1);
			MG_COMMON_ASSERT(arr[1].myValue == 2);
			MG_COMMON_ASSERT(arr[2].myValue == 3);
			MG_COMMON_ASSERT(arr[3].myValue == 4);
			UTAValue::UseDestrCount(1);

			// Try to remove from the middle.
			arr.RemoveAtIndex(1);
			MG_COMMON_ASSERT(arr.Count() == 3);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			MG_COMMON_ASSERT(arr[0].myValue == 1);
			MG_COMMON_ASSERT(arr[1].myValue == 3);
			MG_COMMON_ASSERT(arr[2].myValue == 4);
			UTAValue::UseDestrCount(1);
			// There were 2 elements after the removed one, and
			// they are moved.
			UTAValue::UseMoveAssignCount(2);

			// Try to remove first.
			arr.RemoveAtIndex(0);
			MG_COMMON_ASSERT(arr.Count() == 2);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			MG_COMMON_ASSERT(arr[0].myValue == 3);
			MG_COMMON_ASSERT(arr[1].myValue == 4);
			UTAValue::UseMoveAssignCount(2);
			UTAValue::UseDestrCount(1);

			// Remove all.
			arr.RemoveAtIndex(0);
			UTAValue::UseMoveAssignCount(1);
			UTAValue::UseDestrCount(1);
			arr.RemoveAtIndex(0);
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			UTAValue::UseDestrCount(1);
		}
		UTAValue::CheckCounters();

		// All the same with plain type to ensure the policies
		// work correctly.
		{
			mg::common::Array<int> arr;
			for (int i = 1; i <= 5; ++i)
				arr.Add(i);

			arr.RemoveAtIndex(4);
			MG_COMMON_ASSERT(arr.Count() == 4);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			MG_COMMON_ASSERT(arr[0] == 1);
			MG_COMMON_ASSERT(arr[1] == 2);
			MG_COMMON_ASSERT(arr[2] == 3);
			MG_COMMON_ASSERT(arr[3] == 4);

			arr.RemoveAtIndex(1);
			MG_COMMON_ASSERT(arr.Count() == 3);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			MG_COMMON_ASSERT(arr[0] == 1);
			MG_COMMON_ASSERT(arr[1] == 3);
			MG_COMMON_ASSERT(arr[2] == 4);

			arr.RemoveAtIndex(0);
			MG_COMMON_ASSERT(arr.Count() == 2);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			MG_COMMON_ASSERT(arr[0] == 3);
			MG_COMMON_ASSERT(arr[1] == 4);

			arr.RemoveAtIndex(0);
			arr.RemoveAtIndex(0);
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
		}
	}

	static void
	UnitTestArrayDeleteAtIndex()
	{
		{
			mg::common::Array<UTAValue*> arr;
			for (int i = 1; i <= 5; ++i)
				arr.Add(new UTAValue(i));
			UTAValue::ResetCounters();

			// Try to delete last.
			arr.DeleteAtIndex(4);
			MG_COMMON_ASSERT(arr.Count() == 4);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			MG_COMMON_ASSERT(arr[0]->myValue == 1);
			MG_COMMON_ASSERT(arr[1]->myValue == 2);
			MG_COMMON_ASSERT(arr[2]->myValue == 3);
			MG_COMMON_ASSERT(arr[3]->myValue == 4);
			UTAValue::UseDestrCount(1);
			UTAValue::UseDelCount(1);

			// Try to delete from the middle.
			arr.DeleteAtIndex(1);
			MG_COMMON_ASSERT(arr.Count() == 3);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			MG_COMMON_ASSERT(arr[0]->myValue == 1);
			MG_COMMON_ASSERT(arr[1]->myValue == 3);
			MG_COMMON_ASSERT(arr[2]->myValue == 4);
			UTAValue::UseDestrCount(1);
			UTAValue::UseDelCount(1);

			// Try to delete first.
			arr.DeleteAtIndex(0);
			MG_COMMON_ASSERT(arr.Count() == 2);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			MG_COMMON_ASSERT(arr[0]->myValue == 3);
			MG_COMMON_ASSERT(arr[1]->myValue == 4);
			UTAValue::UseDestrCount(1);
			UTAValue::UseDelCount(1);

			// Delete all.
			arr.DeleteAtIndex(0);
			UTAValue::UseDestrCount(1);
			UTAValue::UseDelCount(1);
			arr.DeleteAtIndex(0);
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			UTAValue::UseDestrCount(1);
			UTAValue::UseDelCount(1);
		}
		UTAValue::CheckCounters();

		// All the same with plain type to ensure the policies
		// work correctly.
		{
			mg::common::Array<int*> arr;
			for (int i = 1; i <= 5; ++i)
				arr.Add(new int(i));

			arr.DeleteAtIndex(4);
			MG_COMMON_ASSERT(arr.Count() == 4);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			MG_COMMON_ASSERT(*arr[0] == 1);
			MG_COMMON_ASSERT(*arr[1] == 2);
			MG_COMMON_ASSERT(*arr[2] == 3);
			MG_COMMON_ASSERT(*arr[3] == 4);

			arr.DeleteAtIndex(1);
			MG_COMMON_ASSERT(arr.Count() == 3);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			MG_COMMON_ASSERT(*arr[0] == 1);
			MG_COMMON_ASSERT(*arr[1] == 3);
			MG_COMMON_ASSERT(*arr[2] == 4);

			arr.DeleteAtIndex(0);
			MG_COMMON_ASSERT(arr.Count() == 2);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			MG_COMMON_ASSERT(*arr[0] == 3);
			MG_COMMON_ASSERT(*arr[1] == 4);

			arr.DeleteAtIndex(0);
			arr.DeleteAtIndex(0);
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
		}
	}

	static void
	UnitTestArrayRemoveCyclicAtIndex()
	{
		{
			mg::common::Array<UTAValue> arr;
			for (int i = 1; i <= 5; ++i)
				arr.Add(UTAValue(i));
			UTAValue::ResetCounters();

			// Try to remove last.
			arr.RemoveCyclicAtIndex(4);
			MG_COMMON_ASSERT(arr.Count() == 4);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			MG_COMMON_ASSERT(arr[0].myValue == 1);
			MG_COMMON_ASSERT(arr[1].myValue == 2);
			MG_COMMON_ASSERT(arr[2].myValue == 3);
			MG_COMMON_ASSERT(arr[3].myValue == 4);
			UTAValue::UseDestrCount(1);

			// Try to remove from the middle.
			arr.RemoveCyclicAtIndex(1);
			MG_COMMON_ASSERT(arr.Count() == 3);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			MG_COMMON_ASSERT(arr[0].myValue == 1);
			MG_COMMON_ASSERT(arr[1].myValue == 4);
			MG_COMMON_ASSERT(arr[2].myValue == 3);
			// Move the last element in place of the removed one.
			// The last's location is destroyed.
			UTAValue::UseMoveAssignCount(1);
			UTAValue::UseDestrCount(1);

			// Try to remove first.
			arr.RemoveCyclicAtIndex(0);
			MG_COMMON_ASSERT(arr.Count() == 2);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			MG_COMMON_ASSERT(arr[0].myValue == 3);
			MG_COMMON_ASSERT(arr[1].myValue == 4);
			UTAValue::UseMoveAssignCount(1);
			UTAValue::UseDestrCount(1);

			// Remove all.
			arr.RemoveCyclicAtIndex(0);
			UTAValue::UseMoveAssignCount(1);
			UTAValue::UseDestrCount(1);
			arr.RemoveCyclicAtIndex(0);
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			UTAValue::UseDestrCount(1);
		}
		UTAValue::CheckCounters();

		// All the same with plain type to ensure the policies
		// work correctly.
		{
			mg::common::Array<int> arr;
			for (int i = 1; i <= 5; ++i)
				arr.Add(i);

			arr.RemoveCyclicAtIndex(4);
			MG_COMMON_ASSERT(arr.Count() == 4);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			MG_COMMON_ASSERT(arr[0] == 1);
			MG_COMMON_ASSERT(arr[1] == 2);
			MG_COMMON_ASSERT(arr[2] == 3);
			MG_COMMON_ASSERT(arr[3] == 4);

			arr.RemoveCyclicAtIndex(1);
			MG_COMMON_ASSERT(arr.Count() == 3);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			MG_COMMON_ASSERT(arr[0] == 1);
			MG_COMMON_ASSERT(arr[1] == 4);
			MG_COMMON_ASSERT(arr[2] == 3);

			arr.RemoveCyclicAtIndex(0);
			MG_COMMON_ASSERT(arr.Count() == 2);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			MG_COMMON_ASSERT(arr[0] == 3);
			MG_COMMON_ASSERT(arr[1] == 4);

			arr.RemoveCyclicAtIndex(0);
			arr.RemoveCyclicAtIndex(0);
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
		}
	}

	static void
	UnitTestArrayDeleteCyclicAtIndex()
	{
		{
			mg::common::Array<UTAValue*> arr;
			for (int i = 1; i <= 5; ++i)
				arr.Add(new UTAValue(i));
			UTAValue::ResetCounters();

			// Try to delete last.
			arr.DeleteCyclicAtIndex(4);
			MG_COMMON_ASSERT(arr.Count() == 4);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			MG_COMMON_ASSERT(arr[0]->myValue == 1);
			MG_COMMON_ASSERT(arr[1]->myValue == 2);
			MG_COMMON_ASSERT(arr[2]->myValue == 3);
			MG_COMMON_ASSERT(arr[3]->myValue == 4);
			UTAValue::UseDestrCount(1);
			UTAValue::UseDelCount(1);

			// Try to delete from the middle.
			arr.DeleteCyclicAtIndex(1);
			MG_COMMON_ASSERT(arr.Count() == 3);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			MG_COMMON_ASSERT(arr[0]->myValue == 1);
			MG_COMMON_ASSERT(arr[1]->myValue == 4);
			MG_COMMON_ASSERT(arr[2]->myValue == 3);
			UTAValue::UseDestrCount(1);
			UTAValue::UseDelCount(1);

			// Try to delete first.
			arr.DeleteCyclicAtIndex(0);
			MG_COMMON_ASSERT(arr.Count() == 2);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			MG_COMMON_ASSERT(arr[0]->myValue == 3);
			MG_COMMON_ASSERT(arr[1]->myValue == 4);
			UTAValue::UseDestrCount(1);
			UTAValue::UseDelCount(1);

			// Delete all.
			arr.DeleteCyclicAtIndex(0);
			UTAValue::UseDestrCount(1);
			UTAValue::UseDelCount(1);
			arr.DeleteCyclicAtIndex(0);
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			UTAValue::UseDestrCount(1);
			UTAValue::UseDelCount(1);
		}
		UTAValue::CheckCounters();

		// All the same with plain type to ensure the policies
		// work correctly.
		{
			mg::common::Array<int*> arr;
			for (int i = 1; i <= 5; ++i)
				arr.Add(new int(i));

			arr.DeleteCyclicAtIndex(4);
			MG_COMMON_ASSERT(arr.Count() == 4);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			MG_COMMON_ASSERT(*arr[0] == 1);
			MG_COMMON_ASSERT(*arr[1] == 2);
			MG_COMMON_ASSERT(*arr[2] == 3);
			MG_COMMON_ASSERT(*arr[3] == 4);

			arr.DeleteCyclicAtIndex(1);
			MG_COMMON_ASSERT(arr.Count() == 3);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			MG_COMMON_ASSERT(*arr[0] == 1);
			MG_COMMON_ASSERT(*arr[1] == 4);
			MG_COMMON_ASSERT(*arr[2] == 3);

			arr.DeleteCyclicAtIndex(0);
			MG_COMMON_ASSERT(arr.Count() == 2);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			MG_COMMON_ASSERT(*arr[0] == 3);
			MG_COMMON_ASSERT(*arr[1] == 4);

			arr.DeleteCyclicAtIndex(0);
			arr.DeleteCyclicAtIndex(0);
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
		}
	}

	static void
	UnitTestArrayRemoveLast()
	{
		{
			mg::common::Array<UTAValue> arr;

			// Remove all when only 1 element.
			arr.Add(UTAValue(1));
			UTAValue::ResetCounters();
			arr.RemoveLast();
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 1);
			UTAValue::UseDestrCount(1);

			UTAValue::CheckCounters();
			for (int i = 1; i <= 5; ++i)
				arr.Add(UTAValue(i));
			UTAValue::ResetCounters();

			for (int count = 5; count >= 1; --count)
			{
				int newCount = count - 1;
				arr.RemoveLast();
				MG_COMMON_ASSERT(arr.Count() == (uint32_t)newCount);
				MG_COMMON_ASSERT(arr.GetCapacity() == 8);
				for (int j = 0; j < newCount; ++j)
					MG_COMMON_ASSERT(arr[j].myValue == j + 1);
				UTAValue::UseDestrCount(1);
			}
		}
		UTAValue::CheckCounters();

		// All the same with plain type to ensure the policies
		// work correctly.
		{
			mg::common::Array<int> arr;

			arr.Add(1);
			arr.RemoveLast();
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 1);

			for (int i = 1; i <= 5; ++i)
				arr.Add(i);

			for (int count = 5; count >= 1; --count)
			{
				int newCount = count - 1;
				arr.RemoveLast();
				MG_COMMON_ASSERT(arr.Count() == (uint32_t)newCount);
				MG_COMMON_ASSERT(arr.GetCapacity() == 8);
				for (int j = 0; j < newCount; ++j)
					MG_COMMON_ASSERT(arr[j] == j + 1);
			}
		}
	}

	static void
	UnitTestArrayDeleteLast()
	{
		{
			mg::common::Array<UTAValue*> arr;

			// Delete when only 1 element.
			arr.Add(new UTAValue(1));
			UTAValue::ResetCounters();
			arr.DeleteLast();
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 1);
			UTAValue::UseDestrCount(1);
			UTAValue::UseDelCount(1);

			UTAValue::CheckCounters();
			for (int i = 1; i <= 5; ++i)
				arr.Add(new UTAValue(i));
			UTAValue::ResetCounters();

			for (int count = 5; count >= 1; --count)
			{
				int newCount = count - 1;
				arr.DeleteLast();
				MG_COMMON_ASSERT(arr.Count() == (uint32_t)newCount);
				MG_COMMON_ASSERT(arr.GetCapacity() == 8);
				for (int j = 0; j < newCount; ++j)
					MG_COMMON_ASSERT(arr[j]->myValue == j + 1);
				UTAValue::UseDestrCount(1);
				UTAValue::UseDelCount(1);
			}
		}
		UTAValue::CheckCounters();

		// All the same with plain type to ensure the policies
		// work correctly.
		{
			mg::common::Array<int*> arr;

			arr.Add(new int(1));
			arr.DeleteLast();
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 1);

			for (int i = 1; i <= 5; ++i)
				arr.Add(new int(i));

			for (int count = 5; count >= 1; --count)
			{
				int newCount = count - 1;
				arr.DeleteLast();
				MG_COMMON_ASSERT(arr.Count() == (uint32_t)newCount);
				MG_COMMON_ASSERT(arr.GetCapacity() == 8);
				for (int j = 0; j < newCount; ++j)
					MG_COMMON_ASSERT(*arr[j] == j + 1);
			}
		}
	}

	static void
	UnitTestArrayRemoveAll()
	{
		UTAValue::ResetCounters();
		{
			mg::common::Array<UTAValue> arr;

			arr.RemoveAll();
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 0);
			UTAValue::CheckCounters();

			// Remove all when only 1 element.
			arr.Add(UTAValue(1));
			UTAValue::ResetCounters();
			arr.RemoveAll();
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 1);
			UTAValue::UseDestrCount(1);

			UTAValue::CheckCounters();
			for (int i = 1; i <= 5; ++i)
				arr.Add(UTAValue(i));
			UTAValue::ResetCounters();
			MG_COMMON_ASSERT(arr.Count() == 5);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);

			arr.RemoveAll();
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			UTAValue::UseDestrCount(5);
		}
		UTAValue::CheckCounters();

		// All the same with plain type to ensure the policies
		// work correctly.
		{
			mg::common::Array<int> arr;

			arr.RemoveAll();
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 0);
			UTAValue::CheckCounters();

			arr.Add(1);
			arr.RemoveAll();
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 1);

			for (int i = 1; i <= 5; ++i)
				arr.Add(i);
			MG_COMMON_ASSERT(arr.Count() == 5);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);

			arr.RemoveAll();
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
		}
	}

	static void
	UnitTestArrayDeleteAll()
	{
		{
			mg::common::Array<UTAValue*> arr;

			arr.DeleteAll();
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 0);
			UTAValue::CheckCounters();

			// Delete all when only 1 element.
			arr.Add(new UTAValue(1));
			UTAValue::ResetCounters();
			arr.DeleteAll();
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 1);
			UTAValue::UseDestrCount(1);
			UTAValue::UseDelCount(1);

			UTAValue::CheckCounters();
			for (int i = 1; i <= 5; ++i)
				arr.Add(new UTAValue(i));
			UTAValue::ResetCounters();
			MG_COMMON_ASSERT(arr.Count() == 5);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);

			arr.DeleteAll();
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
			UTAValue::UseDestrCount(5);
			UTAValue::UseDelCount(5);
		}
		UTAValue::CheckCounters();

		// All the same with plain type to ensure the policies
		// work correctly.
		{
			mg::common::Array<int*> arr;

			arr.DeleteAll();
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 0);
			UTAValue::CheckCounters();

			arr.Add(new int(1));
			arr.DeleteAll();
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 1);

			for (int i = 1; i <= 5; ++i)
				arr.Add(new int(i));
			MG_COMMON_ASSERT(arr.Count() == 5);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);

			arr.DeleteAll();
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 8);
		}
	}

	static void
	UnitTestArrayPopLast()
	{
		{
			mg::common::Array<UTAValue> arr;

			// Pop when only 1 element.
			arr.Add(UTAValue(1));
			UTAValue::ResetCounters();
			UTAValue v = arr.PopLast();
			MG_COMMON_ASSERT(v.myValue == 1);
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 1);
#if IS_COMPILER_MSVC && IS_BUILD_DEBUG
			// One value is move-constructed inside PopLast on the
			// stack to be able to delete the original value in
			// the array. Deleted on return.
			UTAValue::UseMoveConstrCount(1);
			UTAValue::UseDestrCount(1);
			// One value is move constructed on the stack here.
			UTAValue::UseMoveConstrCount(1);
			// The popped value is deleted from array's memory.
			UTAValue::UseDestrCount(1);
#else
			// Copy elision optimization allows to move the popped
			// object directly without intermediate objects.
			UTAValue::UseMoveConstrCount(1);
			UTAValue::UseDestrCount(1);
#endif

			UTAValue::CheckCounters();
			arr.Add(UTAValue(2));
			arr.Add(UTAValue(3));
			UTAValue::ResetCounters();

			MG_COMMON_ASSERT(arr.Count() == 2);
			MG_COMMON_ASSERT(arr.GetCapacity() == 2);
			v = arr.PopLast();
			MG_COMMON_ASSERT(v.myValue == 3);
			MG_COMMON_ASSERT(arr[0].myValue == 2);
			MG_COMMON_ASSERT(arr.Count() == 1);
			MG_COMMON_ASSERT(arr.GetCapacity() == 2);
#if IS_COMPILER_MSVC && IS_BUILD_DEBUG
			// One value is move-constructed inside PopLast on the
			// stack to be able to delete the original value in
			// the array. Deleted on return.
			UTAValue::UseMoveConstrCount(1);
			UTAValue::UseDestrCount(1);
			// The popped value is deleted from array's memory.
			UTAValue::UseDestrCount(1);
			// One value is moved to the stack here.
			UTAValue::UseMoveAssignCount(1);
#else
			// Copy elision optimization.
			UTAValue::UseMoveAssignCount(1);
			UTAValue::UseDestrCount(1);
#endif
			// The returned value is stored to a temporary
			// location from where it is assigned to the value on
			// the stack here.
			UTAValue::UseMoveConstrCount(1);
			UTAValue::UseDestrCount(1);

			v = arr.PopLast();
			MG_COMMON_ASSERT(v.myValue == 2);
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 2);
#if IS_COMPILER_MSVC && IS_BUILD_DEBUG
			// All the same as above. Inside of PopLast():
			UTAValue::UseMoveConstrCount(1);
			UTAValue::UseDestrCount(1);
			// Original value:
			UTAValue::UseDestrCount(1);
			// Move to this stack.
			UTAValue::UseMoveAssignCount(1);
#else
			// Copy elision optimization.
			UTAValue::UseMoveAssignCount(1);
			UTAValue::UseDestrCount(1);
#endif
			// Temporary returned value.
			UTAValue::UseMoveConstrCount(1);
			UTAValue::UseDestrCount(1);
		}
		UTAValue::UseDestrCount(1);
		UTAValue::CheckCounters();

		// All the same with plain type to ensure the policies
		// work correctly.
		{
			mg::common::Array<int> arr;

			arr.Add(1);
			int v = arr.PopLast();
			MG_COMMON_ASSERT(v == 1);
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 1);

			arr.Add(2);
			arr.Add(3);
			UTAValue::ResetCounters();
			MG_COMMON_ASSERT(arr.Count() == 2);
			MG_COMMON_ASSERT(arr.GetCapacity() == 2);
			v = arr.PopLast();
			MG_COMMON_ASSERT(v == 3);
			MG_COMMON_ASSERT(arr[0] == 2);
			MG_COMMON_ASSERT(arr.Count() == 1);
			MG_COMMON_ASSERT(arr.GetCapacity() == 2);

			v = arr.PopLast();
			MG_COMMON_ASSERT(v == 2);
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 2);
		}
	}

	static void
	UnitTestArrayIsDynamic()
	{
		{
			mg::common::Array<UTAValue> arr;
			MG_COMMON_ASSERT(arr.IsDynamic());

			arr.SetCount(1);
			MG_COMMON_ASSERT(arr.IsDynamic());
		}
		{
			mg::common::HybridArray<UTAValue, 3> arr;
			MG_COMMON_ASSERT(!arr.IsDynamic());
			arr.SetCount(3);
			MG_COMMON_ASSERT(!arr.IsDynamic());
			arr.SetCount(4);
			MG_COMMON_ASSERT(arr.IsDynamic());
		}

		// All the same with plain type to ensure the policies
		// work correctly.
		{
			mg::common::Array<int> arr;
			MG_COMMON_ASSERT(arr.IsDynamic());

			arr.SetCount(1);
			MG_COMMON_ASSERT(arr.IsDynamic());
		}
		{
			mg::common::HybridArray<int, 3> arr;
			MG_COMMON_ASSERT(!arr.IsDynamic());
			arr.SetCount(3);
			MG_COMMON_ASSERT(!arr.IsDynamic());
			arr.SetCount(4);
			MG_COMMON_ASSERT(arr.IsDynamic());
		}
	}

	static void
	UnitTestArrayOperatorEqual()
	{
		{
			mg::common::Array<UTAValue> arr1;
			mg::common::Array<UTAValue> arr2;
			MG_COMMON_ASSERT(arr1 == arr2 && arr2 == arr1);
			MG_COMMON_ASSERT(!(arr1 != arr2) && !(arr2 != arr1));

			arr1.Add(UTAValue(1));
			MG_COMMON_ASSERT(arr1 != arr2 && arr2 != arr1);
			MG_COMMON_ASSERT(!(arr1 == arr2) && !(arr2 == arr1));

			arr2.Add(UTAValue(2));
			MG_COMMON_ASSERT(arr1 != arr2 && arr2 != arr1);
			MG_COMMON_ASSERT(!(arr1 == arr2) && !(arr2 == arr1));

			// Not enough to just have the same elements. The
			// order must be the same too.
			arr1.Add(UTAValue(2));
			arr2.Add(UTAValue(1));
			MG_COMMON_ASSERT(arr1 != arr2 && arr2 != arr1);
			MG_COMMON_ASSERT(!(arr1 == arr2) && !(arr2 == arr1));

			arr2[0] = UTAValue(1);
			arr2[1] = UTAValue(2);
			MG_COMMON_ASSERT(arr1 == arr2 && arr2 == arr1);
			MG_COMMON_ASSERT(!(arr1 != arr2) && !(arr2 != arr1));

			// Arrays of different types and static sizes can also
			// be compared.
			mg::common::HybridArray<UTAValue, 3> arr3;
			mg::common::HybridArray<UTAValue, 5> arr4;
			MG_COMMON_ASSERT(arr1 != arr3 && arr3 != arr1);
			MG_COMMON_ASSERT(!(arr1 == arr3) && !(arr3 == arr1));
			MG_COMMON_ASSERT(arr3 == arr4 && arr4 == arr3);
			MG_COMMON_ASSERT(!(arr3 != arr4) && !(arr4 != arr3));

			arr3.Add(UTAValue(1));
			arr3.Add(UTAValue(2));
			MG_COMMON_ASSERT(arr1 == arr3 && arr3 == arr1);
			MG_COMMON_ASSERT(!(arr1 != arr3) && !(arr3 != arr1));

			arr4.Add(UTAValue(2));
			arr4.Add(UTAValue(1));
			MG_COMMON_ASSERT(arr1 != arr4 && arr4 != arr1);
			MG_COMMON_ASSERT(!(arr1 == arr4) && !(arr4 == arr1));

			arr4[0] = UTAValue(1);
			arr4[1] = UTAValue(2);
			MG_COMMON_ASSERT(arr1 == arr4 && arr4 == arr1);
			MG_COMMON_ASSERT(!(arr1 != arr4) && !(arr4 != arr1));

			arr1.RemoveAll();
			arr3.RemoveAll();
			MG_COMMON_ASSERT(arr1 == arr3 && arr3 == arr1);
			MG_COMMON_ASSERT(!(arr1 != arr3) && !(arr3 != arr1));
		}

		// All the same with plain type to ensure the policies
		// work correctly.
		{
			mg::common::Array<int> arr1;
			mg::common::Array<int> arr2;
			MG_COMMON_ASSERT(arr1 == arr2 && arr2 == arr1);
			MG_COMMON_ASSERT(!(arr1 != arr2) && !(arr2 != arr1));

			arr1.Add(1);
			MG_COMMON_ASSERT(arr1 != arr2 && arr2 != arr1);
			MG_COMMON_ASSERT(!(arr1 == arr2) && !(arr2 == arr1));

			arr2.Add(2);
			MG_COMMON_ASSERT(arr1 != arr2 && arr2 != arr1);
			MG_COMMON_ASSERT(!(arr1 == arr2) && !(arr2 == arr1));

			arr1.Add(2);
			arr2.Add(1);
			MG_COMMON_ASSERT(arr1 != arr2 && arr2 != arr1);
			MG_COMMON_ASSERT(!(arr1 == arr2) && !(arr2 == arr1));

			arr2[0] = 1;
			arr2[1] = 2;
			MG_COMMON_ASSERT(arr1 == arr2 && arr2 == arr1);
			MG_COMMON_ASSERT(!(arr1 != arr2) && !(arr2 != arr1));

			mg::common::HybridArray<int, 3> arr3;
			mg::common::HybridArray<int, 5> arr4;
			MG_COMMON_ASSERT(arr1 != arr3 && arr3 != arr1);
			MG_COMMON_ASSERT(!(arr1 == arr3) && !(arr3 == arr1));
			MG_COMMON_ASSERT(arr3 == arr4 && arr4 == arr3);
			MG_COMMON_ASSERT(!(arr3 != arr4) && !(arr4 != arr3));

			arr3.Add(1);
			arr3.Add(2);
			MG_COMMON_ASSERT(arr1 == arr3 && arr3 == arr1);
			MG_COMMON_ASSERT(!(arr1 != arr3) && !(arr3 != arr1));

			arr4.Add(2);
			arr4.Add(1);
			MG_COMMON_ASSERT(arr1 != arr4 && arr4 != arr1);
			MG_COMMON_ASSERT(!(arr1 == arr4) && !(arr4 == arr1));

			arr4[0] = 1;
			arr4[1] = 2;
			MG_COMMON_ASSERT(arr1 == arr4 && arr4 == arr1);
			MG_COMMON_ASSERT(!(arr1 != arr4) && !(arr4 != arr1));

			arr1.RemoveAll();
			arr3.RemoveAll();
			MG_COMMON_ASSERT(arr1 == arr3 && arr3 == arr1);
			MG_COMMON_ASSERT(!(arr1 != arr3) && !(arr3 != arr1));
		}
	}

	static void
	UnitTestArrayReserve()
	{
		UTAValue::ResetCounters();
		{
			mg::common::Array<UTAValue> arr;

			arr.Reserve(0);
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 0);

			arr.Reserve(1);
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 1);

			// Ensure Reserve does not shrink the array.
			arr.Reserve(1);
			arr.Reserve(0);
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 1);

			// Ensure Reserve does not respect power of 2
			// capacity.
			arr.Reserve(3);
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 3);

			UTAValue::CheckCounters();
			for (int i = 1; i <= 3; ++i)
				arr.Add(UTAValue(i));
			UTAValue::ResetCounters();

			MG_COMMON_ASSERT(arr.Count() == 3);
			MG_COMMON_ASSERT(arr.GetCapacity() == 3);
			arr.Reserve(3);
			MG_COMMON_ASSERT(arr.Count() == 3);
			MG_COMMON_ASSERT(arr.GetCapacity() == 3);
			MG_COMMON_ASSERT(arr[0].myValue == 1);
			MG_COMMON_ASSERT(arr[1].myValue == 2);
			MG_COMMON_ASSERT(arr[2].myValue == 3);

			arr.Reserve(5);
			MG_COMMON_ASSERT(arr.Count() == 3);
			// Capacity is not 5, because old capacity 3 * 2 > 5.
			MG_COMMON_ASSERT(arr.GetCapacity() == 6);
			MG_COMMON_ASSERT(arr[0].myValue == 1);
			MG_COMMON_ASSERT(arr[1].myValue == 2);
			MG_COMMON_ASSERT(arr[2].myValue == 3);
			// Realloc.
			UTAValue::UseMoveConstrCount(3);
			UTAValue::UseDestrCount(3);
		}
		UTAValue::UseDestrCount(3);
		UTAValue::CheckCounters();
		// Dynamic can switch back to static if the static is
		// bigger.
		{
			mg::common::Array<UTAValue> arr1;
			arr1.SetCount(2);
			UTAValue* buf1 = arr1.GetBuffer();
			mg::common::HybridArray<UTAValue, 10> arr2;
			UTAValue* buf2 = arr2.GetBuffer();

			UTAValue::ResetCounters();
			mg::common::Array<UTAValue> tmp;
			tmp = mg::common::Move(arr2);
			arr2 = mg::common::Move(arr1);
			arr1 = mg::common::Move(tmp);

			MG_COMMON_ASSERT(arr1.IsDynamic());
			MG_COMMON_ASSERT(arr1.GetBuffer() == nullptr);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr1.Count() == 0);

			MG_COMMON_ASSERT(arr2.IsDynamic());
			MG_COMMON_ASSERT(arr2.GetBuffer() == buf1);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 2);
			MG_COMMON_ASSERT(arr2.Count() == 2);

			arr2.Reserve(4);
			// Move 2 old elements.
			UTAValue::UseMoveConstrCount(2);
			UTAValue::UseDestrCount(2);

			// The static buffer is reused instead of creating a
			// new dynamic.
			MG_COMMON_ASSERT(!arr2.IsDynamic());
			MG_COMMON_ASSERT(arr2.GetBuffer() == buf2);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 10);
			MG_COMMON_ASSERT(arr2.Count() == 2);

			arr1 = mg::common::Move(arr2);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
			UTAValue::UseMoveConstrCount(2);
			UTAValue::UseDestrCount(2);

			arr2.SetCount(1);
			UTAValue::UseConstrCount(1);
			// The static buffer is again reused. Now after the
			// move.
			MG_COMMON_ASSERT(!arr2.IsDynamic());
			MG_COMMON_ASSERT(arr2.GetBuffer() == buf2);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 10);
			MG_COMMON_ASSERT(arr2.Count() == 1);
		}
		UTAValue::UseDestrCount(3);
		UTAValue::CheckCounters();

		// All the same with plain type to ensure the policies
		// work correctly.
		{
			mg::common::Array<int> arr;

			arr.Reserve(0);
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 0);

			arr.Reserve(1);
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 1);

			arr.Reserve(1);
			arr.Reserve(0);
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 1);

			arr.Reserve(3);
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 3);

			for (int i = 1; i <= 3; ++i)
				arr.Add(i);
			MG_COMMON_ASSERT(arr.Count() == 3);
			MG_COMMON_ASSERT(arr.GetCapacity() == 3);
			arr.Reserve(3);
			MG_COMMON_ASSERT(arr.Count() == 3);
			MG_COMMON_ASSERT(arr.GetCapacity() == 3);
			MG_COMMON_ASSERT(arr[0] == 1);
			MG_COMMON_ASSERT(arr[1] == 2);
			MG_COMMON_ASSERT(arr[2] == 3);

			arr.Reserve(5);
			MG_COMMON_ASSERT(arr.Count() == 3);
			MG_COMMON_ASSERT(arr.GetCapacity() == 6);
			MG_COMMON_ASSERT(arr[0] == 1);
			MG_COMMON_ASSERT(arr[1] == 2);
			MG_COMMON_ASSERT(arr[2] == 3);
		}
		{
			mg::common::Array<int> arr1;
			arr1.SetCount(2);
			int* buf1 = arr1.GetBuffer();
			mg::common::HybridArray<int, 10> arr2;
			int* buf2 = arr2.GetBuffer();

			mg::common::Array<int> tmp;
			tmp = mg::common::Move(arr2);
			arr2 = mg::common::Move(arr1);
			arr1 = mg::common::Move(tmp);

			MG_COMMON_ASSERT(arr1.IsDynamic());
			MG_COMMON_ASSERT(arr1.GetBuffer() == nullptr);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr1.Count() == 0);

			MG_COMMON_ASSERT(arr2.IsDynamic());
			MG_COMMON_ASSERT(arr2.GetBuffer() == buf1);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 2);
			MG_COMMON_ASSERT(arr2.Count() == 2);

			arr2.Reserve(4);

			MG_COMMON_ASSERT(!arr2.IsDynamic());
			MG_COMMON_ASSERT(arr2.GetBuffer() == buf2);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 10);
			MG_COMMON_ASSERT(arr2.Count() == 2);

			arr1 = mg::common::Move(arr2);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);

			arr2.SetCount(1);
			MG_COMMON_ASSERT(!arr2.IsDynamic());
			MG_COMMON_ASSERT(arr2.GetBuffer() == buf2);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 10);
			MG_COMMON_ASSERT(arr2.Count() == 1);
		}
	}

	static void
	UnitTestArraySetCount()
	{
		UTAValue::ResetCounters();
		{
			mg::common::Array<UTAValue> arr;

			arr.SetCount(0);
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 0);

			arr.SetCount(1);
			MG_COMMON_ASSERT(arr.Count() == 1);
			MG_COMMON_ASSERT(arr.GetCapacity() == 1);
			MG_COMMON_ASSERT(arr[0].myValue == -1);
			arr[0].myValue = 1;
			UTAValue::UseConstrCount(1);

			arr.SetCount(3);
			MG_COMMON_ASSERT(arr.Count() == 3);
			MG_COMMON_ASSERT(arr.GetCapacity() == 3);
			MG_COMMON_ASSERT(arr[0].myValue == 1);
			MG_COMMON_ASSERT(arr[1].myValue == -1);
			MG_COMMON_ASSERT(arr[2].myValue == -1);
			arr[1].myValue = 2;
			arr[2].myValue = 3;
			UTAValue::UseConstrCount(2);
			// Realloc.
			UTAValue::UseMoveConstrCount(1);
			UTAValue::UseDestrCount(1);

			arr.SetCount(5);
			MG_COMMON_ASSERT(arr.Count() == 5);
			MG_COMMON_ASSERT(arr.GetCapacity() == 6);
			MG_COMMON_ASSERT(arr[0].myValue == 1);
			MG_COMMON_ASSERT(arr[1].myValue == 2);
			MG_COMMON_ASSERT(arr[2].myValue == 3);
			MG_COMMON_ASSERT(arr[3].myValue == -1);
			MG_COMMON_ASSERT(arr[4].myValue == -1);
			arr[3].myValue = 4;
			arr[4].myValue = 5;
			UTAValue::UseConstrCount(2);
			// Realloc.
			UTAValue::UseMoveConstrCount(3);
			UTAValue::UseDestrCount(3);

			// SetCount can delete as well.
			arr.SetCount(2);
			MG_COMMON_ASSERT(arr.Count() == 2);
			MG_COMMON_ASSERT(arr.GetCapacity() == 6);
			MG_COMMON_ASSERT(arr[0].myValue == 1);
			MG_COMMON_ASSERT(arr[1].myValue == 2);
			UTAValue::UseDestrCount(3);

			arr.SetCount(0);
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 6);
			UTAValue::UseDestrCount(2);
		}
		UTAValue::CheckCounters();

		// All the same with plain type to ensure the policies
		// work correctly.
		{
			mg::common::Array<int> arr;

			arr.SetCount(0);
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 0);

			arr.SetCount(1);
			MG_COMMON_ASSERT(arr.Count() == 1);
			MG_COMMON_ASSERT(arr.GetCapacity() == 1);
			arr[0] = 1;

			arr.SetCount(3);
			MG_COMMON_ASSERT(arr.Count() == 3);
			MG_COMMON_ASSERT(arr.GetCapacity() == 3);
			MG_COMMON_ASSERT(arr[0] == 1);
			arr[1] = 2;
			arr[2] = 3;

			arr.SetCount(5);
			MG_COMMON_ASSERT(arr.Count() == 5);
			MG_COMMON_ASSERT(arr.GetCapacity() == 6);
			MG_COMMON_ASSERT(arr[0] == 1);
			MG_COMMON_ASSERT(arr[1] == 2);
			MG_COMMON_ASSERT(arr[2] == 3);
			arr[3] = 4;
			arr[4] = 5;

			arr.SetCount(2);
			MG_COMMON_ASSERT(arr.Count() == 2);
			MG_COMMON_ASSERT(arr.GetCapacity() == 6);
			MG_COMMON_ASSERT(arr[0] == 1);
			MG_COMMON_ASSERT(arr[1] == 2);

			arr.SetCount(0);
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 6);
		}
	}

	static void
	UnitTestArrayTruncate()
	{
		UTAValue::ResetCounters();
		{
			mg::common::Array<UTAValue> arr;

			// Truncate empty.
			arr.Truncate(0);
			MG_COMMON_ASSERT(arr.Count() == 0);
			arr.Truncate(1);
			MG_COMMON_ASSERT(arr.Count() == 0);

			// Truncate 1 to 1 or 0.
			arr.SetCount(1);
			MG_COMMON_ASSERT(arr.Count() == 1);
			UTAValue::UseConstrCount(1);

			arr.Truncate(1);
			MG_COMMON_ASSERT(arr.Count() == 1);

			arr.Truncate(0);
			MG_COMMON_ASSERT(arr.Count() == 0);
			UTAValue::UseDestrCount(1);

			// Truncate 1 to something bigger - nop.
			UTAValue::CheckCounters();
			arr.Add(UTAValue(1));
			UTAValue::ResetCounters();
			arr.Truncate(10);
			MG_COMMON_ASSERT(arr.Count() == 1);

			// Truncate 2 to 2, 1, or 0.
			UTAValue::CheckCounters();
			arr.Add(UTAValue(2));
			UTAValue::ResetCounters();
			arr.Truncate(2);
			MG_COMMON_ASSERT(arr.Count() == 2);

			arr.Truncate(1);
			UTAValue::UseDestrCount(1);
			MG_COMMON_ASSERT(arr.Count() == 1);
			MG_COMMON_ASSERT(arr[0].myValue == 1);

			UTAValue::CheckCounters();
			arr.Add(UTAValue(2));
			UTAValue::ResetCounters();
			arr.Truncate(0);
			UTAValue::UseDestrCount(2);
			MG_COMMON_ASSERT(arr.Count() == 0);
		}
		UTAValue::CheckCounters();

		// All the same with plain type to ensure the policies
		// work correctly.
		{
			mg::common::Array<int> arr;

			arr.Truncate(0);
			MG_COMMON_ASSERT(arr.Count() == 0);
			arr.Truncate(1);
			MG_COMMON_ASSERT(arr.Count() == 0);

			arr.SetCount(1);
			MG_COMMON_ASSERT(arr.Count() == 1);

			arr.Truncate(1);
			MG_COMMON_ASSERT(arr.Count() == 1);

			arr.Truncate(0);
			MG_COMMON_ASSERT(arr.Count() == 0);

			arr.Add(1);
			arr.Truncate(10);

			arr.Add(2);
			arr.Truncate(2);
			MG_COMMON_ASSERT(arr.Count() == 2);

			arr.Truncate(1);
			MG_COMMON_ASSERT(arr.Count() == 1);
			MG_COMMON_ASSERT(arr[0] == 1);

			arr.Add(2);
			arr.Truncate(0);
			MG_COMMON_ASSERT(arr.Count() == 0);
		}
	}

	static void
	UnitTestArrayIndexOf()
	{
		{
			mg::common::Array<UTAValue> arr;
			for (int i = 1; i <= 5; ++i)
				arr.Add(UTAValue(i));
			UTAValue v;
			UTAValue::ResetCounters();

			v.myValue = 6;
			MG_COMMON_ASSERT(arr.IndexOf(v) == -1);

			// Try to find all the elements.
			for (int i = 1; i <= 5; ++i)
			{
				v.myValue = i;
				MG_COMMON_ASSERT(arr.IndexOf(v) == i - 1);
			}
		}
		UTAValue::UseDestrCount(6);
		UTAValue::CheckCounters();

		// All the same with plain type to ensure the policies
		// work correctly.
		{
			mg::common::Array<int> arr;
			for (int i = 1; i <= 5; ++i)
				arr.Add(i);

			MG_COMMON_ASSERT(arr.IndexOf(6) == -1);

			for (int i = 1; i <= 5; ++i)
				MG_COMMON_ASSERT(arr.IndexOf(i) == i - 1);
		}

		// Ensure a non-comparable type can be used in the array.
		{
			struct NonComparable
			{
				int myValue;
			};

			mg::common::Array<NonComparable> arr;
			arr.SetCount(3);
			arr[0].myValue = 1;
			arr[1].myValue = 2;
			arr[2].myValue = 3;
		}
		// Comparator works even if the type is trivial.
		{
			struct TrivialComparable
			{
				inline bool
				operator==(
					const TrivialComparable& aOther) const
				{
					return aOther.myValue == myValue;
				}

				int myValue;
				int myValue2;
			};

			mg::common::Array<TrivialComparable> arr;
			arr.SetCount(3);
			arr[0].myValue = 1;
			arr[0].myValue2 = -1;
			arr[1].myValue = 2;
			arr[1].myValue2 = -1;
			arr[2].myValue = 3;
			arr[2].myValue2 = -1;

			TrivialComparable v;
			v.myValue = 2;
			v.myValue2 = 100;
			// It wouldn't be found by memcmp, because value2 is
			// different.
			MG_COMMON_ASSERT(arr.IndexOf(v) == 1);
			v.myValue = 4;
			MG_COMMON_ASSERT(arr.IndexOf(v) == -1);
		}
	}

	static void
	UnitTestArrayGet()
	{
		{
			mg::common::HybridArray<UTAValue, 3> arr;
			arr.Add(UTAValue(1));
			arr.Add(UTAValue(2));
			arr.Add(UTAValue(3));

			// Mutable get.
			MG_COMMON_ASSERT(arr.Get(0).myValue == 1);
			MG_COMMON_ASSERT(arr.Get(1).myValue == 2);
			MG_COMMON_ASSERT(arr.Get(2).myValue == 3);

			// Const get.
			const mg::common::Array<UTAValue>& arrRef = arr;
			MG_COMMON_ASSERT(arrRef.Get(0).myValue == 1);
			MG_COMMON_ASSERT(arrRef.Get(1).myValue == 2);
			MG_COMMON_ASSERT(arrRef.Get(2).myValue == 3);

			// Works as reference.
			arr.Get(0) = UTAValue(10);
			arr.Get(1) = UTAValue(20);
			arr.Get(2) = UTAValue(30);
			MG_COMMON_ASSERT(arr.Get(0).myValue == 10);
			MG_COMMON_ASSERT(arr.Get(1).myValue == 20);
			MG_COMMON_ASSERT(arr.Get(2).myValue == 30);
		}

		// All the same with plain type to ensure the policies
		// work correctly.
		{
			mg::common::HybridArray<int, 3> arr;
			arr.Add(1);
			arr.Add(2);
			arr.Add(3);

			MG_COMMON_ASSERT(arr.Get(0) == 1);
			MG_COMMON_ASSERT(arr.Get(1) == 2);
			MG_COMMON_ASSERT(arr.Get(2) == 3);

			const mg::common::Array<int>& arrRef = arr;
			MG_COMMON_ASSERT(arrRef.Get(0) == 1);
			MG_COMMON_ASSERT(arrRef.Get(1) == 2);
			MG_COMMON_ASSERT(arrRef.Get(2) == 3);

			arr.Get(0) = 10;
			arr.Get(1) = 20;
			arr.Get(2) = 30;
			MG_COMMON_ASSERT(arr.Get(0) == 10);
			MG_COMMON_ASSERT(arr.Get(1) == 20);
			MG_COMMON_ASSERT(arr.Get(2) == 30);
		}
	}

	static void
	UnitTestArrayOperatorAssignCopy()
	{
		UTAValue::ResetCounters();
		// Empty = empty.
		{
			mg::common::Array<UTAValue> arr1;
			mg::common::Array<UTAValue> arr2;
			arr1 = arr2;
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);

			arr1.Add(UTAValue(1));
			arr2.Add(UTAValue(2));
			UTAValue::UseConstrCount(2);
			UTAValue::UseMoveConstrCount(2);
			UTAValue::UseDestrCount(2);

			MG_COMMON_ASSERT(arr1.Count() == 1);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 1);
			MG_COMMON_ASSERT(arr2.Count() == 1);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 1);
			MG_COMMON_ASSERT(arr1[0].myValue == 1);
			MG_COMMON_ASSERT(arr2[0].myValue == 2);

			arr1.RemoveAll();
			UTAValue::UseDestrCount(1);
			arr2.RemoveAll();
			UTAValue::UseDestrCount(1);

			// Ensure that assignment does not make the arrays
			// share their buffer.
			arr1 = arr2;
			MG_COMMON_ASSERT(arr1.GetBuffer() != arr2.GetBuffer());
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 1);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 1);
		}
		UTAValue::CheckCounters();

		// Empty = non empty.
		{
			mg::common::Array<UTAValue> arr1;
			mg::common::Array<UTAValue> arr2;
			for (int i = 0; i < 3; ++i)
				arr2.Add(UTAValue(i));
			UTAValue::ResetCounters();

			arr1 = arr2;
			MG_COMMON_ASSERT(arr1.Count() == 3);
			// 3, not 4, because reserved exactly how many was
			// needed.
			MG_COMMON_ASSERT(arr1.GetCapacity() == 3);
			MG_COMMON_ASSERT(arr2.Count() == 3);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 4);
			for (int i = 0; i < 3; ++i)
			{
				MG_COMMON_ASSERT(arr1[i].myValue == i);
				MG_COMMON_ASSERT(arr2[i].myValue == i);
			}
			UTAValue::UseCopyConstrCount(3);

			arr1.Add(UTAValue(4));
			UTAValue::UseMoveConstrCount(1);
			// Temporary value.
			UTAValue::UseConstrCount(1);
			UTAValue::UseDestrCount(1);
			// Realloc.
			UTAValue::UseMoveConstrCount(3);
			UTAValue::UseDestrCount(3);

			arr2.Add(UTAValue(5));
			UTAValue::UseMoveConstrCount(1);
			// Temporary value.
			UTAValue::UseConstrCount(1);
			UTAValue::UseDestrCount(1);

			MG_COMMON_ASSERT(arr1.Count() == 4);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 6);
			MG_COMMON_ASSERT(arr2.Count() == 4);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 4);
			for (int i = 0; i < 3; ++i)
			{
				MG_COMMON_ASSERT(arr1[i].myValue == i);
				MG_COMMON_ASSERT(arr2[i].myValue == i);
			}
			MG_COMMON_ASSERT(arr1[3].myValue == 4);
			MG_COMMON_ASSERT(arr2[3].myValue == 5);
		}
		UTAValue::UseDestrCount(8);
		UTAValue::CheckCounters();

		// Non empty = empty.
		{
			mg::common::Array<UTAValue> arr1;
			mg::common::Array<UTAValue> arr2;
			for (int i = 0; i < 3; ++i)
				arr1.Add(UTAValue(i));
			UTAValue::ResetCounters();

			MG_COMMON_ASSERT(arr1.Count() == 3);
			arr1 = arr2;
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 4);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
			UTAValue::UseDestrCount(3);
		}
		UTAValue::CheckCounters();

		// Assign bigger array to smaller array. Some items
		// should be copied, some copy-constructed.
		{
			mg::common::Array<UTAValue> arr1;
			mg::common::Array<UTAValue> arr2;
			for (int v = 0; v < 3; ++v)
				arr1.Add(UTAValue(v));
			for (int v = 3; v < 8; ++v)
				arr2.Add(UTAValue(v));
			UTAValue::ResetCounters();

			arr1 = arr2;
			MG_COMMON_ASSERT(arr1.Count() == 5);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 8);
			MG_COMMON_ASSERT(arr2.Count() == 5);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 8);
			for (int i = 0, v = 3; i < 5; ++i, ++v)
			{
				MG_COMMON_ASSERT(arr1[i].myValue == v);
				MG_COMMON_ASSERT(arr2[i].myValue == v);
			}
			// Copied values.
			UTAValue::UseCopyAssignCount(3);
			// 2 values in the source array are copy-constructed
			// into the destination array, because these places
			// were not constructed here before.
			UTAValue::UseCopyConstrCount(2);
			// The destination array was reallocated before
			// assigned the source array, due to lack of space.
			UTAValue::UseMoveConstrCount(3);
			UTAValue::UseDestrCount(3);
		}
		UTAValue::UseDestrCount(10);
		UTAValue::CheckCounters();

		// Assign smaller array to bigger array. All items should
		// be copied, some old ones should be destroyed.
		{
			mg::common::Array<UTAValue> arr1;
			mg::common::Array<UTAValue> arr2;
			for (int i = 0; i < 3; ++i)
				arr2.Add(UTAValue(i));
			for (int i = 3; i < 8; ++i)
				arr1.Add(UTAValue(i));
			UTAValue::ResetCounters();

			arr1 = arr2;
			MG_COMMON_ASSERT(arr1.Count() == 3);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 8);
			MG_COMMON_ASSERT(arr2.Count() == 3);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 4);
			for (int i = 0; i < 3; ++i)
			{
				MG_COMMON_ASSERT(arr1[i].myValue == i);
				MG_COMMON_ASSERT(arr2[i].myValue == i);
			}
			UTAValue::UseCopyAssignCount(3);
			UTAValue::UseDestrCount(2);
		}
		UTAValue::UseDestrCount(6);
		UTAValue::CheckCounters();
#if !IS_PLATFORM_POSIX
		// On Linux it does not compile - the compiler warns about
		// self-assign.

		// Self copy is nop.
		{
			mg::common::Array<UTAValue> arr;
			for (int i = 0; i < 3; ++i)
				arr.Add(UTAValue(i));
			UTAValue::ResetCounters();

			arr = arr;
			MG_COMMON_ASSERT(arr.Count() == 3);
			MG_COMMON_ASSERT(arr.GetCapacity() == 4);
			for (int i = 0; i < 3; ++i)
				MG_COMMON_ASSERT(arr[i].myValue == i);
		}
		UTAValue::UseDestrCount(3);
		UTAValue::CheckCounters();
#endif
		// All the same with plain type to ensure the policies
		// work correctly.
		{
			mg::common::Array<int> arr1;
			mg::common::Array<int> arr2;
			arr1 = arr2;
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);

			arr1.Add(1);
			arr2.Add(2);
			MG_COMMON_ASSERT(arr1.Count() == 1);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 1);
			MG_COMMON_ASSERT(arr2.Count() == 1);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 1);
			MG_COMMON_ASSERT(arr1[0] == 1);
			MG_COMMON_ASSERT(arr2[0] == 2);

			arr1.RemoveAll();
			arr2.RemoveAll();

			arr1 = arr2;
			MG_COMMON_ASSERT(arr1.GetBuffer() != arr2.GetBuffer());
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 1);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 1);
		}
		{
			mg::common::Array<int> arr1;
			mg::common::Array<int> arr2;
			for (int i = 0; i < 3; ++i)
				arr2.Add(i);

			arr1 = arr2;
			MG_COMMON_ASSERT(arr1.Count() == 3);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 3);
			MG_COMMON_ASSERT(arr2.Count() == 3);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 4);
			for (int i = 0; i < 3; ++i)
			{
				MG_COMMON_ASSERT(arr1[i] == i);
				MG_COMMON_ASSERT(arr2[i] == i);
			}

			arr1.Add(4);
			arr2.Add(5);
			MG_COMMON_ASSERT(arr1.Count() == 4);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 6);
			MG_COMMON_ASSERT(arr2.Count() == 4);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 4);
			for (int i = 0; i < 3; ++i)
			{
				MG_COMMON_ASSERT(arr1[i] == i);
				MG_COMMON_ASSERT(arr2[i] == i);
			}
			MG_COMMON_ASSERT(arr1[3] == 4);
			MG_COMMON_ASSERT(arr2[3] == 5);
		}
		{
			mg::common::Array<int> arr1;
			mg::common::Array<int> arr2;
			for (int i = 0; i < 3; ++i)
				arr1.Add(i);

			MG_COMMON_ASSERT(arr1.Count() == 3);
			arr1 = arr2;
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 4);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
		}
		{
			mg::common::Array<int> arr1;
			mg::common::Array<int> arr2;
			for (int v = 0; v < 3; ++v)
				arr1.Add(v);
			for (int v = 3; v < 8; ++v)
				arr2.Add(v);

			arr1 = arr2;
			MG_COMMON_ASSERT(arr1.Count() == 5);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 8);
			MG_COMMON_ASSERT(arr2.Count() == 5);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 8);
			for (int i = 0, v = 3; i < 5; ++i, ++v)
			{
				MG_COMMON_ASSERT(arr1[i] == v);
				MG_COMMON_ASSERT(arr2[i] == v);
			}
		}
		{
			mg::common::Array<int> arr1;
			mg::common::Array<int> arr2;
			for (int i = 0; i < 3; ++i)
				arr2.Add(i);
			for (int i = 3; i < 8; ++i)
				arr1.Add(i);

			arr1 = arr2;
			MG_COMMON_ASSERT(arr1.Count() == 3);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 8);
			MG_COMMON_ASSERT(arr2.Count() == 3);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 4);
			for (int i = 0; i < 3; ++i)
			{
				MG_COMMON_ASSERT(arr1[i] == i);
				MG_COMMON_ASSERT(arr2[i] == i);
			}
		}
#if !IS_PLATFORM_POSIX
		{
			mg::common::Array<int> arr;
			for (int i = 0; i < 3; ++i)
				arr.Add(i);

			arr = arr;
			MG_COMMON_ASSERT(arr.Count() == 3);
			MG_COMMON_ASSERT(arr.GetCapacity() == 4);
			for (int i = 0; i < 3; ++i)
				MG_COMMON_ASSERT(arr[i] == i);
		}
#endif
	}

	static void
	UnitTestArrayOperatorAssignCopyVariants()
	{
		UTAArrayVariants* vars = UnitTestArrayCreateVariants();

		int varCount = vars->Count();
		for (int i = 0; i < varCount; ++i)
		{
			for (int j = 0; j < varCount; ++j)
			{
				mg::common::Array<UTAValue>* arr1 = vars->Get(i)();
				mg::common::Array<UTAValue>* arr2 = vars->Get(j)();

				uint32_t cap1 = arr1->GetCapacity();
				uint32_t stat1 = arr1->GetStaticCapacity();
				UTAValue* buf1 = arr1->GetBuffer();
				bool isDyn1 = arr1->IsDynamic();

				uint32_t cnt2 = arr2->Count();
				uint32_t cap2 = arr2->GetCapacity();
				uint32_t stat2 = arr2->GetStaticCapacity();
				UTAValue* buf2 = arr2->GetBuffer();
				bool isDyn2 = arr2->IsDynamic();

				*arr1 = *arr2;

				MG_COMMON_ASSERT(arr1->GetBuffer() != arr2->GetBuffer() ||
					arr1->GetBuffer() == nullptr);

				MG_COMMON_ASSERT(arr1->Count() == cnt2);
				MG_COMMON_ASSERT(arr1->GetStaticCapacity() == stat1);
				MG_COMMON_ASSERT(arr1->GetBuffer() != buf2 || buf2 == nullptr);
				MG_COMMON_ASSERT(arr2->IsDynamic() == isDyn2);
				MG_COMMON_ASSERT(arr2->Count() == cnt2);
				MG_COMMON_ASSERT(arr2->GetBuffer() == buf2);
				MG_COMMON_ASSERT(arr2->GetCapacity() == cap2);
				MG_COMMON_ASSERT(arr2->GetStaticCapacity() == stat2);

				if (cap1 >= cnt2)
				{
					MG_COMMON_ASSERT(arr1->IsDynamic() == isDyn1);
					MG_COMMON_ASSERT(arr1->GetBuffer() == buf1);
					MG_COMMON_ASSERT(arr1->GetCapacity() == cap1);
				}
				else
				{
					MG_COMMON_ASSERT(arr1->IsDynamic());
					MG_COMMON_ASSERT(arr1->GetCapacity() >= cnt2);
				}

				for (uint32_t k = 0; k < cnt2; ++k)
					MG_COMMON_ASSERT(arr1->Get(k).myValue == (int)k);
				for (uint32_t k = 0; k < cnt2; ++k)
					MG_COMMON_ASSERT(arr2->Get(k).myValue == (int)k);

				const int count = 10;
				for (int k = arr1->Count(); k < count; ++k)
					arr1->Add(UTAValue(k));
				for (int k = arr2->Count(); k < count; ++k)
					arr2->Add(UTAValue(k));

				MG_COMMON_ASSERT(arr1->Count() == count);
				MG_COMMON_ASSERT(arr1->GetCapacity() >= count);
				MG_COMMON_ASSERT(arr1->GetStaticCapacity() == stat1);

				MG_COMMON_ASSERT(arr2->Count() == count);
				MG_COMMON_ASSERT(arr2->GetCapacity() >= count);
				MG_COMMON_ASSERT(arr2->GetStaticCapacity() == stat2);

				MG_COMMON_ASSERT(arr1->GetBuffer() != arr2->GetBuffer());

				for (int k = 0; k < count; ++k)
				{
					MG_COMMON_ASSERT(arr1->Get(k).myValue == k);
					MG_COMMON_ASSERT(arr2->Get(k).myValue == k);
				}

				delete arr1;
				delete arr2;
			}
		}

		delete vars;
	}

	static void
	UnitTestArrayOperatorAssignMove()
	{
		UTAValue::ResetCounters();
		// Empty = empty.
		{
			mg::common::Array<UTAValue> arr1;
			mg::common::Array<UTAValue> arr2;
			arr1 = mg::common::Move(arr2);
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr1.GetBuffer() == nullptr);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
		}
		UTAValue::CheckCounters();

		// Empty = non empty.
		{
			mg::common::Array<UTAValue> arr1;
			mg::common::Array<UTAValue> arr2;
			for (int i = 0; i < 3; ++i)
				arr2.Add(UTAValue(i));
			UTAValue::ResetCounters();

			MG_COMMON_ASSERT(arr2.Count() == 3);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 4);
			void* buf2 = arr2.GetBuffer();

			arr1 = mg::common::Move(arr2);
			MG_COMMON_ASSERT(arr1.Count() == 3);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 4);
			MG_COMMON_ASSERT(arr1.GetBuffer() == buf2);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
			for (int i = 0; i < 3; ++i)
				MG_COMMON_ASSERT(arr1[i].myValue == i);
		}
		UTAValue::UseDestrCount(3);
		UTAValue::CheckCounters();

		// Non empty = empty with no buffer.
		{
			mg::common::Array<UTAValue> arr1;
			mg::common::Array<UTAValue> arr2;
			for (int i = 0; i < 3; ++i)
				arr1.Add(UTAValue(i));
			UTAValue::ResetCounters();

			MG_COMMON_ASSERT(arr1.Count() == 3);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 4);
			MG_COMMON_ASSERT(arr1.GetBuffer() != nullptr);

			arr1 = mg::common::Move(arr2);
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr1.GetBuffer() == nullptr);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
			UTAValue::UseDestrCount(3);
		}
		UTAValue::CheckCounters();

		// Non empty = empty with an unused buffer.
		{
			mg::common::Array<UTAValue> arr1;
			mg::common::Array<UTAValue> arr2;
			for (int i = 0; i < 3; ++i)
				arr1.Add(UTAValue(i));
			arr2.Add(UTAValue(100));
			arr2.RemoveAll();
			UTAValue::ResetCounters();

			MG_COMMON_ASSERT(arr1.Count() == 3);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 4);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 1);
			void* buf2 = arr2.GetBuffer();
			MG_COMMON_ASSERT(buf2 != nullptr);

			arr1 = mg::common::Move(arr2);
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 1);
			MG_COMMON_ASSERT(arr1.GetBuffer() == buf2);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
			UTAValue::UseDestrCount(3);
		}
		UTAValue::CheckCounters();

		// Non empty = non empty.
		{
			mg::common::Array<UTAValue> arr1;
			mg::common::Array<UTAValue> arr2;
			for (int v = 0; v < 3; ++v)
				arr1.Add(UTAValue(v));
			for (int v = 3; v < 8; ++v)
				arr2.Add(UTAValue(v));
			UTAValue::ResetCounters();

			MG_COMMON_ASSERT(arr1.Count() == 3);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 4);
			MG_COMMON_ASSERT(arr2.Count() == 5);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 8);
			void* buf2 = arr2.GetBuffer();

			arr1 = mg::common::Move(arr2);
			MG_COMMON_ASSERT(arr1.Count() == 5);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 8);
			MG_COMMON_ASSERT(arr1.GetBuffer() == buf2);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
			for (int i = 0, v = 3; i < 5; ++i, ++v)
				MG_COMMON_ASSERT(arr1[i].myValue == v);
			UTAValue::UseDestrCount(3);
		}
		UTAValue::UseDestrCount(5);
		UTAValue::CheckCounters();

		// Move static size1 into size2, size2 < size1.
		{
			mg::common::HybridArray<UTAValue, 10> arr1;
			mg::common::Array<UTAValue> arr2;
			for (int v = 0; v < 5; ++v)
				arr1.Add(UTAValue(v));
			for (int v = 5; v < 8; ++v)
				arr2.Add(UTAValue(v));
			UTAValue::ResetCounters();

			void* buf1 = arr1.GetBuffer();
			arr2 = mg::common::Move(arr1);
			// Destroy the old place.
			UTAValue::UseDestrCount(5);
			// Move into existing places.
			UTAValue::UseMoveAssignCount(3);
			// Construct new places.
			UTAValue::UseMoveConstrCount(2);
			// Target is reallocated before the move.
			UTAValue::UseMoveConstrCount(3);
			UTAValue::UseDestrCount(3);

			MG_COMMON_ASSERT(arr1.GetBuffer() == nullptr);
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 0);

			MG_COMMON_ASSERT(arr2.GetBuffer() != buf1);
			MG_COMMON_ASSERT(arr2.Count() == 5);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 8);
		}
		UTAValue::UseDestrCount(5);
		UTAValue::CheckCounters();

		// Move static size1 into size2, size2 = size1.
		{
			mg::common::HybridArray<UTAValue, 10> arr1;
			mg::common::Array<UTAValue> arr2;
			for (int v = 0; v < 4; ++v)
				arr1.Add(UTAValue(v));
			for (int v = 4; v < 8; ++v)
				arr2.Add(UTAValue(v));
			UTAValue::ResetCounters();

			void* buf1 = arr1.GetBuffer();
			arr2 = mg::common::Move(arr1);
			// Destroy the old place.
			UTAValue::UseDestrCount(4);
			// Move into existing places.
			UTAValue::UseMoveAssignCount(4);

			MG_COMMON_ASSERT(arr1.GetBuffer() == nullptr);
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 0);

			MG_COMMON_ASSERT(arr2.GetBuffer() != buf1);
			MG_COMMON_ASSERT(arr2.Count() == 4);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 4);
		}
		UTAValue::UseDestrCount(4);
		UTAValue::CheckCounters();

		// Move static size1 into size2, size2 > size1.
		{
			mg::common::HybridArray<UTAValue, 10> arr1;
			mg::common::Array<UTAValue> arr2;
			for (int v = 0; v < 3; ++v)
				arr1.Add(UTAValue(v));
			for (int v = 3; v < 8; ++v)
				arr2.Add(UTAValue(v));
			UTAValue::ResetCounters();

			void* buf1 = arr1.GetBuffer();
			arr2 = mg::common::Move(arr1);
			// Destroy the old place.
			UTAValue::UseDestrCount(3);
			// Move into existing places.
			UTAValue::UseMoveAssignCount(3);
			// Destroy no more used places.
			UTAValue::UseDestrCount(2);

			MG_COMMON_ASSERT(arr1.GetBuffer() == nullptr);
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 0);

			MG_COMMON_ASSERT(arr2.GetBuffer() != buf1);
			MG_COMMON_ASSERT(arr2.Count() == 3);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 8);
		}
		UTAValue::UseDestrCount(3);
		UTAValue::CheckCounters();

		// All the same with plain type to ensure the policies
		// work correctly.
		{
			mg::common::Array<int> arr1;
			mg::common::Array<int> arr2;
			arr1 = mg::common::Move(arr2);
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr1.GetBuffer() == nullptr);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
		}
		{
			mg::common::Array<int> arr1;
			mg::common::Array<int> arr2;
			for (int i = 0; i < 3; ++i)
				arr2.Add(i);

			MG_COMMON_ASSERT(arr2.Count() == 3);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 4);
			void* buf2 = arr2.GetBuffer();

			arr1 = mg::common::Move(arr2);
			MG_COMMON_ASSERT(arr1.Count() == 3);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 4);
			MG_COMMON_ASSERT(arr1.GetBuffer() == buf2);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
			for (int i = 0; i < 3; ++i)
				MG_COMMON_ASSERT(arr1[i] == i);
		}
		{
			mg::common::Array<int> arr1;
			mg::common::Array<int> arr2;
			for (int i = 0; i < 3; ++i)
				arr1.Add(i);

			MG_COMMON_ASSERT(arr1.Count() == 3);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 4);
			MG_COMMON_ASSERT(arr1.GetBuffer() != nullptr);

			arr1 = mg::common::Move(arr2);
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr1.GetBuffer() == nullptr);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
		}
		{
			mg::common::Array<int> arr1;
			mg::common::Array<int> arr2;
			for (int i = 0; i < 3; ++i)
				arr1.Add(i);
			arr2.Add(100);
			arr2.RemoveAll();
			UTAValue::ResetCounters();

			MG_COMMON_ASSERT(arr1.Count() == 3);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 4);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 1);
			void* buf2 = arr2.GetBuffer();
			MG_COMMON_ASSERT(buf2 != nullptr);

			arr1 = mg::common::Move(arr2);
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 1);
			MG_COMMON_ASSERT(arr1.GetBuffer() == buf2);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
		}
		{
			mg::common::Array<int> arr1;
			mg::common::Array<int> arr2;
			for (int v = 0; v < 3; ++v)
				arr1.Add(v);
			for (int v = 3; v < 8; ++v)
				arr2.Add(v);

			MG_COMMON_ASSERT(arr1.Count() == 3);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 4);
			MG_COMMON_ASSERT(arr2.Count() == 5);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 8);
			void* buf2 = arr2.GetBuffer();

			arr1 = mg::common::Move(arr2);
			MG_COMMON_ASSERT(arr1.Count() == 5);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 8);
			MG_COMMON_ASSERT(arr1.GetBuffer() == buf2);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
			for (int i = 0, v = 3; i < 5; ++i, ++v)
				MG_COMMON_ASSERT(arr1[i] == v);
		}
		{
			mg::common::HybridArray<int, 10> arr1;
			mg::common::Array<int> arr2;
			for (int v = 0; v < 5; ++v)
				arr1.Add(v);
			for (int v = 5; v < 8; ++v)
				arr2.Add(v);

			void* buf1 = arr1.GetBuffer();
			arr2 = mg::common::Move(arr1);

			MG_COMMON_ASSERT(arr1.GetBuffer() == nullptr);
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 0);

			MG_COMMON_ASSERT(arr2.GetBuffer() != buf1);
			MG_COMMON_ASSERT(arr2.Count() == 5);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 8);
		}
		{
			mg::common::HybridArray<int, 10> arr1;
			mg::common::Array<int> arr2;
			for (int v = 0; v < 4; ++v)
				arr1.Add(v);
			for (int v = 4; v < 8; ++v)
				arr2.Add(v);

			void* buf1 = arr1.GetBuffer();
			arr2 = mg::common::Move(arr1);

			MG_COMMON_ASSERT(arr1.GetBuffer() == nullptr);
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 0);

			MG_COMMON_ASSERT(arr2.GetBuffer() != buf1);
			MG_COMMON_ASSERT(arr2.Count() == 4);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 4);
		}
		{
			mg::common::HybridArray<int, 10> arr1;
			mg::common::Array<int> arr2;
			for (int v = 0; v < 3; ++v)
				arr1.Add(v);
			for (int v = 3; v < 8; ++v)
				arr2.Add(v);

			void* buf1 = arr1.GetBuffer();
			arr2 = mg::common::Move(arr1);

			MG_COMMON_ASSERT(arr1.GetBuffer() == nullptr);
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 0);

			MG_COMMON_ASSERT(arr2.GetBuffer() != buf1);
			MG_COMMON_ASSERT(arr2.Count() == 3);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 8);
		}
	}

	static void
	UnitTestArrayOperatorAssignMoveVariants()
	{
		UTAArrayVariants* vars = UnitTestArrayCreateVariants();

		int varCount = vars->Count();
		for (int i = 0; i < varCount; ++i)
		{
			for (int j = 0; j < varCount; ++j)
			{
				mg::common::Array<UTAValue>* arr1 = vars->Get(i)();
				mg::common::Array<UTAValue>* arr2 = vars->Get(j)();

				uint32_t cnt1 = arr1->Count();
				uint32_t cap1 = arr1->GetCapacity();
				uint32_t stat1 = arr1->GetStaticCapacity();
				UTAValue* buf1 = arr1->GetBuffer();
				bool isDyn1 = arr1->IsDynamic();

				uint32_t cnt2 = arr2->Count();
				uint32_t cap2 = arr2->GetCapacity();
				uint32_t stat2 = arr2->GetStaticCapacity();
				UTAValue* buf2 = arr2->GetBuffer();
				bool isDyn2 = arr2->IsDynamic();

				UTAValue::ResetCounters();
				*arr1 = mg::common::Move(*arr2);
				UTAValue::UseDestrCount(cnt1);

				MG_COMMON_ASSERT(arr1->GetBuffer() != arr2->GetBuffer() ||
					arr1->GetBuffer() == nullptr);

				MG_COMMON_ASSERT(arr1->Count() == cnt2);
				MG_COMMON_ASSERT(arr1->GetStaticCapacity() == stat1);
				MG_COMMON_ASSERT(arr2->Count() == 0);
				MG_COMMON_ASSERT(arr2->GetStaticCapacity() == stat2);
				MG_COMMON_ASSERT(arr2->GetBuffer() == nullptr);
				MG_COMMON_ASSERT(arr2->GetCapacity() == 0);

				if (isDyn1 && isDyn2)
				{
					MG_COMMON_ASSERT(arr1->GetBuffer() == buf2);
					MG_COMMON_ASSERT(arr1->GetCapacity() == cap2);
				}
				else if (!isDyn1 && !isDyn2)
				{
					if (cap1 >= cnt2)
					{
						MG_COMMON_ASSERT(!arr1->IsDynamic());
						MG_COMMON_ASSERT(arr1->GetBuffer() == buf1);
						MG_COMMON_ASSERT(arr1->GetCapacity() == cap1);
					}
					else if (cap1 < cnt2)
					{
						MG_COMMON_ASSERT(arr1->IsDynamic());
						MG_COMMON_ASSERT(arr1->GetBuffer() != buf1);
						MG_COMMON_ASSERT(arr1->GetCapacity() == cnt2);
					}
				}
				else if (isDyn1 && !isDyn2)
				{
					MG_COMMON_ASSERT(arr1->IsDynamic());
					if (cap1 >= cnt2)
						MG_COMMON_ASSERT(arr1->GetBuffer() == buf1);
					MG_COMMON_ASSERT(arr1->GetCapacity() >= cnt2);
				}
				else if (isDyn2 && !isDyn1)
				{
					MG_COMMON_ASSERT(arr1->IsDynamic());
					MG_COMMON_ASSERT(arr1->GetBuffer() == buf2);
					MG_COMMON_ASSERT(arr1->GetCapacity() == cap2);
				}
				else
				{
					MG_COMMON_ASSERT(false);
				}

				for (uint32_t k = 0; k < cnt2; ++k)
					MG_COMMON_ASSERT(arr1->Get(k).myValue == (int)k);

				const int count = 10;
				for (int k = arr1->Count(); k < count; ++k)
					arr1->Add(UTAValue(k));
				for (int k = arr2->Count(); k < count; ++k)
					arr2->Add(UTAValue(k));

				MG_COMMON_ASSERT(arr1->Count() == count);
				MG_COMMON_ASSERT(arr1->GetCapacity() >= count);
				MG_COMMON_ASSERT(arr1->GetStaticCapacity() == stat1);

				MG_COMMON_ASSERT(arr2->Count() == count);
				MG_COMMON_ASSERT(arr2->GetCapacity() >= count);
				MG_COMMON_ASSERT(arr2->GetStaticCapacity() == stat2);

				MG_COMMON_ASSERT(arr1->GetBuffer() != arr2->GetBuffer());

				for (int k = 0; k < count; ++k)
				{
					MG_COMMON_ASSERT(arr1->Get(k).myValue == k);
					MG_COMMON_ASSERT(arr2->Get(k).myValue == k);
				}

				delete arr1;
				delete arr2;
			}
		}

		delete vars;
	}

	static void
	UnitTestArrayLast()
	{
		{
			mg::common::Array<UTAValue> arr;
			const mg::common::Array<UTAValue>& ref = arr;

			arr.Add(UTAValue(1));
			UTAValue::ResetCounters();
			MG_COMMON_ASSERT(arr.Last().myValue == 1);
			MG_COMMON_ASSERT(ref.Last().myValue == 1);
			UTAValue::CheckCounters();

			arr.Add(UTAValue(2));
			UTAValue::ResetCounters();
			MG_COMMON_ASSERT(arr.Last().myValue == 2);
			MG_COMMON_ASSERT(ref.Last().myValue == 2);
		}
		UTAValue::UseDestrCount(2);
		UTAValue::CheckCounters();

		{
			mg::common::Array<int> arr;
			const mg::common::Array<int>& ref = arr;
			arr.Add(1);
			MG_COMMON_ASSERT(arr.Last() == 1);
			MG_COMMON_ASSERT(ref.Last() == 1);
			arr.Add(2);
			MG_COMMON_ASSERT(arr.Last() == 2);
			MG_COMMON_ASSERT(ref.Last() == 2);
		}
	}

	void
	UnitTestArrayConstructorCopy()
	{
		UTAValue::ResetCounters();
		// From empty.
		{
			mg::common::Array<UTAValue> arr2;
			mg::common::Array<UTAValue> arr1(arr2);
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
		}
		UTAValue::CheckCounters();

		// From non empty.
		{
			mg::common::Array<UTAValue> arr2;
			for (int i = 0; i < 3; ++i)
				arr2.Add(UTAValue(i));
			UTAValue::ResetCounters();

			mg::common::Array<UTAValue> arr1(arr2);
			MG_COMMON_ASSERT(arr1.Count() == 3);
			// 3, not 4, because reserved exactly how many was
			// needed.
			MG_COMMON_ASSERT(arr1.GetCapacity() == 3);
			MG_COMMON_ASSERT(arr2.Count() == 3);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 4);
			for (int i = 0; i < 3; ++i)
			{
				MG_COMMON_ASSERT(arr1[i].myValue == i);
				MG_COMMON_ASSERT(arr2[i].myValue == i);
			}
			UTAValue::UseCopyConstrCount(3);
		}
		UTAValue::UseDestrCount(6);
		UTAValue::CheckCounters();

		// All the same with plain type to ensure the policies
		// work correctly.
		{
			mg::common::Array<int> arr2;
			mg::common::Array<int> arr1(arr2);
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
		}
		{
			mg::common::Array<int> arr2;
			for (int i = 0; i < 3; ++i)
				arr2.Add(i);

			mg::common::Array<int> arr1(arr2);
			MG_COMMON_ASSERT(arr1.Count() == 3);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 3);
			MG_COMMON_ASSERT(arr2.Count() == 3);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 4);
			for (int i = 0; i < 3; ++i)
			{
				MG_COMMON_ASSERT(arr1[i] == i);
				MG_COMMON_ASSERT(arr2[i] == i);
			}
		}
	}

	static void
	UnitTestArrayConstructorMove()
	{
		UTAValue::ResetCounters();
		// From empty.
		{
			mg::common::Array<UTAValue> arr2;
			mg::common::Array<UTAValue> arr1(mg::common::Move(arr2));
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr1.GetBuffer() == nullptr);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
		}
		UTAValue::CheckCounters();

		// From non empty.
		{
			mg::common::Array<UTAValue> arr2;
			for (int i = 0; i < 3; ++i)
				arr2.Add(UTAValue(i));
			UTAValue::ResetCounters();

			MG_COMMON_ASSERT(arr2.Count() == 3);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 4);
			void* buf2 = arr2.GetBuffer();

			mg::common::Array<UTAValue> arr1(mg::common::Move(arr2));
			MG_COMMON_ASSERT(arr1.Count() == 3);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 4);
			MG_COMMON_ASSERT(arr1.GetBuffer() == buf2);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
			for (int i = 0; i < 3; ++i)
				MG_COMMON_ASSERT(arr1[i].myValue == i);
		}
		UTAValue::UseDestrCount(3);
		UTAValue::CheckCounters();

		// All the same with plain type to ensure the policies
		// work correctly.
		{
			mg::common::Array<int> arr2;
			mg::common::Array<int> arr1(mg::common::Move(arr2));
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr1.GetBuffer() == nullptr);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
		}
		{
			mg::common::Array<int> arr2;
			for (int i = 0; i < 3; ++i)
				arr2.Add(i);

			MG_COMMON_ASSERT(arr2.Count() == 3);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 4);
			void* buf2 = arr2.GetBuffer();

			mg::common::Array<int> arr1(mg::common::Move(arr2));
			MG_COMMON_ASSERT(arr1.Count() == 3);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 4);
			MG_COMMON_ASSERT(arr1.GetBuffer() == buf2);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
			for (int i = 0; i < 3; ++i)
				MG_COMMON_ASSERT(arr1[i] == i);
		}
	}

	static void
	UnitTestArrayIterator()
	{
		UTAValue::ResetCounters();
		{
			mg::common::HybridArray<UTAValue, 2> arr;

			for (const UTAValue& val : arr)
			{
				MG_UNUSED(val);
				MG_COMMON_ASSERT(false);
			}
			for (UTAValue& val : arr)
			{
				MG_UNUSED(val);
				MG_COMMON_ASSERT(false);
			}

			arr.Add(UTAValue(1));
			UTAValue::UseConstrCount(1);
			UTAValue::UseMoveConstrCount(1);
			UTAValue::UseDestrCount(1);

			int count = 0;
			for (const UTAValue& val : arr)
			{
				MG_COMMON_ASSERT(val.myValue == count + 1);
				++count;
			}
			MG_COMMON_ASSERT(count == 1);

			count = 0;
			for (UTAValue& val : arr)
			{
				MG_COMMON_ASSERT(val.myValue == count + 1);
				val.myValue *= 10;
				++count;
			}
			MG_COMMON_ASSERT(count == 1);

			count = 0;
			for (UTAValue& val : arr)
				MG_COMMON_ASSERT(val.myValue == ++count * 10);

			arr[0] = UTAValue(1);
			UTAValue::UseConstrCount(1);
			UTAValue::UseMoveAssignCount(1);
			UTAValue::UseDestrCount(1);

			arr.Add(UTAValue(2));
			UTAValue::UseConstrCount(1);
			UTAValue::UseMoveConstrCount(1);
			UTAValue::UseDestrCount(1);

			count = 0;
			for (const UTAValue& val : arr)
			{
				MG_COMMON_ASSERT(val.myValue == count + 1);
				++count;
			}
			MG_COMMON_ASSERT(count == 2);

			count = 0;
			for (UTAValue& val : arr)
			{
				MG_COMMON_ASSERT(val.myValue == count + 1);
				val.myValue *= 10;
				++count;
			}
			MG_COMMON_ASSERT(count == 2);

			count = 0;
			for (UTAValue& val : arr)
				MG_COMMON_ASSERT(val.myValue == ++count * 10);
		}
		UTAValue::UseDestrCount(2);
		UTAValue::CheckCounters();

		// All the same with plain type to ensure the policies
		// work correctly.
		{
			mg::common::HybridArray<int, 2> arr;

			for (const int& val : arr)
			{
				MG_UNUSED(val);
				MG_COMMON_ASSERT(false);
			}
			for (int& val : arr)
			{
				MG_UNUSED(val);
				MG_COMMON_ASSERT(false);
			}

			arr.Add(1);

			int count = 0;
			for (const int& val : arr)
			{
				MG_COMMON_ASSERT(val == count + 1);
				++count;
			}
			MG_COMMON_ASSERT(count == 1);

			count = 0;
			for (int& val : arr)
			{
				MG_COMMON_ASSERT(val == count + 1);
				val *= 10;
				++count;
			}
			MG_COMMON_ASSERT(count == 1);

			count = 0;
			for (int val : arr)
				MG_COMMON_ASSERT(val == ++count * 10);

			arr[0] = 1;
			arr.Add(2);

			count = 0;
			for (const int& val : arr)
			{
				MG_COMMON_ASSERT(val == count + 1);
				++count;
			}
			MG_COMMON_ASSERT(count == 2);

			count = 0;
			for (int& val : arr)
			{
				MG_COMMON_ASSERT(val == count + 1);
				val *= 10;
				++count;
			}
			MG_COMMON_ASSERT(count == 2);

			count = 0;
			for (int val : arr)
				MG_COMMON_ASSERT(val == ++count * 10);
		}
	}

	static void
	UnitTestArrayHybridRealloc()
	{
		UTAValue::ResetCounters();
		{
			mg::common::HybridArray<UTAValue, 3> arr;
			MG_COMMON_ASSERT(arr.Count() == 0);
			MG_COMMON_ASSERT(arr.GetCapacity() == 3);
			void* staticBuf = arr.GetBuffer();
			MG_COMMON_ASSERT(staticBuf != nullptr);

			UTAValue v;
			UTAValue::UseConstrCount(1);

			for (int i = 0; i < 3; ++i)
			{
				v.myValue = i + 1;
				arr.Add(v);
				MG_COMMON_ASSERT(arr.Count() == (uint32_t)(i + 1));
				MG_COMMON_ASSERT(arr.GetCapacity() == 3);
				MG_COMMON_ASSERT(arr.GetBuffer() == staticBuf);
				UTAValue::UseCopyConstrCount(1);
				for (int j = 0; j <= i; ++j)
					MG_COMMON_ASSERT(arr[j].myValue == j + 1);
			}

			v.myValue = 4;
			arr.Add(v);
			MG_COMMON_ASSERT(arr.Count() == 4);
			MG_COMMON_ASSERT(arr.GetCapacity() == 6);
			MG_COMMON_ASSERT(arr.GetBuffer() != staticBuf);
			for (int i = 0; i < 4; ++i)
				MG_COMMON_ASSERT(arr[i].myValue == i + 1);
			UTAValue::UseMoveConstrCount(3);
			UTAValue::UseDestrCount(3);
			UTAValue::UseCopyConstrCount(1);

			arr.RemoveAll();
			MG_COMMON_ASSERT(arr.GetBuffer() != staticBuf);
			MG_COMMON_ASSERT(arr.GetBuffer() != nullptr);
			UTAValue::UseDestrCount(4);
		}
		UTAValue::UseDestrCount(1);
		UTAValue::CheckCounters();
	}

	static void
	UnitTestArrayHybridConstructorCopy()
	{
		UTAValue::ResetCounters();
		// From empty normal array.
		{
			mg::common::Array<UTAValue> arr2;
			mg::common::HybridArray<UTAValue, 3> arr1(arr2);
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 3);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
		}
		UTAValue::CheckCounters();

		// From empty hybrid array.
		{
			// Note, the static size is different, and still
			// works.
			mg::common::HybridArray<UTAValue, 6> arr2;
			mg::common::HybridArray<UTAValue, 3> arr1(arr2);
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 3);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 6);
		}
		UTAValue::CheckCounters();

		// From non empty normal array, count fits.
		{
			mg::common::Array<UTAValue> arr2;
			for (int i = 0; i < 3; ++i)
				arr2.Add(UTAValue(i));
			UTAValue::ResetCounters();

			mg::common::HybridArray<UTAValue, 5> arr1(arr2);
			MG_COMMON_ASSERT(arr1.Count() == 3);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 5);
			MG_COMMON_ASSERT(arr2.Count() == 3);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 4);
			for (int i = 0; i < 3; ++i)
			{
				MG_COMMON_ASSERT(arr1[i].myValue == i);
				MG_COMMON_ASSERT(arr2[i].myValue == i);
			}
			UTAValue::UseCopyConstrCount(3);
		}
		UTAValue::UseDestrCount(6);
		UTAValue::CheckCounters();

		// From non empty hybrid array, count fits.
		{
			mg::common::HybridArray<UTAValue, 15> arr2;
			for (int i = 0; i < 3; ++i)
				arr2.Add(UTAValue(i));
			UTAValue::ResetCounters();

			mg::common::HybridArray<UTAValue, 10> arr1(arr2);
			MG_COMMON_ASSERT(arr1.Count() == 3);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 10);
			MG_COMMON_ASSERT(arr2.Count() == 3);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 15);
			for (int i = 0; i < 3; ++i)
			{
				MG_COMMON_ASSERT(arr1[i].myValue == i);
				MG_COMMON_ASSERT(arr2[i].myValue == i);
			}
			UTAValue::UseCopyConstrCount(3);
		}
		UTAValue::UseDestrCount(6);
		UTAValue::CheckCounters();

		// From non empty normal array, count does not fit.
		{
			mg::common::Array<UTAValue> arr2;
			for (int i = 0; i < 5; ++i)
				arr2.Add(UTAValue(i));
			UTAValue::ResetCounters();

			mg::common::HybridArray<UTAValue, 3> arr1(arr2);
			MG_COMMON_ASSERT(arr1.Count() == 5);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 6);
			MG_COMMON_ASSERT(arr2.Count() == 5);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 8);
			for (int i = 0; i < 5; ++i)
			{
				MG_COMMON_ASSERT(arr1[i].myValue == i);
				MG_COMMON_ASSERT(arr2[i].myValue == i);
			}
			UTAValue::UseCopyConstrCount(5);
		}
		UTAValue::UseDestrCount(10);
		UTAValue::CheckCounters();

		// From non empty hybrid array, count does not fit.
		{
			mg::common::HybridArray<UTAValue, 10> arr2;
			for (int i = 0; i < 10; ++i)
				arr2.Add(UTAValue(i));
			UTAValue::ResetCounters();

			mg::common::HybridArray<UTAValue, 7> arr1(arr2);
			MG_COMMON_ASSERT(arr1.Count() == 10);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 14);
			MG_COMMON_ASSERT(arr2.Count() == 10);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 10);
			for (int i = 0; i < 10; ++i)
			{
				MG_COMMON_ASSERT(arr1[i].myValue == i);
				MG_COMMON_ASSERT(arr2[i].myValue == i);
			}
			UTAValue::UseCopyConstrCount(10);
		}
		UTAValue::UseDestrCount(20);
		UTAValue::CheckCounters();

		// Copy of the same type.
		{
			mg::common::HybridArray<UTAValue, 8> arr;
			arr.Add(UTAValue(1));
			arr.Add(UTAValue(2));
			UTAValue::ResetCounters();

			mg::common::HybridArray<UTAValue, 8> arr2(arr);
			MG_COMMON_ASSERT(arr2[0].myValue == 1);
			MG_COMMON_ASSERT(arr2[1].myValue == 2);
			UTAValue::UseCopyConstrCount(2);
		}
		UTAValue::UseDestrCount(4);
		UTAValue::CheckCounters();
	}

	static void
	UnitTestArrayHybridConstructorMove()
	{
		UTAValue::ResetCounters();
		// From empty normal array.
		{
			mg::common::Array<UTAValue> arr2;
			mg::common::HybridArray<UTAValue, 3> arr1(mg::common::Move(arr2));
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr1.GetBuffer() == nullptr);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
		}
		UTAValue::CheckCounters();

		// From empty hybrid array.
		{
			// Note, the static size is different, and still
			// works.
			mg::common::HybridArray<UTAValue, 6> arr2;
			mg::common::HybridArray<UTAValue, 3> arr1(mg::common::Move(arr2));
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 3);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
		}
		UTAValue::CheckCounters();

		// From non empty normal array, count fits.
		{
			mg::common::Array<UTAValue> arr2;
			for (int i = 0; i < 3; ++i)
				arr2.Add(UTAValue(i));
			UTAValue::ResetCounters();

			void* buf = arr2.GetBuffer();
			MG_COMMON_ASSERT(buf != nullptr);

			mg::common::HybridArray<UTAValue, 5> arr1(mg::common::Move(arr2));
			MG_COMMON_ASSERT(arr1.Count() == 3);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 4);
			MG_COMMON_ASSERT(arr1.GetBuffer() == buf);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
			for (int i = 0; i < 3; ++i)
				MG_COMMON_ASSERT(arr1[i].myValue == i);
		}
		UTAValue::UseDestrCount(3);
		UTAValue::CheckCounters();

		// From non empty hybrid array, count fits.
		{
			mg::common::HybridArray<UTAValue, 15> arr2;
			for (int i = 0; i < 3; ++i)
				arr2.Add(UTAValue(i));
			UTAValue::ResetCounters();

			mg::common::HybridArray<UTAValue, 10> arr1(mg::common::Move(arr2));
			MG_COMMON_ASSERT(arr1.Count() == 3);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 10);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
			for (int i = 0; i < 3; ++i)
				MG_COMMON_ASSERT(arr1[i].myValue == i);
			UTAValue::UseMoveConstrCount(3);
			UTAValue::UseDestrCount(3);
		}
		UTAValue::UseDestrCount(3);
		UTAValue::CheckCounters();

		// From non empty normal array, count does not fit.
		{
			mg::common::Array<UTAValue> arr2;
			for (int i = 0; i < 5; ++i)
				arr2.Add(UTAValue(i));
			UTAValue::ResetCounters();

			void* buf = arr2.GetBuffer();
			MG_COMMON_ASSERT(buf != nullptr);

			mg::common::HybridArray<UTAValue, 3> arr1(mg::common::Move(arr2));
			MG_COMMON_ASSERT(arr1.Count() == 5);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 8);
			MG_COMMON_ASSERT(arr1.GetBuffer() == buf);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
			for (int i = 0; i < 5; ++i)
				MG_COMMON_ASSERT(arr1[i].myValue == i);
		}
		UTAValue::UseDestrCount(5);
		UTAValue::CheckCounters();

		// From non empty hybrid array, count does not fit.
		{
			mg::common::HybridArray<UTAValue, 10> arr2;
			for (int i = 0; i < 10; ++i)
				arr2.Add(UTAValue(i));
			UTAValue::ResetCounters();

			void* buf = arr2.GetBuffer();
			MG_COMMON_ASSERT(buf != nullptr);

			mg::common::HybridArray<UTAValue, 7> arr1(mg::common::Move(arr2));
			MG_COMMON_ASSERT(arr1.Count() == 10);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 14);
			MG_COMMON_ASSERT(arr1.GetBuffer() != buf);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
			for (int i = 0; i < 10; ++i)
				MG_COMMON_ASSERT(arr1[i].myValue == i);
			UTAValue::UseMoveConstrCount(10);
			UTAValue::UseDestrCount(10);
		}
		UTAValue::UseDestrCount(10);
		UTAValue::CheckCounters();

		// Move of the same type.
		{
			mg::common::HybridArray<UTAValue, 8> arr;
			arr.Add(UTAValue(1));
			arr.Add(UTAValue(2));
			UTAValue::ResetCounters();

			mg::common::HybridArray<UTAValue, 8> arr2(mg::common::Move(arr));
			MG_COMMON_ASSERT(arr2[0].myValue == 1);
			MG_COMMON_ASSERT(arr2[1].myValue == 2);
			UTAValue::UseMoveConstrCount(2);
			UTAValue::UseDestrCount(2);
		}
		UTAValue::UseDestrCount(2);
		UTAValue::CheckCounters();
	}

	static void
	UnitTestArrayHybridOperatorAssignCopy()
	{
		UTAValue::ResetCounters();
		// From empty normal array.
		{
			mg::common::Array<UTAValue> arr2;
			mg::common::HybridArray<UTAValue, 3> arr1;
			arr1 = arr2;
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 3);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
		}
		UTAValue::CheckCounters();

		// From empty hybrid array.
		{
			// Note, the static size is different, and still
			// works.
			mg::common::HybridArray<UTAValue, 6> arr2;
			mg::common::HybridArray<UTAValue, 3> arr1;
			arr1 = arr2;
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 3);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 6);
		}
		UTAValue::CheckCounters();

		// From non empty normal array, count fits.
		{
			mg::common::Array<UTAValue> arr2;
			mg::common::HybridArray<UTAValue, 5> arr1;
			for (int i = 0; i < 3; ++i)
				arr2.Add(UTAValue(i));
			UTAValue::ResetCounters();

			arr1 = arr2;
			MG_COMMON_ASSERT(arr1.Count() == 3);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 5);
			MG_COMMON_ASSERT(arr2.Count() == 3);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 4);
			for (int i = 0; i < 3; ++i)
			{
				MG_COMMON_ASSERT(arr1[i].myValue == i);
				MG_COMMON_ASSERT(arr2[i].myValue == i);
			}
			UTAValue::UseCopyConstrCount(3);
		}
		UTAValue::UseDestrCount(6);
		UTAValue::CheckCounters();

		// From non empty hybrid array, count fits.
		{
			mg::common::HybridArray<UTAValue, 15> arr2;
			mg::common::HybridArray<UTAValue, 10> arr1;
			for (int i = 0; i < 3; ++i)
				arr2.Add(UTAValue(i));
			UTAValue::ResetCounters();

			arr1 = arr2;
			MG_COMMON_ASSERT(arr1.Count() == 3);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 10);
			MG_COMMON_ASSERT(arr2.Count() == 3);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 15);
			for (int i = 0; i < 3; ++i)
			{
				MG_COMMON_ASSERT(arr1[i].myValue == i);
				MG_COMMON_ASSERT(arr2[i].myValue == i);
			}
			UTAValue::UseCopyConstrCount(3);
		}
		UTAValue::UseDestrCount(6);
		UTAValue::CheckCounters();

		// From non empty normal array, count does not fit.
		{
			mg::common::Array<UTAValue> arr2;
			mg::common::HybridArray<UTAValue, 3> arr1;
			for (int i = 0; i < 5; ++i)
				arr2.Add(UTAValue(i));
			UTAValue::ResetCounters();

			arr1 = arr2;
			MG_COMMON_ASSERT(arr1.Count() == 5);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 6);
			MG_COMMON_ASSERT(arr2.Count() == 5);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 8);
			for (int i = 0; i < 5; ++i)
			{
				MG_COMMON_ASSERT(arr1[i].myValue == i);
				MG_COMMON_ASSERT(arr2[i].myValue == i);
			}
			UTAValue::UseCopyConstrCount(5);
		}
		UTAValue::UseDestrCount(10);
		UTAValue::CheckCounters();

		// From non empty hybrid array, count does not fit.
		{
			mg::common::HybridArray<UTAValue, 10> arr2;
			mg::common::HybridArray<UTAValue, 7> arr1;
			for (int i = 0; i < 10; ++i)
				arr2.Add(UTAValue(i));
			UTAValue::ResetCounters();

			arr1 = arr2;
			MG_COMMON_ASSERT(arr1.Count() == 10);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 14);
			MG_COMMON_ASSERT(arr2.Count() == 10);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 10);
			for (int i = 0; i < 10; ++i)
			{
				MG_COMMON_ASSERT(arr1[i].myValue == i);
				MG_COMMON_ASSERT(arr2[i].myValue == i);
			}
			UTAValue::UseCopyConstrCount(10);
		}
		UTAValue::UseDestrCount(20);
		UTAValue::CheckCounters();

		// Assign of the same type.
		{
			mg::common::HybridArray<UTAValue, 8> arr;
			arr.Add(UTAValue(1));
			arr.Add(UTAValue(2));
			UTAValue::ResetCounters();

			mg::common::HybridArray<UTAValue, 8> arr2;
			arr2 = arr;
			MG_COMMON_ASSERT(arr2[0].myValue == 1);
			MG_COMMON_ASSERT(arr2[1].myValue == 2);
			UTAValue::UseCopyConstrCount(2);
		}
		UTAValue::UseDestrCount(4);
		UTAValue::CheckCounters();
	}

	static void
	UnitTestArrayHybridOperatorAssignMove()
	{
		UTAValue::ResetCounters();
		// Empty = empty normal array.
		{
			mg::common::HybridArray<UTAValue, 5> arr1;
			mg::common::Array<UTAValue> arr2;
			arr1 = mg::common::Move(arr2);
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr1.GetBuffer() == nullptr);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
		}
		UTAValue::CheckCounters();

		// Empty = empty hybrid array.
		{
			mg::common::HybridArray<UTAValue, 5> arr1;
			mg::common::HybridArray<UTAValue, 9> arr2;
			arr1 = mg::common::Move(arr2);
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 5);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
		}
		UTAValue::CheckCounters();

		// Empty = non empty normal array.
		{
			mg::common::HybridArray<UTAValue, 5> arr1;
			mg::common::Array<UTAValue> arr2;
			for (int i = 0; i < 3; ++i)
				arr2.Add(UTAValue(i));
			UTAValue::ResetCounters();

			MG_COMMON_ASSERT(arr2.Count() == 3);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 4);
			void* buf2 = arr2.GetBuffer();

			arr1 = mg::common::Move(arr2);
			MG_COMMON_ASSERT(arr1.Count() == 3);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 4);
			MG_COMMON_ASSERT(arr1.GetBuffer() == buf2);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
			for (int i = 0; i < 3; ++i)
				MG_COMMON_ASSERT(arr1[i].myValue == i);
		}
		UTAValue::UseDestrCount(3);
		UTAValue::CheckCounters();

		// Empty = non empty hybrid array.
		{
			mg::common::HybridArray<UTAValue, 5> arr1;
			mg::common::HybridArray<UTAValue, 9> arr2;
			for (int i = 0; i < 3; ++i)
				arr2.Add(UTAValue(i));
			UTAValue::ResetCounters();

			MG_COMMON_ASSERT(arr2.Count() == 3);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 9);
			void* buf1 = arr1.GetBuffer();

			arr1 = mg::common::Move(arr2);
			MG_COMMON_ASSERT(arr1.Count() == 3);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 5);
			MG_COMMON_ASSERT(arr1.GetBuffer() == buf1);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
			for (int i = 0; i < 3; ++i)
				MG_COMMON_ASSERT(arr1[i].myValue == i);
			UTAValue::UseMoveConstrCount(3);
			UTAValue::UseDestrCount(3);
		}
		UTAValue::UseDestrCount(3);
		UTAValue::CheckCounters();

		// Non empty = empty with no buffer.
		{
			mg::common::HybridArray<UTAValue, 5> arr1;
			mg::common::Array<UTAValue> arr2;
			for (int i = 0; i < 3; ++i)
				arr1.Add(UTAValue(i));
			UTAValue::ResetCounters();

			MG_COMMON_ASSERT(arr1.Count() == 3);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 5);
			MG_COMMON_ASSERT(arr1.GetBuffer() != nullptr);

			arr1 = mg::common::Move(arr2);
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr1.GetBuffer() == nullptr);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
			UTAValue::UseDestrCount(3);
		}
		UTAValue::CheckCounters();

		// Non empty = empty with an unused buffer.
		{
			mg::common::HybridArray<UTAValue, 5> arr1;
			mg::common::Array<UTAValue> arr2;
			for (int i = 0; i < 3; ++i)
				arr1.Add(UTAValue(i));
			arr2.Add(UTAValue(100));
			arr2.RemoveAll();
			UTAValue::ResetCounters();

			MG_COMMON_ASSERT(arr1.Count() == 3);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 5);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 1);
			void* buf2 = arr2.GetBuffer();
			MG_COMMON_ASSERT(buf2 != nullptr);

			arr1 = mg::common::Move(arr2);
			MG_COMMON_ASSERT(arr1.Count() == 0);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 1);
			MG_COMMON_ASSERT(arr1.GetBuffer() == buf2);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
			UTAValue::UseDestrCount(3);
		}
		UTAValue::CheckCounters();

		// Non empty = non empty with more data.
		{
			mg::common::HybridArray<UTAValue, 6> arr1;
			mg::common::HybridArray<UTAValue, 10> arr2;
			for (int v = 0; v < 3; ++v)
				arr1.Add(UTAValue(v));
			for (int v = 3; v < 8; ++v)
				arr2.Add(UTAValue(v));
			UTAValue::ResetCounters();

			MG_COMMON_ASSERT(arr1.Count() == 3);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 6);
			MG_COMMON_ASSERT(arr2.Count() == 5);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 10);

			arr1 = mg::common::Move(arr2);
			MG_COMMON_ASSERT(arr1.Count() == 5);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 6);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
			for (int i = 0, v = 3; i < 5; ++i, ++v)
				MG_COMMON_ASSERT(arr1[i].myValue == v);
			UTAValue::UseMoveAssignCount(3);
			UTAValue::UseMoveConstrCount(2);
			UTAValue::UseDestrCount(5);
		}
		UTAValue::UseDestrCount(5);
		UTAValue::CheckCounters();

		// Non empty = non empty with fewer data.
		{
			mg::common::HybridArray<UTAValue, 6> arr1;
			mg::common::HybridArray<UTAValue, 10> arr2;
			for (int v = 0; v < 3; ++v)
				arr2.Add(UTAValue(v));
			for (int v = 3; v < 8; ++v)
				arr1.Add(UTAValue(v));
			UTAValue::ResetCounters();

			MG_COMMON_ASSERT(arr1.Count() == 5);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 6);
			MG_COMMON_ASSERT(arr2.Count() == 3);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 10);

			arr1 = mg::common::Move(arr2);
			MG_COMMON_ASSERT(arr1.Count() == 3);
			MG_COMMON_ASSERT(arr1.GetCapacity() == 6);
			MG_COMMON_ASSERT(arr2.Count() == 0);
			MG_COMMON_ASSERT(arr2.GetCapacity() == 0);
			MG_COMMON_ASSERT(arr2.GetBuffer() == nullptr);
			for (int i = 0; i < 3; ++i)
				MG_COMMON_ASSERT(arr1[i].myValue == i);
			UTAValue::UseMoveAssignCount(3);
			UTAValue::UseDestrCount(5);
		}
		UTAValue::UseDestrCount(3);
		UTAValue::CheckCounters();

		// Move of the same type.
		{
			mg::common::HybridArray<UTAValue, 8> arr;
			arr.Add(UTAValue(1));
			arr.Add(UTAValue(2));
			UTAValue::ResetCounters();

			mg::common::HybridArray<UTAValue, 8> arr2;
			arr2 = mg::common::Move(arr);
			MG_COMMON_ASSERT(arr2[0].myValue == 1);
			MG_COMMON_ASSERT(arr2[1].myValue == 2);
			UTAValue::UseMoveConstrCount(2);
			UTAValue::UseDestrCount(2);
		}
		UTAValue::UseDestrCount(2);
		UTAValue::CheckCounters();
	}

	static void
	UnitTestArrayNewDelete()
	{
		// It is possible to allocate and delete arrays without a
		// virtual destructor.

		// New normal array.
		{
			mg::common::Array<UTAValue>* arr = new mg::common::Array<UTAValue>();
			arr->SetCount(3);
			MG_COMMON_ASSERT(arr->Count() == 3);
			MG_COMMON_ASSERT(arr->GetCapacity() == 3);
			for (int i = 0; i < 3; ++i)
				MG_COMMON_ASSERT((*arr)[i].myValue == -1);
			UTAValue::ResetCounters();
			delete arr;
			UTAValue::UseDestrCount(3);
		}

		// New hybrid array.
		{
			mg::common::HybridArray<UTAValue, 5>* arr =
				new mg::common::HybridArray<UTAValue, 5>();
			arr->SetCount(3);
			MG_COMMON_ASSERT(arr->Count() == 3);
			MG_COMMON_ASSERT(arr->GetCapacity() == 5);
			for (int i = 0; i < 3; ++i)
				MG_COMMON_ASSERT((*arr)[i].myValue == -1);
			UTAValue::ResetCounters();
			delete arr;
			UTAValue::UseDestrCount(3);
		}

		// New hybrid array, but delete as normal.
		{
			mg::common::HybridArray<UTAValue, 5>* arr =
				new mg::common::HybridArray<UTAValue, 5>();
			arr->SetCount(3);
			MG_COMMON_ASSERT(arr->Count() == 3);
			MG_COMMON_ASSERT(arr->GetCapacity() == 5);
			for (int i = 0; i < 3; ++i)
				MG_COMMON_ASSERT((*arr)[i].myValue == -1);
			UTAValue::ResetCounters();
			delete (mg::common::Array<UTAValue>*) arr;
			UTAValue::UseDestrCount(3);
		}
	}

	void
	UnitTestArray()
	{
		TestSuiteGuard suite("Array");

		UnitTestArrayAdd();
		UnitTestArrayAddAtIndex();
		UnitTestArrayRemoveAtIndex();
		UnitTestArrayDeleteAtIndex();
		UnitTestArrayRemoveCyclicAtIndex();
		UnitTestArrayDeleteCyclicAtIndex();
		UnitTestArrayRemoveLast();
		UnitTestArrayDeleteLast();
		UnitTestArrayRemoveAll();
		UnitTestArrayDeleteAll();
		UnitTestArrayPopLast();
		UnitTestArrayIsDynamic();
		UnitTestArrayOperatorEqual();
		UnitTestArrayReserve();
		UnitTestArraySetCount();
		UnitTestArrayTruncate();
		UnitTestArrayIndexOf();
		UnitTestArrayGet();
		UnitTestArrayOperatorAssignCopy();
		UnitTestArrayOperatorAssignCopyVariants();
		UnitTestArrayOperatorAssignMove();
		UnitTestArrayOperatorAssignMoveVariants();
		UnitTestArrayLast();
		UnitTestArrayConstructorCopy();
		UnitTestArrayConstructorMove();
		UnitTestArrayIterator();

		UnitTestArrayHybridRealloc();
		UnitTestArrayHybridConstructorCopy();
		UnitTestArrayHybridConstructorMove();
		UnitTestArrayHybridOperatorAssignCopy();
		UnitTestArrayHybridOperatorAssignMove();

		UnitTestArrayNewDelete();
	}

}
}
