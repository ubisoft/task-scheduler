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
			uint32_t aSubQueueSize);

		void Push(
			BenchValue* aValue);

		BenchValue* Pop();

		void Reserve(
			uint32_t aCount);

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
		uint32_t /*aSubQueueSize*/)
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
		uint32_t /*aCount*/)
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
