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

#include "mg/common/Atomic.h"
#include "mg/common/Types.h"

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
			return mg::common::AtomicLoadPtr(&myHead) == nullptr;
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
			return mg::common::AtomicExchangePtr(&myHead, (T*)nullptr);
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
			T* oldHead;
			do
			{
				oldHead = mg::common::AtomicLoadPtr(&myHead);
				aLast->*myNext = oldHead;
			} while (mg::common::AtomicCompareExchangePtr(
				&myHead, aFirst, oldHead
			) != oldHead);
			return oldHead == nullptr;
		}

		T* myHead;
	};

}
}
