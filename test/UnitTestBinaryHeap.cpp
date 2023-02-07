#include "mg/common/BinaryHeap.h"

#include "UnitTest.h"

#include <algorithm>

namespace mg {
namespace unittests {

	struct UTBHeapValue
	{
		UTBHeapValue()
			: myValue(-1)
			, myIndex(-1)
		{
			++ourConstrCount;
		}

		~UTBHeapValue()
		{
			++ourDestrCount;
			myValue = -2;
			myIndex = -2;
		}

		UTBHeapValue(
			const UTBHeapValue& aSrc)
			: myIndex(-1)
		{
			myValue = aSrc.myValue;
			++ourCopyConstrCount;
		}

		UTBHeapValue(
			UTBHeapValue&& aSrc)
			: myIndex(-1)
		{
			myValue = aSrc.myValue;
			++ourMoveConstrCount;
		}

		UTBHeapValue&
		operator=(
			const UTBHeapValue& aSrc)
		{
			myValue = aSrc.myValue;
			myIndex = -1;
			++ourCopyAssignCount;
			return *this;
		}

		UTBHeapValue&
		operator=(
			UTBHeapValue&& aSrc)
		{
			myValue = aSrc.myValue;
			myIndex = -1;
			++ourMoveAssignCount;
			return *this;
		}

		static void
		UseCopyConstrCount(
			int aToUse)
		{
			ourCopyConstrCount -= aToUse;
			MG_COMMON_ASSERT(ourCopyConstrCount >= 0);
		}

		static void
		UseCopyAssignCount(
			int aToUse)
		{
			ourCopyAssignCount -= aToUse;
			MG_COMMON_ASSERT(ourCopyAssignCount >= 0);
		}

		static void
		UseMoveConstrCount(
			int aToUse)
		{
			ourMoveConstrCount -= aToUse;
			MG_COMMON_ASSERT(ourMoveConstrCount >= 0);
		}

		static void
		UseMoveAssignCount(
			int aToUse)
		{
			ourMoveAssignCount -= aToUse;
			MG_COMMON_ASSERT(ourMoveAssignCount >= 0);
		}

		static void
		UseConstrCount(
			int aToUse)
		{
			ourConstrCount -= aToUse;
			MG_COMMON_ASSERT(ourConstrCount >= 0);
		}

		static void
		UseDestrCount(
			int aToUse)
		{
			ourDestrCount -= aToUse;
			MG_COMMON_ASSERT(ourDestrCount >= 0);
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
		}

		static void
		CheckCounters()
		{
			MG_COMMON_ASSERT(
				ourCopyConstrCount == 0 && ourCopyAssignCount == 0 &&
				ourMoveConstrCount == 0 && ourMoveAssignCount == 0 &&
				ourConstrCount == 0 && ourDestrCount == 0
			);
		}

		int myValue;
		int myIndex;

		static int ourCopyConstrCount;
		static int ourCopyAssignCount;
		static int ourMoveConstrCount;
		static int ourMoveAssignCount;
		static int ourConstrCount;
		static int ourDestrCount;

		inline bool
		operator<=(
			const UTBHeapValue& aValue) const
		{
			return myValue <= aValue.myValue;
		}

		inline bool
		operator>=(
			const UTBHeapValue& aValue) const
		{
			return myValue >= aValue.myValue;
		}
	};

	int UTBHeapValue::ourCopyConstrCount = 0;
	int UTBHeapValue::ourCopyAssignCount = 0;
	int UTBHeapValue::ourMoveConstrCount = 0;
	int UTBHeapValue::ourMoveAssignCount = 0;
	int UTBHeapValue::ourConstrCount = 0;
	int UTBHeapValue::ourDestrCount = 0;

