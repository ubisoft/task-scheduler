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

#include "Bench.h"

#include "mg/common/Atomic.h"
#include "mg/common/ForwardList.h"
#include "mg/common/Mutex.h"

namespace mg {
namespace bench {

	struct BenchValue
	{
		BenchValue();

		BenchValue* myNext;
	};

	using BenchValueList = mg::common::ForwardList<BenchValue>;

	// Trivial queue takes a mutex lock on each operation. The simplest possible
	// implementation and the most typical one. And most of the time it is enough.
	//
	class BenchQueue
	{
	public:
		BenchQueue(
			uint32 aSubQueueSize);

		void Push(
			BenchValue* aValue);

		BenchValue* Pop();

		void Reserve(
			uint32 aCount);

	private:
		mg::common::Mutex myMutex;
		BenchValueList myItems;
	};

	class BenchQueueConsumer
	{
	public:
		BenchQueueConsumer();

		BenchValue* Pop();

		void Attach(
			BenchQueue* aQueue);

		void Detach();
	private:
		BenchQueue* myQueue;
	};

	//////////////////////////////////////////////////////////////////////////////////////

	BenchValue::BenchValue()
		: myNext(nullptr)
	{
	}

	BenchQueue::BenchQueue(
		uint32 /*aSubQueueSize*/)
	{
	}

	void
	BenchQueue::Push(
		BenchValue* aValue)
	{
		myMutex.Lock();
		myItems.Append(aValue);
		myMutex.Unlock();
	}

	BenchValue*
	BenchQueue::Pop()
	{
		BenchValue* res = nullptr;
		myMutex.Lock();
		if (!myItems.IsEmpty())
			res = myItems.PopFirst();
		myMutex.Unlock();
		return res;
	}

	void
	BenchQueue::Reserve(
		uint32 /*aCount*/)
	{
	}

	BenchQueueConsumer::BenchQueueConsumer()
		: myQueue(nullptr)
	{
	}

	BenchValue*
	BenchQueueConsumer::Pop()
	{
		return myQueue->Pop();
	}

	void
	BenchQueueConsumer::Attach(
		BenchQueue* aQueue)
	{
		myQueue = aQueue;
	}

	void
	BenchQueueConsumer::Detach()
	{
		myQueue = nullptr;
	}

}
}

#include "BenchMultiConsumerQueueTemplate.hpp"
