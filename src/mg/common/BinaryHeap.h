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

#include "mg/common/HybridArray.h"

namespace mg {
namespace common {

	// Binary heap is a priority queue data structure. It gives
	// the following complexity estimations:
	//
	// - Space: O(n), to store the elements.
	//
	// - Insertion: O(1) in the best case, O(log(n)) in the worst
	//   one. Example of the best case is when, for example, the
	//   heap is min-heap, and a new element is the biggest. This
	//   is particularly useful for storing something having
	//   deadlines - new usually are bigger.
	//
	// - Find top element: O(1). In case of min-heap it is the
	//   smallest element. In case of max-heap it is the biggest
	//   element.
	//
	// - Delete or update any element: O(log(n)).
	//
	// The heap is a complete binary tree, so the log(n) means
	// literally <= log(n), without constant multiplicators. For
	// instance, for a 10k stored elements, it is just 14
	// operations for any operation in the worst case.
	//
	// The heap is optimized for structures having proper move
	// constructors, and tries to make as few copies as possible.
	//
	template<typename T, uint32 StaticSize, typename Methods>
	class BinaryHeap
	{
	public:
		void Push(
			const T& aValue);

		// If objects in the heap are updated, then it is going to
		// be twice faster to update them in-place, instead of
		// doing Pop + Push.
		void UpdateTop();

		void Update(
			const T& aValue);

		bool RemoveTop();

		void Remove(
			const T& aValue);

		bool Pop(
			T& aOutValue);

		T& GetTop();

		const T& GetTop() const;

		uint32 Count() const;

		uint32 GetCapacity() const;

		void Reserve(
			uint32 aCount);

	private:
		static uint32 PrivParentIndex(
			uint32 aIndex);

		static uint32 PrivLeftChildIndex(
			uint32 aIndex);

		bool PrivUpdateUp(
			uint32 aIndex);

		bool PrivUpdateDown(
			uint32 aIndex);

		void PrivUpdate(
			uint32 aIndex);

		mg::common::HybridArray<T, StaticSize> myData;
	};

	//////////////////////////////////////////////////////////////
	// Implementations of heap index save.
	//
	// Saving an index helps to be able to remove and update any
	// elements, not only the top one. Without an index it would
	// take O(N) time to find the element to be able to update it.
	// With the index the look-up takes O(1) time.
	//
	// There is no an active set index for non-ptr heaps, because
	// it does not make sense. If the heap stores copy of an
	// element, the heap owner does not have these elements anyway
	// and can't modify them.

	template<typename T, int32 T::*myIndex>
	struct BinaryHeapMethodPtrIndexEnabled
	{
		static inline void
		SetIndex(
			T* aItem,
			int32 aIndex)
		{
			aItem->*myIndex = aIndex;
		}

		static inline int32
		GetIndex(
			const T* aItem)
		{
			return aItem->*myIndex;
		}

		// The validation should compare the pointers, not
		// objects. It is used by update/remove by index to ensure
		// the index really points at the value it belongs to.
		static inline bool
		IsSame(
			const T* aItem1,
			const T* aItem2)
		{
			return aItem1 == aItem2;
		}
	};

	template<typename T>
	struct BinaryHeapMethodPtrIndexDisabled
	{
		static inline void
		SetIndex(
			const T*,
			int32)
		{
		}

		static inline int32
		GetIndex(
			const T*)
		{
			MG_COMMON_ASSERT(!"Index access is disabled, use intrusive heap");
			return false;
		}

		static inline bool
		IsSame(
			const T*,
			const T*)
		{
			MG_COMMON_ASSERT(!"Index access is disabled, use intrusive heap");
			return false;
		}
	};

	template<typename T>
	struct BinaryHeapMethodObjIndexDisabled
	{
		static inline void
		SetIndex(
			const T&,
			int32)
		{
		}

		static inline int32
		GetIndex(
			const T&)
		{
			MG_COMMON_ASSERT(!"Index access is disabled, use intrusive heap");
			return 0;
		}

		static inline bool
		IsSame(
			const T*,
			const T*)
		{
			MG_COMMON_ASSERT(!"Index access is disabled, use intrusive heap");
			return false;
		}
	};

	//////////////////////////////////////////////////////////////
	// Implementations of heap ordering.
	//
	// In min-heap the top element is the smallest. In max-heap -
	// the top is the greatest.