	static void
	UnitTestBinaryHeapBasic()
	{
		// On 9 the permutation count is 362880.
		// On 10 it is 3628800 - the test becomes too long
		// (order of seconds instead of milliseconds).
		const int count = 9;
		UTBHeapValue values[count];
		{
			mg::common::BinaryHeapMinPtr<UTBHeapValue> heap;
			UTBHeapValue* pop = nullptr;
			MG_COMMON_ASSERT(!heap.Pop(pop));
			MG_COMMON_ASSERT(heap.Count() == 0);
			for (int i = 0; i < count; ++i)
				values[i].myValue = i;

			for (int counti = 1; counti <= count; ++counti)
			{
				int indexes[count];
				for (int i = 0; i < counti; ++i)
					indexes[i] = i;
				do
				{
					for (int i = 0; i < counti; ++i)
						heap.Push(&values[indexes[i]]);
					MG_COMMON_ASSERT(heap.Count() == (uint32)counti);
					for (int i = 0; i < counti; ++i)
					{
						pop = nullptr;
						MG_COMMON_ASSERT(heap.GetTop() == &values[i]);
						MG_COMMON_ASSERT(heap.Pop(pop));
						MG_COMMON_ASSERT(pop == &values[i]);
					}
					MG_COMMON_ASSERT(!heap.Pop(pop));
					MG_COMMON_ASSERT(heap.Count() == 0);
				} while (std::next_permutation(&indexes[0], &indexes[counti]));
			}
		}
		{
			mg::common::BinaryHeapMaxPtr<UTBHeapValue> heap;
			UTBHeapValue* pop = nullptr;
			MG_COMMON_ASSERT(!heap.Pop(pop));
			MG_COMMON_ASSERT(heap.Count() == 0);
			for (int i = 0; i < count; ++i)
				values[i].myValue = count - 1 - i;

			for (int counti = 1; counti <= count; ++counti)
			{
				int indexes[count];
				for (int i = 0; i < counti; ++i)
					indexes[i] = i;
				do
				{
					for (int i = 0; i < counti; ++i)
						heap.Push(&values[indexes[i]]);
					MG_COMMON_ASSERT(heap.Count() == (uint32)counti);
					for (int i = 0; i < counti; ++i)
					{
						pop = nullptr;
						MG_COMMON_ASSERT(heap.GetTop() == &values[i]);
						MG_COMMON_ASSERT(heap.Pop(pop));
						MG_COMMON_ASSERT(pop == &values[i]);
					}
					MG_COMMON_ASSERT(!heap.Pop(pop));
					MG_COMMON_ASSERT(heap.Count() == 0);
				} while (std::next_permutation(&indexes[0], &indexes[counti]));
			}
		}
		{
			mg::common::BinaryHeapMax<UTBHeapValue> heap;
			UTBHeapValue pop;
			pop.myValue = INT_MAX;
			MG_COMMON_ASSERT(!heap.Pop(pop));
			MG_COMMON_ASSERT(heap.Count() == 0);
			for (int i = 0; i < count; ++i)
				values[i].myValue = count - 1 - i;

			for (int counti = 1; counti <= count; ++counti)
			{
				int indexes[count];
				for (int i = 0; i < counti; ++i)
					indexes[i] = i;
				do
				{
					for (int i = 0; i < counti; ++i)
						heap.Push(values[indexes[i]]);
					MG_COMMON_ASSERT(heap.Count() == (uint32)counti);
					for (int i = 0; i < counti; ++i)
					{
						pop.myValue = INT_MAX;
						MG_COMMON_ASSERT(heap.GetTop().myValue == values[i].myValue);
						MG_COMMON_ASSERT(heap.Pop(pop));
						MG_COMMON_ASSERT(pop.myValue == values[i].myValue);
					}
					MG_COMMON_ASSERT(!heap.Pop(pop));
					MG_COMMON_ASSERT(heap.Count() == 0);
				} while (std::next_permutation(&indexes[0], &indexes[counti]));
			}
		}
		{
			mg::common::BinaryHeapMin<UTBHeapValue> heap;
			UTBHeapValue pop;
			pop.myValue = INT_MAX;
			MG_COMMON_ASSERT(!heap.Pop(pop));
			MG_COMMON_ASSERT(heap.Count() == 0);
			for (int i = 0; i < count; ++i)
				values[i].myValue = i;

			for (int counti = 1; counti <= count; ++counti)
			{
				int indexes[count];
				for (int i = 0; i < counti; ++i)
					indexes[i] = i;
				do
				{
					for (int i = 0; i < counti; ++i)
						heap.Push(values[indexes[i]]);
					MG_COMMON_ASSERT(heap.Count() == (uint32)counti);
					for (int i = 0; i < counti; ++i)
					{
						pop.myValue = INT_MAX;
						MG_COMMON_ASSERT(heap.GetTop().myValue == values[i].myValue);
						MG_COMMON_ASSERT(heap.Pop(pop));
						MG_COMMON_ASSERT(pop.myValue == values[i].myValue);
					}
					MG_COMMON_ASSERT(!heap.Pop(pop));
					MG_COMMON_ASSERT(heap.Count() == 0);
				} while (std::next_permutation(&indexes[0], &indexes[counti]));
			}
		}
		{
			mg::common::BinaryHeapMin<UTBHeapValue> heap;
			UTBHeapValue pop;
			pop.myValue = INT_MAX;
			MG_COMMON_ASSERT(!heap.Pop(pop));
			MG_COMMON_ASSERT(heap.Count() == 0);

			for (int counti = 1; counti <= count; ++counti)
			{
				int indexes[count];
				for (int i = 0; i < counti; ++i)
					indexes[i] = i;
				do
				{
					for (int i = 0; i < counti; ++i)
					{
						values[i].myValue = 0;
						heap.Push(values[i]);
					}
					for (int i = 0; i < counti; ++i)
					{
						heap.GetTop().myValue = indexes[i];
						heap.UpdateTop();
					}
					MG_COMMON_ASSERT(heap.Count() == (uint32)counti);
					for (int i = 0; i < counti; ++i)
					{
						pop.myValue = INT_MAX;
						MG_COMMON_ASSERT(heap.GetTop().myValue == i);
						MG_COMMON_ASSERT(heap.Pop(pop));
						MG_COMMON_ASSERT(pop.myValue == i);
					}
					MG_COMMON_ASSERT(!heap.Pop(pop));
					MG_COMMON_ASSERT(heap.Count() == 0);
				} while (std::next_permutation(&indexes[0], &indexes[counti]));
			}
		}
		{
			mg::common::BinaryHeapMinIntrusive<UTBHeapValue> heap;
			UTBHeapValue* pop = nullptr;

			// The idea is to try every possible update for every
			// possible element count in the given range.
			// For each count, each element is moved to each
			// possible place, including places between the other
			// elements, and the heap is checked to stay valid.
			for (int counti = 1; counti <= count; ++counti)
			{
				for (int srci = 0; srci < counti; ++srci)
				{
					for (int newv = 5, endv = srci * 10 + 5; newv <= endv; newv += 5)
					{
						for (int i = 0; i < counti; ++i)
						{
							values[i].myValue = (i + 1) * 10;
							heap.Push(&values[i]);
						}
						values[srci].myValue = newv;
						heap.Update(&values[srci]);
						MG_COMMON_ASSERT(values[srci].myIndex >= 0);
						MG_COMMON_ASSERT(values[srci].myIndex < counti);
						int prev = -1;
						for (int i = 0; i < counti; ++i)
						{
							pop = nullptr;
							MG_COMMON_ASSERT(heap.Pop(pop));
							MG_COMMON_ASSERT(pop->myIndex == -1);
							MG_COMMON_ASSERT(pop->myValue >= prev);
							MG_COMMON_ASSERT(pop->myValue >= 0);
							prev = pop->myValue;
							pop->myValue = -1;
						}
						MG_COMMON_ASSERT(!heap.Pop(pop));
						MG_COMMON_ASSERT(heap.Count() == 0);
					}
				}
			}
		}
		{
			mg::common::BinaryHeapMaxIntrusive<UTBHeapValue> heap;
			UTBHeapValue* pop = nullptr;

			// The idea is to try every possible delete for every
			// possible element count in the given range.
			// For each count, each element is removed and the
			// heap is checked to stay valid.
			for (int counti = 1; counti <= count; ++counti)
			{
				for (int srci = 0; srci < counti; ++srci)
				{
					for (int i = 0; i < counti; ++i)
					{
						values[i].myValue = i;
						heap.Push(&values[i]);
					}
					values[srci].myValue = INT_MAX;
					heap.Remove(&values[srci]);
					MG_COMMON_ASSERT(values[srci].myIndex == -1);
					int prev = INT_MAX;
					for (int i = 1; i < counti; ++i)
					{
						pop = nullptr;
						MG_COMMON_ASSERT(heap.Pop(pop));
						MG_COMMON_ASSERT(pop->myIndex == -1);
						MG_COMMON_ASSERT(pop->myValue < prev);
						MG_COMMON_ASSERT(pop->myValue >= 0);
						prev = pop->myValue;
						pop->myValue = -1;
					}
					MG_COMMON_ASSERT(!heap.Pop(pop));
					MG_COMMON_ASSERT(heap.Count() == 0);
				}
			}
		}
		{
			// Check reservation.
			mg::common::BinaryHeapMaxIntrusive<UTBHeapValue> heap;
			MG_COMMON_ASSERT(heap.GetCapacity() == 1);
			heap.Reserve(100);
			MG_COMMON_ASSERT(heap.GetCapacity() == 100);
		}
	}

