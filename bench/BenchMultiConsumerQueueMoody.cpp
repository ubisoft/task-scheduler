#include "Bench.h"

#include "mg/common/Atomic.h"
#include "mg/common/ForwardList.h"
#include "mg/common/Mutex.h"

#include "BenchMultiConsumerQueueMoody.h"

namespace mg {
namespace bench {

	class BenchQueueConsumer;

	struct BenchValue
	{
		BenchValue();

		BenchValue* myNext;
	};

	using BenchValueList = mg::common::ForwardList<BenchValue>;
	using MoodyQueue = moodycamel::ConcurrentQueue<BenchValue*>;

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

		void Reserve(
			uint32_t aCount);

	private:
		MoodyQueue myQueue;

		friend class BenchQueueConsumer;
	};

	class BenchQueueConsumer
	{
	public:
		BenchQueueConsumer();

		~BenchQueueConsumer();

		BenchValue* Pop();

		void Attach(
			BenchQueue* aQueue);

		void Detach();
	private:
		MoodyQueue* myQueue;
	};

	//////////////////////////////////////////////////////////////////////////////////////

	BenchValue::BenchValue()
		: myNext(nullptr)
	{
	}

	BenchQueue::BenchQueue(
		uint32_t /*aSubQueueSize*/)
		: myQueue(32)
	{
	}

	void
	BenchQueue::Push(
		BenchValue* aValue)
	{
		bool rc = myQueue.enqueue(aValue);
		MG_COMMON_ASSERT(rc);
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

	BenchQueueConsumer::~BenchQueueConsumer()
	{
	}

	BenchValue*
	BenchQueueConsumer::Pop()
	{
		BenchValue* res = nullptr;
		if (myQueue->try_dequeue(res))
			return res;
		return nullptr;
	}

	void
	BenchQueueConsumer::Attach(
		BenchQueue* aQueue)
	{
		MG_COMMON_ASSERT(myQueue == nullptr);
		myQueue = &aQueue->myQueue;

	}

	void
	BenchQueueConsumer::Detach()
	{
		myQueue = nullptr;
	}

}
}

#include "BenchMultiConsumerQueueTemplate.hpp"
