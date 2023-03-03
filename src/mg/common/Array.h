#pragma once

#include "mg/common/Policies.h"
#include "mg/common/PseudoVirtual.h"
#include "mg/common/Util.h"

namespace mg {
namespace common {

	// The array is dynamic. It has 0 static data by default. In
	// order to provide a static storage use HybridArray.
	//
	template <typename T>
	class Array
		// The only child class is HybridArray, and it does not
		// need to destruct anything. So a real virtuality is not
		// needed. That saves 8 bytes by not storing a vtable
		// pointer.
		: public PseudoVirtual
	{
	public:
		Array();

		explicit Array(
			const Array& aOther);

		explicit Array(
			Array&& aOther);

		~Array();

		// Modification //////////////////////////////////////////

		void Reserve(
			uint32_t aSize);

		void SetCount(
			uint32_t aCount);

		void Truncate(
			uint32_t aCount);

		Array& operator=(
			const Array& aOther);

		Array& operator=(
			Array&& aOther);

		// Modification: addition ////////////////////////////////

		uint32_t Add(
			const T& aItem);

		uint32_t Add(
			T&& aItem);

		uint32_t AddAtIndex(
			const T& aItem,
			uint32_t aIndex);

		// Modification: removal /////////////////////////////////

		void RemoveAtIndex(
			uint32_t aIndex);

		void DeleteAtIndex(
			uint32_t aIndex);

		void RemoveCyclicAtIndex(
			uint32_t aIndex);

		void DeleteCyclicAtIndex(
			uint32_t aIndex);

		void RemoveLast();

		void DeleteLast();

		void RemoveAll();

		void DeleteAll();

		T PopLast();

		// Checks ////////////////////////////////////////////////

		bool IsDynamic() const;

		bool operator==(
			const Array& aOther) const;

		bool operator!=(
			const Array& aOther) const;

		// Access ////////////////////////////////////////////////

		int32_t IndexOf(
			const T& aItem) const;

		T& operator[](
			uint32_t aIndex);

		const T& operator[](
			uint32_t aIndex) const;

		T& Get(
			uint32_t aIndex);

		const T& Get(
			uint32_t aIndex) const;

		T& Last();

		const T& Last() const;

		uint32_t Count() const;

		uint32_t GetCapacity() const;

		uint32_t GetStaticCapacity() const;

		T* GetBuffer();

		const T* GetBuffer() const;

		// C++ sugar /////////////////////////////////////////////

		T* begin();

		T* end();

		const T* begin() const;

		const T* end() const;

	protected:
		// These constructors are protected, because are intended
		// to be used by HybridArray only.
		Array(
			uint32_t aTailCapacity,
			uint8_t* aTail);

		Array(
			uint32_t aTailCapacity,
			uint8_t* aTail,
			const Array& aOther);

		Array(
			uint32_t aTailCapacity,
			uint8_t* aTail,
			Array&& aOther);

	private:
		// Array does not have virtual methods, and therefore does
		// not have a vtable - its layout is strictly defined.
		// Also it assumes it is inherited by a non-virtual
		// HybridArray with a defined layout.
		//
		// HybridArray has a static buffer as a first member, so
		// Array can get it by looking at the end address of self.
		// It is valid to assume that HybridArray's members follow
		// Array in memory, because
		//
		//   - No vtable;
		//
		//   - Pointer at HybridArray class should be safe to cast
		//     to Array, which means Array members go first, for
		//     sure.
		//
		// It is also fine even in case Array is used on its own,
		// without HybridArray wrap. In that case the function
		// below will crash. But it is just never called for such
		// arrays.
		//
		// Note! Even if the dynamic alloc happens right after the
		// array object, it won't treat it like an own static
		// buf, because it will see the tail capacity is 0. The
		// static buf part is used **only** when the tail capacity
		// is > 0.
		const T* PrivGetBufferStatic() const;

		T* PrivGetBufferStatic();

		T* myData;
		uint32_t myCount;
		uint32_t myCapacity;
		uint32_t myTailCapacity;
		// Never used now. But can be used for something later.
		// It does not increase the object size, because otherwise
		// is turned into natural padding by the compiler.
		uint8_t myPadding[4];
	};

	// The same as Array, but looks more explicit as an out
	// parameter emphasizing that the array type does not matter.
	template <typename T>
	using IArray = Array<T>;