	static void
	UnitTestBinaryHeapPushPop()
	{
		UTBHeapValue::ResetCounters();
		{
			mg::common::BinaryHeapMax<UTBHeapValue> heap;
			// No static elements are created.
		}
		UTBHeapValue::CheckCounters();

		{
			mg::common::BinaryHeapMax<UTBHeapValue, 2> heap;
			// No static elements are created.
		}
		UTBHeapValue::CheckCounters();

		{
			UTBHeapValue v1;
			UTBHeapValue v2;
			UTBHeapValue v3;
			UTBHeapValue v4;
			UTBHeapValue pop;
			v1.myValue = 1;
			v2.myValue = 2;
			v3.myValue = 3;
			v4.myValue = 4;
			mg::common::BinaryHeapMax<UTBHeapValue, 10> heap;
			UTBHeapValue::UseConstrCount(5);

			heap.Push(v1);
			UTBHeapValue::UseCopyConstrCount(1);

			heap.Push(v2);
			// Copy the new value to the end.
			UTBHeapValue::UseCopyConstrCount(1);
			// It should be on top, so moving procedure is
			// started. A temporary object with the new value is
			// created for that.
			UTBHeapValue::UseMoveConstrCount(1);
			// Move current top down.
			UTBHeapValue::UseMoveAssignCount(1);
			// Move the new value to the top.
			UTBHeapValue::UseMoveAssignCount(1);
			// Temporary object is deleted.
			UTBHeapValue::UseDestrCount(1);

			heap.Push(v3);
			// The same. Because the value 2 stays in place.
			UTBHeapValue::UseCopyConstrCount(1);
			UTBHeapValue::UseMoveConstrCount(1);
			UTBHeapValue::UseMoveAssignCount(2);
			UTBHeapValue::UseDestrCount(1);

			heap.Push(v4);
			// The same but +1 move, because the new element goes
			// up two times.
			UTBHeapValue::UseCopyConstrCount(1);
			UTBHeapValue::UseMoveConstrCount(1);
			UTBHeapValue::UseMoveAssignCount(3);
			UTBHeapValue::UseDestrCount(1);

			MG_COMMON_ASSERT(heap.Pop(pop));
			// Return result.
			UTBHeapValue::UseMoveAssignCount(1);
			// Move the rightmost value to the root to start its
			// way down.
			UTBHeapValue::UseMoveAssignCount(1);
			// Move down requires to create one temporary
			// variable.
			UTBHeapValue::UseMoveConstrCount(1);
			// The new root goes down 2 times.
			UTBHeapValue::UseMoveAssignCount(2);
			// Temporary variable is deleted.
			UTBHeapValue::UseDestrCount(1);
			// Removed element is destroyed in the heap.
			UTBHeapValue::UseDestrCount(1);

			MG_COMMON_ASSERT(heap.Pop(pop));
			// The tree becomes 2 level - root and 2 children. So
			// one move to return the old root, and one move to
			// set a new root to one of the children.
			UTBHeapValue::UseMoveAssignCount(2);
			// Removed element is destroyed in the heap.
			UTBHeapValue::UseDestrCount(1);

			MG_COMMON_ASSERT(heap.Pop(pop));
			// The same.
			UTBHeapValue::UseMoveAssignCount(2);
			// Removed element is destroyed in the heap.
			UTBHeapValue::UseDestrCount(1);

			MG_COMMON_ASSERT(heap.Pop(pop));
			// Move the single value to the out parameter.
			UTBHeapValue::UseMoveAssignCount(1);
			// Removed element is destroyed in the heap.
			UTBHeapValue::UseDestrCount(1);
		}
		// Stack variables are destroyed.
		UTBHeapValue::UseDestrCount(5);
		UTBHeapValue::CheckCounters();
	}