	template<typename T>
	struct BinaryHeapMethodMinPtrIsLeftAbove
	{
		static inline bool
		IsLeftAbove(
			const T* aLeft,
			const T* aRight)
		{
			return *aLeft <= *aRight;
		}
	};

	template<typename T>
	struct BinaryHeapMethodMinObjIsLeftAbove
	{
		static inline bool
		IsLeftAbove(
			const T& aLeft,
			const T& aRight)
		{
			return aLeft <= aRight;
		}
	};

	template<typename T>
	struct BinaryHeapMethodMaxPtrIsLeftAbove
	{
		static inline bool
		IsLeftAbove(
			const T* aLeft,
			const T* aRight)
		{
			return *aLeft >= *aRight;
		}
	};

	template<typename T>
	struct BinaryHeapMethodMaxObjIsLeftAbove
	{
		static inline bool
		IsLeftAbove(
			const T& aLeft,
			const T& aRight)
		{
			return aLeft >= aRight;
		}
	};

	//////////////////////////////////////////////////////////////
	// Heap method combinations.

	template<typename T>
	struct BinaryHeapMinPtrMethods
		: public BinaryHeapMethodMinPtrIsLeftAbove<T>
		, public BinaryHeapMethodPtrIndexDisabled<T>
	{
	};

	template<typename T, int32 T::*myIndex>
	struct BinaryHeapMinPtrIntrusiveMethods
		: public BinaryHeapMethodMinPtrIsLeftAbove<T>
		, public BinaryHeapMethodPtrIndexEnabled<T, myIndex>
	{
	};

	template<typename T>
	struct BinaryHeapMaxPtrMethods
		: public BinaryHeapMethodMaxPtrIsLeftAbove<T>
		, public BinaryHeapMethodPtrIndexDisabled<T>
	{
	};

	template<typename T, int32 T::*myIndex>
	struct BinaryHeapMaxPtrIntrusiveMethods
		: public BinaryHeapMethodMaxPtrIsLeftAbove<T>
		, public BinaryHeapMethodPtrIndexEnabled<T, myIndex>
	{
	};

	template<typename T>
	struct BinaryHeapMinMethods
		: public BinaryHeapMethodMinObjIsLeftAbove<T>
		, public BinaryHeapMethodObjIndexDisabled<T>
	{
	};

	template<typename T>
	struct BinaryHeapMaxMethods
		: public BinaryHeapMethodMaxObjIsLeftAbove<T>
		, public BinaryHeapMethodObjIndexDisabled<T>
	{
	};

	//////////////////////////////////////////////////////////////
	// Actual heaps available for usage.

	// Min-heap (top element is the smallest) for pointers to
	// actual data. Works good for big and/or non-copyable
	// objects, and/or not belonging to the heap.
	// Non-intrusive, so only top element can be updated and
	// removed.
	template<typename T, uint32 StaticSize = 1>
	using BinaryHeapMinPtr = BinaryHeap<
		T*, StaticSize, BinaryHeapMinPtrMethods<T>
	>;

	// Min-heap (top element is the smallest) for pointers to
	// actual data. Works good for big and/or non-copyable
	// objects, and/or not belonging to the heap.
	// Intrusive, so any element can be updated and removed for
	// logarithmic time. If an element is removed from the heap,
	// its index is set to -1.
	template<typename T, uint32 StaticSize = 1, int32 T::*myIndex = &T::myIndex>
	using BinaryHeapMinIntrusive = BinaryHeap<
		T*, StaticSize, BinaryHeapMinPtrIntrusiveMethods<T, myIndex>
	>;

	// Max-heap (top element is the greatest) for pointers to
	// actual data. Works good for big and/or non-copyable
	// objects, and/or not belonging to the heap.
	// Non-intrusive, so only top element can be updated and
	// removed.
	template<typename T, uint32 StaticSize = 1>
	using BinaryHeapMaxPtr = BinaryHeap<
		T*, StaticSize, BinaryHeapMaxPtrMethods<T>
	>;

	// Max-heap (top element is the greatest) for pointers to
	// actual data. Works good for big and/or non-copyable
	// objects, and/or not belonging to the heap.
	// Intrusive, so any element can be updated and removed for
	// logarithmic time. If an element is removed from the heap,
	// its index is set to -1.
	template<typename T, uint32 StaticSize = 1, int32 T::*myIndex = &T::myIndex>
	using BinaryHeapMaxIntrusive = BinaryHeap<
		T*, StaticSize, BinaryHeapMaxPtrIntrusiveMethods<T, myIndex>
	>;