	template <typename T>
	inline
	Array<T>::Array()
		: myData(nullptr)
		, myCount(0)
		, myCapacity(0)
		, myTailCapacity(0)
	{
		// 8 bytes - sizeof(myData).
		// 12 bytes - 3 * sizeof(uint32_t).
		// 4 bytes - padding.
		static_assert(sizeof(Array) == 24, "Array should not have vtable");
		// Can be fixed when have aligned allocation functions.
		static_assert(alignof(Array) >= alignof(T), "Bad alignment");
	}

	template <typename T>
	inline
	Array<T>::Array(
		const Array& aOther)
		: myData(nullptr)
		, myCount(0)
		, myCapacity(0)
		, myTailCapacity(0)
	{
		*this = aOther;
	}

	template <typename T>
	inline
	Array<T>::Array(
		Array&& aOther)
		: myData(nullptr)
		, myCount(0)
		, myCapacity(0)
		, myTailCapacity(0)
	{
		*this = mg::common::Move(aOther);
	}

	template <typename T>
	inline
	Array<T>::Array(
		uint32_t aTailCapacity,
		uint8_t* aTail)
		: myData((T*)aTail)
		, myCount(0)
		, myCapacity(aTailCapacity)
		, myTailCapacity(aTailCapacity)
	{
		MG_DEV_ASSERT(aTailCapacity != 0);
		MG_DEV_ASSERT(PrivGetBufferStatic() == myData);
	}

	template <typename T>
	inline
	Array<T>::Array(
		uint32_t aTailCapacity,
		uint8_t* aTail,
		const Array& aOther)
		: myData((T*)aTail)
		, myCount(0)
		, myCapacity(aTailCapacity)
		, myTailCapacity(aTailCapacity)
	{
		MG_DEV_ASSERT(aTailCapacity != 0);
		MG_DEV_ASSERT(PrivGetBufferStatic() == myData);
		*this = aOther;
	}

	template <typename T>
	inline
	Array<T>::Array(
		uint32_t aTailCapacity,
		uint8_t* aTail,
		Array&& aOther)
		: myData((T*)aTail)
		, myCount(0)
		, myCapacity(aTailCapacity)
		, myTailCapacity(aTailCapacity)
	{
		MG_DEV_ASSERT(aTailCapacity != 0);
		MG_DEV_ASSERT(PrivGetBufferStatic() == myData);
		*this = mg::common::Move(aOther);
	}

	template <typename T>
	inline
	Array<T>::~Array()
	{
		ItemsDestroy(myData, myCount);
		if (IsDynamic())
			delete[] (uint8_t*)myData;
	}

	// Modification //////////////////////////////////////////////

	template <typename T>
	void
	Array<T>::Reserve(
		uint32_t aSize)
	{
		uint32_t oldCapacity = myCapacity;
		if (aSize <= oldCapacity)
			return;
		T* newData;
		uint32_t tailCapacity = myTailCapacity;
		uint32_t newCapacity;
		if (aSize <= tailCapacity)
		{
			// This can happen if this array got the dynamic data
			// from some other array via move-assign or swap. It
			// could be smaller than the array's own static buf.
			newData = PrivGetBufferStatic();
			newCapacity = tailCapacity;
		}
		else
		{
			newCapacity = oldCapacity * 2;
			if (newCapacity < aSize)
				newCapacity = aSize;
			// Use new[]/delete[], not free/malloc. Because
			// new/delete may be redefined on an upper level for
			// memory accounting.
			newData = (T*)new uint8_t[newCapacity * sizeof(T)];
		}
		T* oldData = myData;
		ItemsMoveConstruct(newData, oldData, myCount);
		ItemsDestroy(oldData, myCount);
		if (IsDynamic())
			delete[] (uint8_t*)oldData;
		myData = newData;
		myCapacity = newCapacity;
	}

	template <typename T>
	void
	Array<T>::SetCount(
		uint32_t aCount)
	{
		Reserve(aCount);
		if (aCount < myCount)
			ItemsDestroy(&myData[aCount], myCount - aCount);
		else
			ItemsConstruct(&myData[myCount], aCount - myCount);
		myCount = aCount;
	}

	template <typename T>
	inline void
	Array<T>::Truncate(
		uint32_t aCount)
	{
		if (aCount < myCount)
		{
			ItemsDestroy(&myData[aCount], myCount - aCount);
			myCount = aCount;
		}
	}