	static void
	UnitTestBinaryHeapUpdateTop()
	{
		{
			UTBHeapValue v1;
			UTBHeapValue v2;
			UTBHeapValue v3;
			UTBHeapValue v4;
			UTBHeapValue pop;
			v1.myValue = 1;
			v2.myValue = 2;
			v3.myValue = 3;
			v4.myValue = 4;
			mg::common::BinaryHeapMax<UTBHeapValue, 10> heap;
			heap.Push(v1);
			heap.Push(v2);
			heap.Push(v3);
			heap.Push(v4);
			UTBHeapValue::ResetCounters();

			heap.GetTop().myValue = 0;
			heap.UpdateTop();
			// Need to go down. The root is saved to a temporary
			// variable.
			UTBHeapValue::UseMoveConstrCount(1);
			// Two elements are moved up to fill the root.
			UTBHeapValue::UseMoveAssignCount(2);
			// One move to put the saved value to the end from the
			// temporary variable.
			UTBHeapValue::UseMoveAssignCount(1);
			// Temporary variable is deleted.
			UTBHeapValue::UseDestrCount(1);

			UTBHeapValue::CheckCounters();
			heap.Pop(pop);
			UTBHeapValue::ResetCounters();
			heap.GetTop().myValue = -1;
			heap.UpdateTop();
			// Single swap via a temporary variable, since there
			// is just 1 level left.
			UTBHeapValue::UseMoveConstrCount(1);
			UTBHeapValue::UseMoveAssignCount(2);
			UTBHeapValue::UseDestrCount(1);

			UTBHeapValue::CheckCounters();
			heap.Pop(pop);
			UTBHeapValue::ResetCounters();
			heap.GetTop().myValue = -2;
			heap.UpdateTop();
			// The same.
			UTBHeapValue::UseMoveConstrCount(1);
			UTBHeapValue::UseMoveAssignCount(2);
			UTBHeapValue::UseDestrCount(1);

			UTBHeapValue::CheckCounters();
			heap.Pop(pop);
			UTBHeapValue::ResetCounters();
			heap.GetTop().myValue = -3;
			heap.UpdateTop();
			// No actions when 1 element is left, because don't
			// have elements to reorder with.
		}
		// 5 from the stack and 1 in the heap.
		UTBHeapValue::UseDestrCount(6);
		UTBHeapValue::CheckCounters();
	}

