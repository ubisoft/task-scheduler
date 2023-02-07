#include "Bench.h"

#include "mg/common/ForwardList.h"
#include "mg/common/Mutex.h"

namespace mg {
namespace bench {

	struct BenchValue
	{
		BenchValue();

		BenchValue* myNext;
	};

	// Trivial queue takes a mutex lock on each operation. The simplest possible
	// implementation and the most typical one. And most of the time it is enough.
	//
	class MultiProducerQueue
	{
	public:
		bool Push(
			BenchValue* aValue);

		BenchValue* PopAll(
			BenchValue*& aOutTail);

	private:
		mg::common::Mutex myLock;
		mg::common::ForwardList<BenchValue> myValues;
	};

	inline
	BenchValue::BenchValue()
		: myNext(nullptr)
	{
	}

	inline bool
	MultiProducerQueue::Push(
		BenchValue* aValue)
	{
		myLock.Lock();
		bool res = myValues.IsEmpty();
		myValues.Append(aValue);
		myLock.Unlock();
		return res;
	}

	inline BenchValue*
	MultiProducerQueue::PopAll(
		BenchValue*& aOutTail)
	{
		myLock.Lock();
		BenchValue* head = myValues.PopAll(aOutTail);
		myLock.Unlock();
		return head;
	}

}
}

#include "BenchMultiProducerQueueTemplate.hpp"
