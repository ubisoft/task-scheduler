#pragma once

#include "mg/common/Types.h"

#include <atomic>

namespace mg {
namespace common {

	template <typename T, T* T::*myNext = &T::myNext>
	class MultiProducerQueueIntrusive
	{
	public:
		MultiProducerQueueIntrusive()
			: myHead(nullptr)
		{
		}

		inline bool
		IsEmpty()
		{
			return myHead.load() == nullptr;
		}

		// Returns true if the object was first. That may be
		// useful if, for example, need to signal an event, when
		// the queue becomes not empty, to another thread maybe.
		// Then it makes sense to do the signal only when a first
		// object is added, because the thread signaling is
		// usually expensive.
		inline bool
		Push(
			T* aItem)
		{
			return PrivPushRange(aItem, aItem);
		}

		// Constant complexity instead of linear for normal
		// multiple items push. Works the best when order is not
		// important or reverse back to normal order is done along
		// with something else afterwards.
		inline bool
		PushManyFastReversed(
			T* aFirst,
			T* aLast)
		{
			if (aFirst == nullptr)
				return IsEmpty();
			aLast->*myNext = nullptr;
			return PrivPushRange(aFirst, aLast);
		}

		bool
		PushManyFastReversed(
			T* aFirst)
		{
			if (aFirst == nullptr)
				return IsEmpty();
			T* first = aFirst;
			T* last = aFirst;
			T* next;
			while ((next = last->*myNext) != nullptr)
				last = next;
			return PrivPushRange(first, last);
		}

		bool
		PushMany(
			T* aFirst)
		{
			if (aFirst == nullptr)
				return IsEmpty();

			T* first = nullptr;
			T* last = aFirst;
			T* prev = aFirst;
			aFirst = aFirst->*myNext;
			if (aFirst == nullptr)
				return Push(last);

			T* next;
			do
			{
				next = aFirst->*myNext;
				aFirst->*myNext = prev;
				prev = aFirst;
			} while ((aFirst = next) != nullptr);
			first = prev;
			return PushManyFastReversed(first, last);
		}

		// Constant complexity instead of linear for normal pop.
		// Works the best, when the queue is used as an
		// intermediate store, and the order in it is not
		// important.
		inline T*
		PopAllFastReversed()
		{
			return myHead.exchange(nullptr);
		}

		inline T*
		PopAll(
			T*& aOutTail)
		{
			T* curr = PopAllFastReversed();
			aOutTail = curr;
			T* prev = nullptr;
			T* next = nullptr;
			while (curr != nullptr)
			{
				next = curr->*myNext;
				curr->*myNext = prev;
				prev = curr;
				curr = next;
			}
			return prev;
		}

		inline T*
		PopAll()
		{
			T* tail;
			return PopAll(tail);
		}

	private:
		bool
		PrivPushRange(
			T* aFirst,
			T* aLast)
		{
			T* oldHead = myHead.load();
			do
			{
				aLast->*myNext = oldHead;
			} while (!myHead.compare_exchange_weak(oldHead, aFirst));
			return oldHead == nullptr;
		}

		std::atomic<T*> myHead;
	};

}
}
