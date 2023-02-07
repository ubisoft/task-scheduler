#pragma once

#include "mg/common/TypeTraits.h"

namespace mg {
namespace common {

	template<typename T, T* T::*myLink>
	struct ForwardListIterator
	{
		ForwardListIterator(
			T* aHead);

		ForwardListIterator& operator++();

		T* operator*() const;

		bool operator!=(
			const ForwardListIterator& aOther) const;

		T* myPos;
		T* myNext;
	};

	template<typename T, T* T::*myLink>
	struct ForwardListConstIterator
	{
		ForwardListConstIterator(
			const T* aHead);

		ForwardListConstIterator& operator++();

		const T* operator*() const;

		bool operator!=(
			const ForwardListConstIterator& aOther) const;

		const T* myPos;
	};

	// Forward singly-linked list targeted at the most efficient
	// append and iteration.
	//
	// Algorithm is the same as used in the Linux kernel and
	// sys/queue.h on Linux. But this list is more flexible thanks
	// to C++, because no need to have a wrapper around the link.
	// This allows to reuse this link to store the elements in
	// different lists. Not simultaneously unless there are
	// multiple links, one per each list.
	template<typename T, T* T::*myLink = &T::myNext>
	struct ForwardList
	{
		ForwardList();

		// Can only move, no copy. Elements can't be in 2 lists,
		// because they use a single link element.
		ForwardList(
			ForwardList&& aList);

		ForwardList(
			T* aFirst,
			T* aLast);

		void Prepend(
			T* aItem);

		void Prepend(
			ForwardList&& aSrc);

		void Prepend(
			T* aFirst,
			T* aLast);

		void Append(
			T* aItem);

		void Append(
			ForwardList&& aSrc);

		void Append(
			T* aFirst,
			T* aLast);

		void Insert(
			T* aPrev,
			T* aItem);

		void Reverse();

		void Clear();

		T* PopAll();

		T* PopAll(
			T*& aOutLast);

		T* PopFirst();

		ForwardList& operator=(
			ForwardList&& aOther);

		T* GetFirst();

		const T* GetFirst() const;

		T* GetLast();

		const T* GetLast() const;

		bool IsEmpty() const;

		ForwardListIterator<T, myLink> begin();

		ForwardListIterator<T, myLink> end();

		ForwardListConstIterator<T, myLink> begin() const;

		ForwardListConstIterator<T, myLink> end() const;

	private:
		T* myFirst;
		// Pointer at the link of the last element. When the list
		// is empty, it points at the first link (myFirst). This
		// simple trick allows not to branch when append.
		T** myLastLink;
	};

	//////////////////////////////////////////////////////////////

	template<typename T, T* T::*myLink>
	inline
	ForwardListIterator<T, myLink>::ForwardListIterator(
		T* aHead)
		: myPos(aHead)
	{
		if (aHead != nullptr)
			myNext = aHead->*myLink;
	}

	template<typename T, T* T::*myLink>
	inline ForwardListIterator<T, myLink>&
	ForwardListIterator<T, myLink>::operator++()
	{
		myPos = myNext;
		if (myNext != nullptr)
			myNext = myNext->*myLink;
		return *this;
	}

	template<typename T, T* T::*myLink>
	inline T*
	ForwardListIterator<T, myLink>::operator*() const
	{
		return myPos;
	}

	template<typename T, T* T::*myLink>
	inline bool
	ForwardListIterator<T, myLink>::operator!=(
		const ForwardListIterator& aOther) const
	{
		return myPos != aOther.myPos;
	}

	//////////////////////////////////////////////////////////////

	template<typename T, T* T::*myLink>
	inline
	ForwardListConstIterator<T, myLink>::ForwardListConstIterator(
		const T* aHead)
		: myPos(aHead)
	{
	}

	template<typename T, T* T::*myLink>
	inline ForwardListConstIterator<T, myLink>&
	ForwardListConstIterator<T, myLink>::operator++()
	{
		myPos = myPos->*myLink;
		return *this;
	}

	template<typename T, T* T::*myLink>
	inline const T*
	ForwardListConstIterator<T, myLink>::operator*() const
	{
		return myPos;
	}

	template<typename T, T* T::*myLink>
	inline bool
	ForwardListConstIterator<T, myLink>::operator!=(
		const ForwardListConstIterator& aOther) const
	{
		return myPos != aOther.myPos;
	}

	//////////////////////////////////////////////////////////////

	template<typename T, T* T::*myLink>
	inline
	ForwardList<T, myLink>::ForwardList()
		: myFirst(nullptr)
		, myLastLink(&myFirst)
	{
	}

	template<typename T, T* T::*myLink>
	inline
	ForwardList<T, myLink>::ForwardList(
		ForwardList&& aList)
		: myFirst(aList.myFirst)
		, myLastLink(aList.myLastLink)
	{
		aList.Clear();
	}

	template<typename T, T* T::*myLink>
	inline
	ForwardList<T, myLink>::ForwardList(
		T* aFirst,
		T* aLast)
		: myFirst(aFirst)
	{
		if (aLast != nullptr)
		{
			myLastLink = &(aLast->*myLink);
			*myLastLink = nullptr;
		}
		else
		{
			myLastLink = &myFirst;
		}
	}

	template<typename T, T* T::*myLink>
	inline void
	ForwardList<T, myLink>::Prepend(
		T* aItem)
	{
		aItem->*myLink = myFirst;
		if (myFirst == nullptr)
			myLastLink = &(aItem->*myLink);
		myFirst = aItem;
	}