	static void
	UnitTestBinaryHeapUpdateAny()
	{
		mg::common::BinaryHeapMinIntrusive<UTBHeapValue, 10> heap;

		UTBHeapValue v1;
		UTBHeapValue v2;
		UTBHeapValue v3;
		UTBHeapValue v4;
		v1.myValue = 0;
		heap.Push(&v1);
		MG_COMMON_ASSERT(v1.myIndex == 0);

		heap.Update(&v1);
		MG_COMMON_ASSERT(v1.myIndex == 0);

		v2.myValue = 1;
		heap.Push(&v2);
		MG_COMMON_ASSERT(v2.myIndex == 1);
		heap.Update(&v1);
		heap.Update(&v2);

		v2.myValue = -1;
		heap.Update(&v2);
		MG_COMMON_ASSERT(v2.myIndex == 0);
		MG_COMMON_ASSERT(v1.myIndex == 1);

		UTBHeapValue* pop = nullptr;
		MG_COMMON_ASSERT(heap.Pop(pop));
		MG_COMMON_ASSERT(pop->myIndex == -1);
		MG_COMMON_ASSERT(pop == &v2);
		MG_COMMON_ASSERT(v1.myIndex == 0);

		MG_COMMON_ASSERT(heap.Pop(pop));
		MG_COMMON_ASSERT(pop->myIndex == -1);
		MG_COMMON_ASSERT(pop == &v1);
		MG_COMMON_ASSERT(!heap.Pop(pop));

		v1.myValue = 0;
		v2.myValue = 1;
		v3.myValue = 2;
		v4.myValue = 3;
		heap.Push(&v1);
		heap.Push(&v2);
		heap.Push(&v3);
		heap.Push(&v4);
		MG_COMMON_ASSERT(v1.myIndex == 0);
		MG_COMMON_ASSERT(v2.myIndex == 1);
		MG_COMMON_ASSERT(v3.myIndex == 2);
		MG_COMMON_ASSERT(v4.myIndex == 3);

		v4.myValue = -4;
		heap.Update(&v4);
		MG_COMMON_ASSERT(v4.myIndex == 0);

		v3.myValue = -3;
		heap.Update(&v3);
		MG_COMMON_ASSERT(v3.myIndex == 2);

		v2.myValue = -2;
		heap.Update(&v2);
		MG_COMMON_ASSERT(v2.myIndex == 1);

		v1.myValue = -1;
		heap.Update(&v1);
		MG_COMMON_ASSERT(v1.myIndex == 3);

		pop = nullptr;
		MG_COMMON_ASSERT(heap.Pop(pop));
		MG_COMMON_ASSERT(pop->myIndex == -1);
		MG_COMMON_ASSERT(pop == &v4);
		MG_COMMON_ASSERT(heap.Pop(pop));
		MG_COMMON_ASSERT(pop->myIndex == -1);
		MG_COMMON_ASSERT(pop == &v3);
		MG_COMMON_ASSERT(heap.Pop(pop));
		MG_COMMON_ASSERT(pop->myIndex == -1);
		MG_COMMON_ASSERT(pop == &v2);
		MG_COMMON_ASSERT(heap.Pop(pop));
		MG_COMMON_ASSERT(pop->myIndex == -1);
		MG_COMMON_ASSERT(pop == &v1);
		MG_COMMON_ASSERT(!heap.Pop(pop));
	}

