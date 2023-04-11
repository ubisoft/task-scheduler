#pragma once

#include "mg/common/Atomic.h"
#include "mg/common/Mutex.h"

namespace mg {
namespace common {

	// Multi-Consumer-Single-Producer queue for void pointers.
	// It is used as a base for the templated queue able to store
	// pointers of any type.
	// Having the base separated helps to generate less code in
	// the final executable file, because it is not templated.
	// And the templated queue consists only of inlined 1-line
	// proxy calls to the base queue.

	class MCQBaseSubQueue;
	class MCQBaseQueue;

	// In order to make the queue semi-lock-free, the consumers
	// must exist as separate objects. They cache some important
	// values, which can be updated or read only under a lock.
	//
	// Semi-lock-free means, that the queue uses locks but
	// extremely rare (depending on sub-queue size). Most of the
	// time push and pop are lock-free and wait-free. That gives
	// significant performance improvement compared to a naive
	// completely lock-based queue, which takes a lock on each
	// push and each pop. The higher is contention, the more is
	// the difference. For instance, on some microbenchmarks this
	// queue is about 8 times faster than a lock-based queue.
	//
	class MCQBaseConsumer
	{
	public:
		MCQBaseConsumer();

		MCQBaseConsumer(
			MCQBaseQueue* aQueue);

		~MCQBaseConsumer();

		void* Pop();

		void Attach(
			MCQBaseQueue* aQueue);

		void Detach();

	private:
		MCQBaseSubQueue* mySubQueue;
		MCQBaseQueue* myQueue;
	};

	// Semi-lock-free Multi-Consumer-Single-Producer queue. The
	// key idea behind making it so fast is batching. The queue
	// is a lock-based list of lock-free queues of fixed size.
	//
	// The queue can't be a plain list, because it would need to
	// have separate head and tail in order to be able to pop
	// elements one by one. And these two values can't be updated
	// in a lock-free and wait-free manner. It is either
	// lock-free, but with spin-lock waits, or it is wait-free,
	// but completely based on locks.
	//
	// The queue can't be a lock-free array, because then it would
	// be bounded. Could store only limited number of items.
	//
	// This queue uses a lock-based list of lock-free bounded
	// sub-queues so while one sub-queue is not fully read by
	// consumers, its access is lock-free. The lock is taken only
	// when switch from one sub-queue to another, which is super
	// rare.
	//
	class MCQBaseQueue
	{
	public:
		MCQBaseQueue(
			uint32_t aSubQueueSize);

		~MCQBaseQueue();

		// Returns true if the queue became non-empty after this
		// push. Saves one atomic operation on not making a
		// separate count check after push.
		// Can be used by user's code so as to emit a signal to
		// the threads, waiting for the queue to become not empty.
		// The push flushes all pending elements.
		bool Push(
			void* aItem);

		// Pending push is an instrument of bulk insertion. It
		// attempts to add a new item but not make it visible to
		// consumers right away, and not increase the queue size.
		// If this happens successfully, there is not a single
		// atomic operation or a lock used. Just a few writes.
		// Which can be much faster than a plain push. But this
		// push may trigger flush of the pending elements, if the
		// current sub-queue is full.
		//
		// It returns true in case flush was triggered, and the
		// queue became non-empty because of that.
		bool PushPending(
			void* aItem);

		// Flush all pending elements. It returns true if the
		// queue became non-empty after the flush.
		bool FlushPending();

		// Make sure the queue has enough sub-queues to fit the
		// given number of elements. Usually it is not needed in
		// real code, because the queue eventually will stabilize
		// itself, and won't allocate anything.
		// This is mainly for the benchmarks, when it is
		// interesting to see pure performance not affected by the
		// queue warm up.
		void Reserve(
			uint32_t aCount);

		uint32_t SubQueueCount();

		uint32_t ConsumerCount();

		uint32_t Count();

	private:
		void PrivNextWpos();

		void PrivGarbageCollectLocked(
			MCQBaseSubQueue* aItem);

		// Fields accessed by all participants.
		int32_t myCount;
		MG_UNUSED_MEMBER char myFalseSharingProtection1[MG_CACHE_LINE_SIZE];
		// Producer-exclusive fields.
		int32_t myPendingCount;
		MG_UNUSED_MEMBER char myFalseSharingProtection2[MG_CACHE_LINE_SIZE];
		// Fields below are accessed *also* from all participants,
		// but changed much rarer. So their cache line has more
		// chances to stay in the cache for longer time, than for
		// the fields above.

		mg::common::Mutex myLock;
		int32_t mySubQueueCount;
		int32_t myConsumerCount;
		MCQBaseSubQueue* myHead;
		MCQBaseSubQueue* myWpos;
		MCQBaseSubQueue* myTail;

		friend MCQBaseConsumer;
	};

	inline uint32_t
	MCQBaseQueue::SubQueueCount()
	{
		return mg::common::AtomicLoad(&mySubQueueCount);
	}

	inline uint32_t
	MCQBaseQueue::ConsumerCount()
	{
		return mg::common::AtomicLoad(&myConsumerCount);
	}

	inline uint32_t
	MCQBaseQueue::Count()
	{
		return mg::common::AtomicLoad(&myCount);
	}

}
}