	template <typename T>
	Array<T>&
	Array<T>::operator=(
		const Array& aOther)
	{
		if (this == &aOther)
			return *this;

		uint32_t dstCount = myCount;
		uint32_t srcCount = aOther.myCount;
		Reserve(srcCount);
		const T* src = aOther.myData;
		T* dst = myData;
		if (srcCount >= dstCount)
		{
			ItemsCopy(dst, src, dstCount);
			ItemsCopyConstruct(&dst[dstCount], &src[dstCount], srcCount - dstCount);
		}
		else
		{
			ItemsCopy(dst, src, srcCount);
			ItemsDestroy(&dst[srcCount], dstCount - srcCount);
		}
		myCount = srcCount;
		return *this;
	}

	template <typename T>
	Array<T>&
	Array<T>::operator=(
		Array&& aOther)
	{
		if (this == &aOther)
			return *this;

		if (aOther.IsDynamic())
		{
			// 0-copy way - just steal the dynamic data. It is
			// much faster than moving the data into own location.
			// Even though it probably could fit into a smaller
			// buffer.
			ItemsDestroy(myData, myCount);
			if (IsDynamic())
				delete[] (uint8_t*)myData;
			myData = aOther.myData;
			myCount = aOther.myCount;
			myCapacity = aOther.myCapacity;
		}
		else
		{
			uint32_t srcCount = aOther.myCount;
			uint32_t dstCount = myCount;
			Reserve(srcCount);
			T* src = aOther.myData;
			T* dst = myData;
			if (srcCount >= dstCount)
			{
				ItemsMove(dst, src, dstCount);
				ItemsMoveConstruct(&dst[dstCount], &src[dstCount], srcCount - dstCount);
			}
			else
			{
				ItemsMove(dst, src, srcCount);
				ItemsDestroy(&dst[srcCount], dstCount - srcCount);
			}
			myCount = srcCount;
			ItemsDestroy(src, srcCount);
		}
		aOther.myData = nullptr;
		aOther.myCount = 0;
		aOther.myCapacity = 0;
		return *this;
	}

	// Modification: addition ////////////////////////////////////

	template <typename T>
	uint32_t
	Array<T>::Add(
		const T& aItem)
	{
		uint32_t idx = myCount;
		uint32_t newCount = idx + 1;
		Reserve(newCount);
		ItemsCopyConstruct(&myData[idx], &aItem);
		myCount = newCount;
		return idx;
	}

	template <typename T>
	uint32_t
	Array<T>::Add(
		T&& aItem)
	{
		uint32_t idx = myCount;
		uint32_t newCount = idx + 1;
		Reserve(newCount);
		ItemsMoveConstruct(&myData[idx], &aItem);
		myCount = newCount;
		return idx;
	}

	template <typename T>
	uint32_t
	Array<T>::AddAtIndex(
		const T& aItem,
		uint32_t aIndex)
	{
		uint32_t count = myCount;
		if (aIndex >= count)
			return Add(aItem);

		MG_COMMON_ASSERT(aIndex < count);
		uint32_t newCount = count + 1;
		Reserve(newCount);
		ItemsMoveConstruct(&myData[count], &myData[count - 1]);
		ItemsMoveForward(&myData[aIndex + 1], &myData[aIndex], count - aIndex - 1);
		myData[aIndex] = aItem;
		myCount = newCount;
		return aIndex;
	}

	// Modification: removal /////////////////////////////////////

	template <typename T>
	void
	Array<T>::RemoveAtIndex(
		uint32_t aIndex)
	{
		MG_COMMON_ASSERT(aIndex < myCount);
		uint32_t count = --myCount;
		T* point = &myData[aIndex];
		ItemsMoveBack(point, point + 1, count - aIndex);
		ItemsDestroy(&myData[count]);
	}

	template <typename T>
	void
	Array<T>::DeleteAtIndex(
		uint32_t aIndex)
	{
		MG_COMMON_ASSERT(aIndex < myCount);
		ItemsDelete(&myData[aIndex]);
		RemoveAtIndex(aIndex);
	}

	template <typename T>
	void
	Array<T>::RemoveCyclicAtIndex(
		uint32_t aIndex)
	{
		MG_COMMON_ASSERT(aIndex < myCount);
		uint32_t count = --myCount;
		if (count != aIndex)
			ItemsMove(&myData[aIndex], &myData[count]);
		ItemsDestroy(&myData[count]);
	}

	template <typename T>
	void
	Array<T>::DeleteCyclicAtIndex(
		uint32_t aIndex)
	{
		MG_COMMON_ASSERT(aIndex < myCount);
		ItemsDelete(&myData[aIndex]);
		RemoveCyclicAtIndex(aIndex);
	}