	// Min-heap (top element is the smallest) storing copies of
	// elements. Works good for small objects, or for big objects
	// with low update rate on the heap.
	template<typename T, uint32 StaticSize = 1>
	using BinaryHeapMin = BinaryHeap<
		T, StaticSize, BinaryHeapMinMethods<T>
	>;

	// Max-heap (top element is the greatest) storing copies of
	// elements. Works good for small objects, or for big objects
	// with low update rate on the heap.
	template<typename T, uint32 StaticSize = 1>
	using BinaryHeapMax = BinaryHeap<
		T, StaticSize, BinaryHeapMaxMethods<T>
	>;

	//////////////////////////////////////////////////////////////
	// Generic heap methods implementation.

	template<typename T, uint32 StaticSize, typename Methods>
	void
	BinaryHeap<T, StaticSize, Methods>::Push(
		const T& aValue)
	{
		uint32 index = myData.Add(aValue);
		Methods::SetIndex(myData[index], index);
		PrivUpdateUp(index);
	}

	template<typename T, uint32 StaticSize, typename Methods>
	inline void
	BinaryHeap<T, StaticSize, Methods>::UpdateTop()
	{
		PrivUpdateDown(0);
	}

	template<typename T, uint32 StaticSize, typename Methods>
	inline void
	BinaryHeap<T, StaticSize, Methods>::Update(
		const T& aValue)
	{
		int32 index = Methods::GetIndex(aValue);
		MG_COMMON_ASSERT(index >= 0);
		// Ensure the index really points at the value it is
		// referencing.
		MG_COMMON_ASSERT(Methods::IsSame(aValue, myData[index]));
		PrivUpdate(index);
	}

	template<typename T, uint32 StaticSize, typename Methods>
	bool
	BinaryHeap<T, StaticSize, Methods>::RemoveTop()
	{
		uint32 count = myData.Count();
		if (count == 0)
			return false;
		T* tree = myData.GetBuffer();
		Methods::SetIndex(tree[0], -1);
		if (count == 1)
		{
			myData.RemoveLast();
			return true;
		}
		tree[0] = mg::common::Move(tree[count - 1]);
		Methods::SetIndex(tree[0], 0);
		myData.RemoveLast();
		PrivUpdateDown(0);
		return true;
	}

	template<typename T, uint32 StaticSize, typename Methods>
	void
	BinaryHeap<T, StaticSize, Methods>::Remove(
		const T& aValue)
	{
		int32 iIndex = Methods::GetIndex(aValue);
		MG_COMMON_ASSERT(iIndex >= 0);
		uint32 index = (uint32) iIndex;

		uint32 count = myData.Count();
		MG_COMMON_ASSERT(index < count);
		T* tree = myData.GetBuffer();
		// Ensure the index really points at the value it is
		// referencing.
		MG_COMMON_ASSERT(Methods::IsSame(aValue, tree[index]));
		Methods::SetIndex(tree[index], -1);
		if (index + 1 == count)
			return myData.RemoveLast();
		tree[index] = mg::common::Move(tree[count - 1]);
		Methods::SetIndex(tree[index], index);
		myData.RemoveLast();
		PrivUpdate(index);
	}

	template<typename T, uint32 StaticSize, typename Methods>
	inline bool
	BinaryHeap<T, StaticSize, Methods>::Pop(
		T& aOutValue)
	{
		if (myData.Count() == 0)
			return false;
		aOutValue = mg::common::Move(myData[0]);
		return RemoveTop();
	}

	template<typename T, uint32 StaticSize, typename Methods>
	inline T&
	BinaryHeap<T, StaticSize, Methods>::GetTop()
	{
		return myData[0];
	}

	template<typename T, uint32 StaticSize, typename Methods>
	inline const T&
	BinaryHeap<T, StaticSize, Methods>::GetTop() const
	{
		return myData[0];
	}

	template<typename T, uint32 StaticSize, typename Methods>
	inline uint32
	BinaryHeap<T, StaticSize, Methods>::Count() const
	{
		return myData.Count();
	}

	template<typename T, uint32 StaticSize, typename Methods>
	inline uint32
	BinaryHeap<T, StaticSize, Methods>::GetCapacity() const
	{
		return myData.GetCapacity();
	}