	static void
	UnitTestBinaryHeapRemoveTop()
	{
		{
			UTBHeapValue v1;
			UTBHeapValue v2;
			UTBHeapValue v3;
			UTBHeapValue v4;
			UTBHeapValue pop;
			v1.myValue = 1;
			v2.myValue = 2;
			v3.myValue = 3;
			v4.myValue = 4;
			mg::common::BinaryHeapMax<UTBHeapValue, 10> heap;
			UTBHeapValue::ResetCounters();

			// NOP on empty.
			heap.RemoveTop();
			MG_COMMON_ASSERT(heap.Count() == 0);

			// Single element remove does not copy nor move
			// anything.
			heap.Push(v1);
			MG_COMMON_ASSERT(heap.Count() == 1);
			UTBHeapValue::UseCopyConstrCount(1);
			heap.RemoveTop();
			UTBHeapValue::UseDestrCount(1);
			MG_COMMON_ASSERT(heap.Count() == 0);
			UTBHeapValue::CheckCounters();

			// Remove() costs less than Pop().
			heap.Push(v1);
			heap.Push(v2);
			heap.Push(v3);
			heap.Push(v4);
			MG_COMMON_ASSERT(heap.Count() == 4);
			UTBHeapValue::ResetCounters();
			heap.RemoveTop();
			// Move end to the deleted root.
			UTBHeapValue::UseMoveAssignCount(1);
			// Single moving down is required and is done via a
			// swap using a temporary variable.
			UTBHeapValue::UseMoveConstrCount(1);
			UTBHeapValue::UseMoveAssignCount(2);
			UTBHeapValue::UseDestrCount(1);
			// The removed element is destroyed in the heap.
			UTBHeapValue::UseDestrCount(1);
		}
		// 5 from the stack, 3 from the heap.
		UTBHeapValue::UseDestrCount(8);
		UTBHeapValue::CheckCounters();
	}

	static void
	UnitTestBinaryHeapRemoveAny()
	{
		mg::common::BinaryHeapMinIntrusive<UTBHeapValue, 10> heap;

		UTBHeapValue v1;
		UTBHeapValue v2;
		UTBHeapValue v3;
		UTBHeapValue v4;
		v1.myValue = 0;
		heap.Push(&v1);
		MG_COMMON_ASSERT(v1.myIndex == 0);

		heap.Remove(&v1);
		MG_COMMON_ASSERT(v1.myIndex == -1);
		MG_COMMON_ASSERT(heap.Count() == 0);

		v2.myValue = 1;
		heap.Push(&v1);
		heap.Push(&v2);

		heap.Remove(&v1);
		MG_COMMON_ASSERT(v1.myIndex == -1);
		MG_COMMON_ASSERT(v2.myIndex == 0);
		MG_COMMON_ASSERT(heap.Count() == 1);

		heap.Remove(&v2);
		MG_COMMON_ASSERT(v2.myIndex == -1);
		MG_COMMON_ASSERT(heap.Count() == 0);

		v1.myValue = 0;
		v2.myValue = 1;
		v3.myValue = 2;
		v4.myValue = 3;
		heap.Push(&v1);
		heap.Push(&v2);
		heap.Push(&v3);
		heap.Push(&v4);
		MG_COMMON_ASSERT(v1.myIndex == 0);
		MG_COMMON_ASSERT(v2.myIndex == 1);
		MG_COMMON_ASSERT(v3.myIndex == 2);
		MG_COMMON_ASSERT(v4.myIndex == 3);

		heap.Remove(&v1);
		MG_COMMON_ASSERT(v1.myIndex == -1);
		MG_COMMON_ASSERT(v2.myIndex == 0);
		MG_COMMON_ASSERT(v3.myIndex == 2);
		MG_COMMON_ASSERT(v4.myIndex == 1);

		heap.Remove(&v2);
		MG_COMMON_ASSERT(v2.myIndex == -1);
		MG_COMMON_ASSERT(v3.myIndex == 0);
		MG_COMMON_ASSERT(v4.myIndex == 1);

		heap.Remove(&v3);
		MG_COMMON_ASSERT(v3.myIndex == -1);
		MG_COMMON_ASSERT(v4.myIndex == 0);

		heap.Remove(&v4);
		MG_COMMON_ASSERT(v4.myIndex == -1);
		MG_COMMON_ASSERT(heap.Count() == 0);
	}

