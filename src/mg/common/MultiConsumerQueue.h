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

#include "mg/common/MultiConsumerQueueBase.h"

namespace mg {
namespace common {

	// This whole file is a type-friendly wrapper for the base
	// queue. For algorithm and API details go to there.

	template<typename T>
	class MultiConsumerQueueConsumer;

	template<typename T>
	class MultiConsumerQueue
	{
	public:
		MultiConsumerQueue(
			uint32 aSubQueueSize);

		bool Push(
			T* aItem);

		bool PushPending(
			T* aItem);

		bool FlushPending();

		void Reserve(
			uint32 aCount);

		uint32 SubQueueCount();

		uint32 ConsumerCount();

		uint32 Count();

	private:
		MCQBaseQueue myBase;

		friend class MultiConsumerQueueConsumer<T>;
	};

	template<typename T>
	class MultiConsumerQueueConsumer
	{
	public:
		MultiConsumerQueueConsumer();

		MultiConsumerQueueConsumer(
			MultiConsumerQueue<T>* aQueue);

		T* Pop();

		void Attach(
			MultiConsumerQueue<T>* aQueue);

		void Detach();

	private:
		MCQBaseConsumer myBase;
	};

	template<typename T>
	inline
	MultiConsumerQueue<T>::MultiConsumerQueue(
		uint32 aSubQueueSize)
		: myBase(aSubQueueSize)
	{
	}

	template<typename T>
	inline bool
	MultiConsumerQueue<T>::Push(
		T* aItem)
	{
		return myBase.Push((void*) aItem);
	}

	template<typename T>
	inline bool
	MultiConsumerQueue<T>::PushPending(
		T* aItem)
	{
		return myBase.PushPending((void*) aItem);
	}

	template<typename T>
	inline bool
	MultiConsumerQueue<T>::FlushPending()
	{
		return myBase.FlushPending();
	}

	template<typename T>
	inline void
	MultiConsumerQueue<T>::Reserve(
		uint32 aCount)
	{
		myBase.Reserve(aCount);
	}

	template<typename T>
	inline uint32
	MultiConsumerQueue<T>::SubQueueCount()
	{
		return myBase.SubQueueCount();
	}

	template<typename T>
	inline uint32
	MultiConsumerQueue<T>::ConsumerCount()
	{
		return myBase.ConsumerCount();
	}

	template<typename T>
	inline uint32
	MultiConsumerQueue<T>::Count()
	{
		return myBase.Count();
	}

	template<typename T>
	MultiConsumerQueueConsumer<T>::MultiConsumerQueueConsumer()
	{
	}

	template<typename T>
	MultiConsumerQueueConsumer<T>::MultiConsumerQueueConsumer(
		MultiConsumerQueue<T>* aQueue)
		: myBase(&aQueue->myBase)
	{
	}

	template<typename T>
	inline T*
	MultiConsumerQueueConsumer<T>::Pop()
	{
		return (T*) myBase.Pop();
	}

	template<typename T>
	inline void
	MultiConsumerQueueConsumer<T>::Attach(
		MultiConsumerQueue<T>* aQueue)
	{
		myBase.Attach(&aQueue->myBase);
	}

	template<typename T>
	inline void
	MultiConsumerQueueConsumer<T>::Detach()
	{
		myBase.Detach();
	}

}
}