	template<typename T, T* T::*myLink>
	inline void
	ForwardList<T, myLink>::Prepend(
		ForwardList&& aSrc)
	{
		T* srcFirst = aSrc.myFirst;
		if (srcFirst != nullptr)
		{
			T* last = mg::common::ContainerOf(aSrc.myLastLink, myLink);
			last->*myLink = myFirst;
			if (myFirst == nullptr)
				myLastLink = &(last->*myLink);
			myFirst = aSrc.myFirst;
			aSrc.Clear();
		}
	}

	template<typename T, T* T::*myLink>
	inline void
	ForwardList<T, myLink>::Prepend(
		T* aFirst,
		T* aLast)
	{
		if (aFirst != nullptr)
		{
			aLast->*myLink = myFirst;
			if (myFirst == nullptr)
				myLastLink = &(aLast->*myLink);
			myFirst = aFirst;
		}
	}

	template<typename T, T* T::*myLink>
	inline T*
	ForwardList<T, myLink>::PopFirst()
	{
		T* res = myFirst;
		myFirst = res->*myLink;
		if (myFirst == nullptr)
			myLastLink = &myFirst;
		return res;
	}

	template<typename T, T* T::*myLink>
	inline void
	ForwardList<T, myLink>::Append(
		T* aItem)
	{
		// Key feature of the list - branch-less append of one
		// element.
		aItem->*myLink = nullptr;
		*myLastLink = aItem;
		myLastLink = &(aItem->*myLink);
	}

	template<typename T, T* T::*myLink>
	inline void
	ForwardList<T, myLink>::Append(
		ForwardList&& aSrc)
	{
		T* srcFirst = aSrc.myFirst;
		if (srcFirst != nullptr)
		{
			*myLastLink = srcFirst;
			myLastLink = aSrc.myLastLink;
			aSrc.Clear();
		}
	}

	template<typename T, T* T::*myLink>
	inline void
	ForwardList<T, myLink>::Append(
		T* aFirst,
		T* aLast)
	{
		if (aFirst != nullptr)
		{
			*myLastLink = aFirst;
			myLastLink = &(aLast->*myLink);
			*myLastLink = nullptr;
		}
	}

	template<typename T, T* T::*myLink>
	void
	ForwardList<T, myLink>::Insert(
		T* aPrev,
		T* aItem)
	{
		if (aPrev != nullptr)
		{
			aItem->*myLink = aPrev->*myLink;
			aPrev->*myLink = aItem;
			if (aItem->*myLink == nullptr)
				myLastLink = &(aItem->*myLink);
		}
		else if (myFirst == nullptr)
		{
			myFirst = aItem;
			myLastLink = &(aItem->*myLink);
			*myLastLink = nullptr;
		}
		else
		{
			aItem->*myLink = myFirst;
			myFirst = aItem;
		}
	}

	template<typename T, T* T::*myLink>
	inline void
	ForwardList<T, myLink>::Reverse()
	{
		T* item = myFirst;
		T* next;
		Clear();
		while (item != nullptr)
		{
			next = item->*myLink;
			Prepend(item);
			item = next;
		}
	}

	template<typename T, T* T::*myLink>
	inline void
	ForwardList<T, myLink>::Clear()
	{
		myFirst = nullptr;
		myLastLink = &myFirst;
	}

	template<typename T, T* T::*myLink>
	inline T*
	ForwardList<T, myLink>::PopAll()
	{
		T* res = myFirst;
		Clear();
		return res;
	}

	template<typename T, T* T::*myLink>
	inline T*
	ForwardList<T, myLink>::PopAll(
		T*& aOutLast)
	{
		T* res = myFirst;
		aOutLast = GetLast();
		Clear();
		return res;
	}

	template<typename T, T* T::*myLink>
	inline ForwardList<T, myLink>&
	ForwardList<T, myLink>::operator=(
		ForwardList&& aOther)
	{
		myFirst = aOther.myFirst;
		if (myFirst != nullptr)
		{
			myLastLink = aOther.myLastLink;
			aOther.Clear();
		}
		else
		{
			myLastLink = &myFirst;
		}
		return *this;
	}

	template<typename T, T* T::*myLink>
	inline T*
	ForwardList<T, myLink>::GetFirst()
	{
		return myFirst;
	}

	template<typename T, T* T::*myLink>
	inline const T*
	ForwardList<T, myLink>::GetFirst() const
	{
		return myFirst;
	}

	template<typename T, T* T::*myLink>
	inline T*
	ForwardList<T, myLink>::GetLast()
	{
		T** lastPtr = myLastLink;
		if (lastPtr == &myFirst)
			return nullptr;
		return mg::common::ContainerOf(lastPtr, myLink);
	}

	template<typename T, T* T::*myLink>
	inline const T*
	ForwardList<T, myLink>::GetLast() const
	{
		T*const* lastPtr = myLastLink;
		if (lastPtr == &myFirst)
			return nullptr;
		return mg::common::ContainerOf(lastPtr, myLink);
	}

	template<typename T, T* T::*myNext>
	inline bool
	ForwardList<T, myNext>::IsEmpty() const
	{
		return myFirst == nullptr;
	}

	template<typename T, T* T::*myNext>
	ForwardListIterator<T, myNext>
	ForwardList<T, myNext>::begin()
	{
		return ForwardListIterator<T, myNext>(myFirst);
	}

	template<typename T, T* T::*myNext>
	ForwardListIterator<T, myNext>
	ForwardList<T, myNext>::end()
	{
		return ForwardListIterator<T, myNext>(nullptr);
	}

	template<typename T, T* T::*myNext>
	ForwardListConstIterator<T, myNext>
	ForwardList<T, myNext>::begin() const
	{
		return ForwardListConstIterator<T, myNext>(myFirst);
	}

	template<typename T, T* T::*myNext>
	ForwardListConstIterator<T, myNext>
	ForwardList<T, myNext>::end() const
	{
		return ForwardListConstIterator<T, myNext>(nullptr);
	}

}
}