	static void
	UnitTestBinaryHeapIndexName()
	{
		// Can use a non-default index name.
		struct TestValue
		{
			bool
			operator<=(
				const TestValue& aItem2) const
			{
				return myValue <= aItem2.myValue;
			}

			int myValue;
			int myIdx;
		};
		mg::common::BinaryHeapMinIntrusive<TestValue, 10, &TestValue::myIdx> heap;
		TestValue v1;
		v1.myValue = 1;
		v1.myIdx = -1;
		TestValue v2;
		v2.myValue = 2;
		v2.myIdx = -1;
		heap.Push(&v1);
		heap.Push(&v2);
		MG_COMMON_ASSERT(v1.myIdx == 0);
		MG_COMMON_ASSERT(v2.myIdx == 1);
		TestValue* pop = nullptr;
		MG_COMMON_ASSERT(heap.Pop(pop));
		MG_COMMON_ASSERT(pop->myIdx == -1);
		MG_COMMON_ASSERT(pop == &v1);
		MG_COMMON_ASSERT(heap.Pop(pop));
		MG_COMMON_ASSERT(pop->myIdx == -1);
		MG_COMMON_ASSERT(pop == &v2);

		heap.Push(&v1);
		heap.Push(&v2);
		v2.myValue = 0;
		heap.Update(&v2);
		MG_COMMON_ASSERT(v1.myIdx == 1);
		MG_COMMON_ASSERT(v2.myIdx == 0);

		heap.Remove(&v2);
		MG_COMMON_ASSERT(v1.myIdx == 0);
		MG_COMMON_ASSERT(v2.myIdx == -1);

		heap.Remove(&v1);
		MG_COMMON_ASSERT(v1.myIdx == -1);
	}

	static void
	UnitTestBinaryHeapRealloc()
	{
		{
			mg::common::BinaryHeapMin<UTBHeapValue, 1> heap;
			UTBHeapValue v;
			v.myValue = 1;
			heap.Push(v);
			UTBHeapValue::ResetCounters();

			v.myValue = 2;
			heap.Push(v);
			// Old value move as a result of realloc.
			UTBHeapValue::UseMoveConstrCount(1);
			// Old location destruction.
			UTBHeapValue::UseDestrCount(1);
			// New value copy.
			UTBHeapValue::UseCopyConstrCount(1);

			v.myValue = 3;
			heap.Push(v);
			// Old values move as a result of realloc.
			UTBHeapValue::UseMoveConstrCount(2);
			// Old location destruction.
			UTBHeapValue::UseDestrCount(2);
			// New value copy.
			UTBHeapValue::UseCopyConstrCount(1);
			// Grows x2 at least.
			MG_COMMON_ASSERT(heap.GetCapacity() == 4);
		}
		UTBHeapValue::UseDestrCount(4);
		UTBHeapValue::CheckCounters();
	}

	void
	UnitTestBinaryHeap()
	{
		TestSuiteGuard suite("BinaryHeap");

		UnitTestBinaryHeapBasic();
		UnitTestBinaryHeapPushPop();
		UnitTestBinaryHeapUpdateTop();
		UnitTestBinaryHeapUpdateAny();
		UnitTestBinaryHeapRemoveTop();
		UnitTestBinaryHeapRemoveAny();
		UnitTestBinaryHeapIndexName();
		UnitTestBinaryHeapRealloc();
	}

}
}
