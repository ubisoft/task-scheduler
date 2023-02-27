#pragma once

#include "mg/common/Assert.h"
#include "mg/common/TypeTraits.h"

#include <utility>
#include <vector>

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
	template<typename T, typename Methods>
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

		uint32_t Count() const;

		uint32_t GetCapacity() const;

		void Reserve(
			uint32_t aCount);

	private:
		static uint32_t PrivParentIndex(
			uint32_t aIndex);

		static uint32_t PrivLeftChildIndex(
			uint32_t aIndex);

		bool PrivUpdateUp(
			uint32_t aIndex);

		bool PrivUpdateDown(
			uint32_t aIndex);

		void PrivUpdate(
			uint32_t aIndex);

		std::vector<T> myData;
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

	template<typename T, int32_t T::*myIndex>
	struct BinaryHeapMethodPtrIndexEnabled
	{
		static inline void
		SetIndex(
			T* aItem,
			int32_t aIndex)
		{
			aItem->*myIndex = aIndex;
		}

		static inline int32_t
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
			int32_t)
		{
		}

		static inline int32_t
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
			int32_t)
		{
		}

		static inline int32_t
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

	template<typename T, int32_t T::*myIndex>
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

	template<typename T, int32_t T::*myIndex>
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
	template<typename T>
	using BinaryHeapMinPtr = BinaryHeap<
		T*, BinaryHeapMinPtrMethods<T>
	>;

	// Min-heap (top element is the smallest) for pointers to
	// actual data. Works good for big and/or non-copyable
	// objects, and/or not belonging to the heap.
	// Intrusive, so any element can be updated and removed for
	// logarithmic time. If an element is removed from the heap,
	// its index is set to -1.
	template<typename T, int32_t T::*myIndex = &T::myIndex>
	using BinaryHeapMinIntrusive = BinaryHeap<
		T*, BinaryHeapMinPtrIntrusiveMethods<T, myIndex>
	>;

	// Max-heap (top element is the greatest) for pointers to
	// actual data. Works good for big and/or non-copyable
	// objects, and/or not belonging to the heap.
	// Non-intrusive, so only top element can be updated and
	// removed.
	template<typename T>
	using BinaryHeapMaxPtr = BinaryHeap<
		T*, BinaryHeapMaxPtrMethods<T>
	>;

	// Max-heap (top element is the greatest) for pointers to
	// actual data. Works good for big and/or non-copyable
	// objects, and/or not belonging to the heap.
	// Intrusive, so any element can be updated and removed for
	// logarithmic time. If an element is removed from the heap,
	// its index is set to -1.
	template<typename T, int32_t T::*myIndex = &T::myIndex>
	using BinaryHeapMaxIntrusive = BinaryHeap<
		T*, BinaryHeapMaxPtrIntrusiveMethods<T, myIndex>
	>;

	// Min-heap (top element is the smallest) storing copies of
	// elements. Works good for small objects, or for big objects
	// with low update rate on the heap.
	template<typename T>
	using BinaryHeapMin = BinaryHeap<
		T, BinaryHeapMinMethods<T>
	>;

	// Max-heap (top element is the greatest) storing copies of
	// elements. Works good for small objects, or for big objects
	// with low update rate on the heap.
	template<typename T>
	using BinaryHeapMax = BinaryHeap<
		T, BinaryHeapMaxMethods<T>
	>;

	//////////////////////////////////////////////////////////////
	// Generic heap methods implementation.

	template<typename T, typename Methods>
	void
	BinaryHeap<T, Methods>::Push(
		const T& aValue)
	{
		int32_t index = myData.size();
		myData.push_back(aValue);
		Methods::SetIndex(myData[index], index);
		PrivUpdateUp(index);
	}

	template<typename T, typename Methods>
	inline void
	BinaryHeap<T, Methods>::UpdateTop()
	{
		PrivUpdateDown(0);
	}

	template<typename T, typename Methods>
	inline void
	BinaryHeap<T, Methods>::Update(
		const T& aValue)
	{
		int32_t index = Methods::GetIndex(aValue);
		MG_COMMON_ASSERT(index >= 0);
		// Ensure the index really points at the value it is
		// referencing.
		MG_COMMON_ASSERT(Methods::IsSame(aValue, myData[index]));
		PrivUpdate(index);
	}

	template<typename T, typename Methods>
	bool
	BinaryHeap<T, Methods>::RemoveTop()
	{
		uint32_t count = myData.size();
		if (count == 0)
			return false;
		T* tree = myData.data();
		Methods::SetIndex(tree[0], -1);
		if (count == 1)
		{
			myData.pop_back();
			return true;
		}
		tree[0] = std::move(tree[count - 1]);
		Methods::SetIndex(tree[0], 0);
		myData.pop_back();
		PrivUpdateDown(0);
		return true;
	}

	template<typename T, typename Methods>
	void
	BinaryHeap<T, Methods>::Remove(
		const T& aValue)
	{
		int32_t iIndex = Methods::GetIndex(aValue);
		MG_COMMON_ASSERT(iIndex >= 0);
		uint32_t index = (uint32_t) iIndex;

		uint32_t count = myData.size();
		MG_COMMON_ASSERT(index < count);
		T* tree = myData.data();
		// Ensure the index really points at the value it is
		// referencing.
		MG_COMMON_ASSERT(Methods::IsSame(aValue, tree[index]));
		Methods::SetIndex(tree[index], -1);
		if (index + 1 == count)
			return myData.pop_back();
		tree[index] = std::move(tree[count - 1]);
		Methods::SetIndex(tree[index], index);
		myData.pop_back();
		PrivUpdate(index);
	}

	template<typename T, typename Methods>
	inline bool
	BinaryHeap<T, Methods>::Pop(
		T& aOutValue)
	{
		if (myData.empty())
			return false;
		aOutValue = std::move(myData[0]);
		return RemoveTop();
	}

	template<typename T, typename Methods>
	inline T&
	BinaryHeap<T, Methods>::GetTop()
	{
		return myData[0];
	}

	template<typename T, typename Methods>
	inline const T&
	BinaryHeap<T, Methods>::GetTop() const
	{
		return myData[0];
	}

	template<typename T, typename Methods>
	inline uint32_t
	BinaryHeap<T, Methods>::Count() const
	{
		return myData.size();
	}

	template<typename T, typename Methods>
	inline uint32_t
	BinaryHeap<T, Methods>::GetCapacity() const
	{
		return myData.capacity();
	}

	template<typename T, typename Methods>
	inline void
	BinaryHeap<T, Methods>::Reserve(
		uint32_t aCount)
	{
		myData.reserve(aCount);
	}

	template<typename T, typename Methods>
	inline uint32_t
	BinaryHeap<T, Methods>::PrivParentIndex(
		uint32_t aIndex)
	{
		return (aIndex - 1) >> 1;
	}

	template<typename T, typename Methods>
	inline uint32_t
	BinaryHeap<T, Methods>::PrivLeftChildIndex(
		uint32_t aIndex)
	{
		return (aIndex << 1) + 1;
	}

	template<typename T, typename Methods>
	bool
	BinaryHeap<T, Methods>::PrivUpdateUp(
		uint32_t aIndex)
	{
		if (aIndex == 0)
			return false;
		T* tree = myData.data();
		uint32_t parent = PrivParentIndex(aIndex);
		if (Methods::IsLeftAbove(tree[parent], tree[aIndex]))
			return false;
		// The checks above repeat the while-condition so as to
		// avoid creation of the temporary object when the
		// target element is already in place.
		T tmp = std::move(tree[aIndex]);
		do {
			tree[aIndex] = std::move(tree[parent]);
			Methods::SetIndex(tree[aIndex], aIndex);
			aIndex = parent;
			parent = PrivParentIndex(aIndex);
		} while (aIndex != 0 && Methods::IsLeftAbove(tmp, tree[parent]));
		tree[aIndex] = std::move(tmp);
		Methods::SetIndex(tree[aIndex], aIndex);
		return true;
	}

	template<typename T, typename Methods>
	bool
	BinaryHeap<T, Methods>::PrivUpdateDown(
		uint32_t aIndex)
	{
		uint32_t count = myData.size();
		uint32_t leftChild = PrivLeftChildIndex(aIndex);
		if (leftChild >= count)
			return false;
		T* tree = myData.data();
		uint32_t rightChild = leftChild + 1;
		uint32_t topChild;
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
		T tmp = std::move(tree[aIndex]);
		do
		{
			tree[aIndex] = std::move(tree[topChild]);
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
		tree[aIndex] = std::move(tmp);
		Methods::SetIndex(tree[aIndex], aIndex);
		return true;
	}

	template<typename T, typename Methods>
	inline void
	BinaryHeap<T, Methods>::PrivUpdate(
		uint32_t aIndex)
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