	template <typename T>
	inline void
	Array<T>::RemoveLast()
	{
		MG_COMMON_ASSERT(myCount > 0);
		ItemsDestroy(&myData[--myCount]);
	}

	template <typename T>
	inline void
	Array<T>::DeleteLast()
	{
		MG_COMMON_ASSERT(myCount > 0);
		ItemsDelete(&myData[--myCount]);
	}

	template <typename T>
	inline void
	Array<T>::RemoveAll()
	{
		ItemsDestroy(myData, myCount);
		myCount = 0;
	}

	template <typename T>
	inline void
	Array<T>::DeleteAll()
	{
		ItemsDelete(myData, myCount);
		myCount = 0;
	}

	template <typename T>
	T
	Array<T>::PopLast()
	{
		MG_COMMON_ASSERT(myCount > 0);
		uint32_t count = --myCount;
		T res = mg::common::Move(myData[count]);
		ItemsDestroy(&myData[count]);
		return res;
	}

	// Checks ////////////////////////////////////////////////////

	template <typename T>
	inline bool
	Array<T>::IsDynamic() const
	{
		return myTailCapacity == 0 || myData != PrivGetBufferStatic();
	}

	template <typename T>
	bool
	Array<T>::operator==(
		const Array& aOther) const
	{
		uint32_t count = myCount;
		if (aOther.myCount != count)
			return false;
		T* ldata = myData;
		T* rdata = aOther.myData;
		T* lend = ldata + count;
		for (; ldata < lend; ++ldata, ++rdata)
		{
			if (!ItemsAreEq(*ldata, *rdata))
				return false;
		}
		return true;
	}

	template <typename T>
	inline bool
	Array<T>::operator!=(
		const Array& aOther) const
	{
		return !(*this == aOther);
	}

	// Access ////////////////////////////////////////////////////

	template <typename T>
	int32_t
	Array<T>::IndexOf(
		const T& aItem) const
	{
		for (uint32_t i = 0; i < myCount; ++i)
		{
			if (ItemsAreEq(myData[i], aItem))
				return i;
		}
		return -1;
	}

	template <typename T>
	inline T&
	Array<T>::operator[](
		uint32_t aIndex)
	{
		return Get(aIndex);
	}

	template <typename T>
	inline const T&
	Array<T>::operator[](
		uint32_t aIndex) const
	{
		return Get(aIndex);
	}

	template <typename T>
	inline T&
	Array<T>::Get(
		uint32_t aIndex)
	{
		MG_COMMON_ASSERT(aIndex < myCount);
		return myData[aIndex];
	}

	template <typename T>
	inline const T&
	Array<T>::Get(
		uint32_t aIndex) const
	{
		MG_COMMON_ASSERT(aIndex < myCount);
		return myData[aIndex];
	}

	template <typename T>
	inline T&
	Array<T>::Last()
	{
		MG_COMMON_ASSERT(myCount > 0);
		return myData[myCount - 1];
	}

	template <typename T>
	inline const T&
	Array<T>::Last() const
	{
		MG_COMMON_ASSERT(myCount > 0);
		return myData[myCount - 1];
	}

	template <typename T>
	inline uint32_t
	Array<T>::Count() const
	{
		return myCount;
	}

	template <typename T>
	inline uint32_t
	Array<T>::GetCapacity() const
	{
		return myCapacity;
	}

	template <typename T>
	inline uint32_t
	Array<T>::GetStaticCapacity() const
	{
		return myTailCapacity;
	}

	template <typename T>
	inline T*
	Array<T>::GetBuffer()
	{
		return myData;
	}

	template <typename T>
	inline const T*
	Array<T>::GetBuffer() const
	{
		return myData;
	}

	// C++ sugar /////////////////////////////////////////////////

	template <typename T>
	inline T*
	Array<T>::begin()
	{
		return myData;
	}

	template <typename T>
	inline T*
	Array<T>::end()
	{
		return myData + myCount;
	}

	template <typename T>
	inline const T*
	Array<T>::begin() const
	{
		return myData;
	}

	template <typename T>
	inline const T*
	Array<T>::end() const
	{
		return myData + myCount;
	}

	// Private methods ///////////////////////////////////////////

	template <typename T>
	inline const T*
	Array<T>::PrivGetBufferStatic() const
	{
		MG_COMMON_ASSERT(myTailCapacity != 0);
		return (const T*) (this + 1);
	}

	template <typename T>
	inline T*
	Array<T>::PrivGetBufferStatic()
	{
		MG_COMMON_ASSERT(myTailCapacity != 0);
		return (T*) (this + 1);
	}

}
}