	template<typename T, uint32 StaticSize, typename Methods>
	inline void
	BinaryHeap<T, StaticSize, Methods>::Reserve(
		uint32 aCount)
	{
		myData.Reserve(aCount);
	}

	template<typename T, uint32 StaticSize, typename Methods>
	inline uint32
	BinaryHeap<T, StaticSize, Methods>::PrivParentIndex(
		uint32 aIndex)
	{
		return (aIndex - 1) >> 1;
	}

	template<typename T, uint32 StaticSize, typename Methods>
	inline uint32
	BinaryHeap<T, StaticSize, Methods>::PrivLeftChildIndex(
		uint32 aIndex)
	{
		return (aIndex << 1) + 1;
	}

	template<typename T, uint32 StaticSize, typename Methods>
	bool
	BinaryHeap<T, StaticSize, Methods>::PrivUpdateUp(
		uint32 aIndex)
	{
		if (aIndex == 0)
			return false;
		T* tree = myData.GetBuffer();
		uint32 parent = PrivParentIndex(aIndex);
		if (Methods::IsLeftAbove(tree[parent], tree[aIndex]))
			return false;
		// The checks above repeat the while-condition so as to
		// avoid creation of the temporary object when the
		// target element is already in place.
		T tmp = mg::common::Move(tree[aIndex]);
		do {
			tree[aIndex] = mg::common::Move(tree[parent]);
			Methods::SetIndex(tree[aIndex], aIndex);
			aIndex = parent;
			parent = PrivParentIndex(aIndex);
		} while (aIndex != 0 && Methods::IsLeftAbove(tmp, tree[parent]));
		tree[aIndex] = mg::common::Move(tmp);
		Methods::SetIndex(tree[aIndex], aIndex);
		return true;
	}

	template<typename T, uint32 StaticSize, typename Methods>
	bool
	BinaryHeap<T, StaticSize, Methods>::PrivUpdateDown(
		uint32 aIndex)
	{
		uint32 count = myData.Count();
		uint32 leftChild = PrivLeftChildIndex(aIndex);
		if (leftChild >= count)
			return false;
		T* tree = myData.GetBuffer();
		uint32 rightChild = leftChild + 1;
		uint32 topChild;
		if (rightChild >= count)
			topChild = leftChild;
		else if (Methods::IsLeftAbove(tree[leftChild], tree[rightChild]))
			topChild = leftChild;
		else
			topChild = rightChild;
		if (Methods::IsLeftAbove(tree[aIndex], tree[topChild]))
			return false;
		// The checks above repeat the checks from the while
		// loop so as avoid creation of the temporary object
		// when the target element is already in place.
		T tmp = mg::common::Move(tree[aIndex]);
		do
		{
			tree[aIndex] = mg::common::Move(tree[topChild]);
			Methods::SetIndex(tree[aIndex], aIndex);
			aIndex = topChild;

			leftChild = PrivLeftChildIndex(aIndex);
			if (leftChild >= count)
				break;
			rightChild = leftChild + 1;
			if (rightChild >= count)
				topChild = leftChild;
			else if (Methods::IsLeftAbove(tree[leftChild], tree[rightChild]))
				topChild = leftChild;
			else
				topChild = rightChild;
		} while (Methods::IsLeftAbove(tree[topChild], tmp));
		MG_COMMON_ASSERT(aIndex < count);
		tree[aIndex] = mg::common::Move(tmp);
		Methods::SetIndex(tree[aIndex], aIndex);
		return true;
	}

	template<typename T, uint32 StaticSize, typename Methods>
	inline void
	BinaryHeap<T, StaticSize, Methods>::PrivUpdate(
		uint32 aIndex)
	{
		// The element either goes up, or stays in place, or
		// goes down. It can't go up and then down.
		//
		// It is easy to prove. Assume the heap is min-heap. Also
		// assume the updated part of the heap looks like this:
		//
		//     R
		//  C1   C2
		//
		// According to the heap properties, R <= C1 and R <= C2.
		// Assume C1 is updated to N, which is <= R. It is going
		// to be moved up in place of R and the heap will look
		// like this:
		//
		//     N
		//   R   C2
		//
		// N does not need to go down to right, because N <= R,
		// and R <= C2, so N <= C2.
		//
		// Even easier is to prove that if a node went down, it
		// won't go up. Because the only way to go up is to be
		// less than the parent, which is not the case. Otherwise
		// it wouldn't go down.
		PrivUpdateUp(aIndex) || PrivUpdateDown(aIndex);
	}

}
}
