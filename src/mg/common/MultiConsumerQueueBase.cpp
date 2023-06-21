#include "MultiConsumerQueueBase.h"

#include "mg/common/Assert.h"

namespace mg {
namespace common {

	// Lock-free bounded queue. It is Multi-Consumer-Single-
	// Producer. Multi-Producer is not supported, because costs a
	// few more atomic operations for pushing new elements, and is
	// not needed anyway. So far.
	//
	class MCQBaseSubQueue
	{
	public:
		MCQBaseSubQueue(
			uint32_t aSize);

		~MCQBaseSubQueue();

		void Reset();

		bool Push(
			void* aItem);

		bool PushPending(
			void* aItem);

		bool FlushPending();

		bool Pop(
			void*& aOutItem);

		bool IsFull() const;

		// Read-index. This is where the next consumer will try to
		// read.
		mg::common::AtomicU32 myReadIndex;
		//
		// Separate consumers from the producer explicitly. Otherwise
		// false sharing significantly hits performance in benchmarks.
		char myFalseSharingProtection1[MG_CACHE_LINE_SIZE];
		//
		// Flush-index. This is the latest index visible to the
		// readers. It always points at nullptr, and the element
		// under it must be updated atomically. Consumers see data
		// [rindex, findex], and all elements here should be
		// updated atomically.
		uint32_t myFlushIndex;
		// Write-index. It can be beyond flush index in case the
		// sub-queue contains pending elements. Elements
		// (findex, windex] are visible only to the producer, so
		// can be updated non-atomically.
		uint32_t myWriteIndex;
		// First pending element. It is saved here to be stored to
		// the flush-index position upon flush or normal push to
		// notify consumers that there is more to consume.
		// Only the first pending element needs to be stored and
		// the rest can be written to write-index positions as
		// only flush-index is visible to consumers. Consider an
		// example, when 3 elements are pushed. 'R' - rindex,
		// 'F' - findex, 'W' - windex.
		//
		//     queue: [null, null, null], saved = null
		//             r,f,w
		//
		// Consumers don't see anything.
		//
		//     pushpending(a)
		//     queue: [null, null, null], saved = a
		//             r,f   w
		//
		//     pushpending(b)
		//     queue: [null, b,    null], saved = a
		//             r,f         w
		//
		// Note, 'b' is pushed, but not visible to the consumers.
		// Also look at how 'w' moved, for the next pending
		// element.
		// Rindex is still at null. consumers still wait for the
		// first element become not null.
		//
		//     pushpending(c)
		//     queue: [null, b,    c],    saved = a
		//             r,f           w
		//
		// Now flush. With a single atomic operation, 'a' is moved
		// to its position (first), and all the 3 elements become
		// visible.
		//
		//     flushpending()
		//     queue: [a,    b,    c],    saved = null
		//             r             f,w
		//
		void* myFirstPending;
		// Order of the indexes is always the following:
		//
		//     rindex <= findex <= windex.

		// The members below are accessed by consumers and producers
		// together. Prevent them from spoiling the consumer-exclusive
		// and producer-exclusive memory in the CPU caches.
		char myFalseSharingProtection2[MG_CACHE_LINE_SIZE];
		// Fields below are accessed *also* from all participants,
		// but changed much rarer or are not changed at all. So
		// their cache line has more chances to stay in the cache
		// for longer time, than for the fields above.

		// The queue pointer never changes, this is correct. The
		// elements are changed, but not the pointer at the queue
		// itself.
		mg::common::AtomicVoidPtr* const myQueue;
		const uint32_t mySize;
		uint32_t myConsumerCount;
		MCQBaseSubQueue* myPrev;
		MCQBaseSubQueue* myNext;
	};

	MCQBaseSubQueue::MCQBaseSubQueue(
		uint32_t aSize)
		: myReadIndex(aSize)
		, myFlushIndex(aSize)
		, myWriteIndex(aSize)
		// + 1 because the queue is null-terminated. See the
		// methods implementation why.
		, myFirstPending(nullptr)
		, myQueue(new mg::common::AtomicVoidPtr[aSize + 1])
		, mySize(aSize)
		, myConsumerCount(0)
		, myPrev(nullptr)
		, myNext(nullptr)
	{
		Reset();
	}

	MCQBaseSubQueue::~MCQBaseSubQueue()
	{
		MG_DEV_ASSERT(myConsumerCount == 0);
		MG_DEV_ASSERT(myPrev == nullptr);
		MG_DEV_ASSERT(myNext == nullptr);
		delete[] myQueue;
	}

	void
	MCQBaseSubQueue::Reset()
	{
		MG_DEV_ASSERT(myConsumerCount == 0);
		MG_DEV_ASSERT(myPrev == nullptr);
		MG_DEV_ASSERT(myNext == nullptr);
		MG_DEV_ASSERT(myReadIndex.LoadRelaxed() == mySize);
		MG_DEV_ASSERT(myWriteIndex == mySize);
		MG_DEV_ASSERT(myFlushIndex == mySize);
		MG_DEV_ASSERT(myFirstPending == nullptr);
		myReadIndex.StoreRelaxed(0);
		myFlushIndex = 0;
		myWriteIndex = 0;
		// Safe to be relaxed. Consumers will attach to this queue via
		// the mutex which will sync them with the producer doing this
		// store also inside the mutex.
		myQueue[0].StoreRelaxed(nullptr);
	}

	bool
	MCQBaseSubQueue::Push(
		void* aItem)
	{
		// Null is a special value in the queue. It can't be used
		// as a queue item. In case that will ever become
		// necessary, it is possible to use some other value. For
		// instance, the queue pointer itself. But the user code
		// can just do the same, if wants kind of a special value.
		MG_DEV_ASSERT(aItem != nullptr);

		uint32_t windex = myWriteIndex;
		if (windex == mySize)
			return false;

		// The queue is like a null-terminated string. Except that
		// null here signals that it is the end of the queue. It
		// is used by readers. So when a new element is being
		// added, it is *always* added into a null-cell. But also
		// it *shall* ensure that the next cell will become null
		// instead.
		// This simple trick allows to avoid doing memcpy(0) for
		// the whole queue when it is reset or created. Such a big
		// memcpy would wash out all the caches.
		// It is also safe to set the next element without a
		// border check, because the sub-queue always allocates +1
		// element in the end exactly for this. That allows not to
		// do sub-queue-size 'if' checks in exchange on one excess
		// 'store' instruction.
		uint32_t nextWindex = windex + 1;
		uint32_t findex = myFlushIndex;
		// Use release for all queue items because consumers will sync
		// with the same items using acquire.
		myQueue[nextWindex].StoreRelease(nullptr);
		myWriteIndex = nextWindex;
		myFlushIndex = nextWindex;

		void* old = myFirstPending;
		if (old != nullptr)
		{
			MG_DEV_ASSERT(windex > findex);
			myQueue[windex].StoreRelease(aItem);
			aItem = old;
			myFirstPending = nullptr;
		}
		old = myQueue[findex].ExchangeRelease(aItem);
		MG_DEV_ASSERT(old == nullptr);
		return true;
	}

	bool
	MCQBaseSubQueue::PushPending(
		void* aItem)
	{
		// Null is a special value in the queue. It can't be used
		// as a queue item. In case that will ever become
		// necessary, it is possible to use some other value. For
		// instance, the queue pointer itself. But the user code
		// can just do the same, if wants kind of a special value.
		MG_DEV_ASSERT(aItem != nullptr);

		uint32_t windex = myWriteIndex;
		if (windex == mySize)
			return false;

		// Can't write the first element right away. Otherwise it would
		// become visible to the consumers which is not the point of the
		// pending push. Save it to push later.
		//
		// The non-first pending items have to use the release barrier
		// even though they are not visible to the consumers yet.
		// Because otherwise when they are exposed, acquire on them
		// wouldn't guarantee sync with all writes done prior to their
		// push. Plain release barrier on a specific memory spot won't
		// sync with acquire barriers on other spots.
		if (myFirstPending == nullptr)
			myFirstPending = aItem;
		else
			myQueue[windex].StoreRelease(aItem);
		myWriteIndex = windex + 1;
		return true;
	}

	bool
	MCQBaseSubQueue::FlushPending()
	{
		if (myFirstPending == nullptr)
			return false;
		uint32_t windex = myWriteIndex;
		uint32_t findex = myFlushIndex;
		MG_DEV_ASSERT(windex > findex);
		// All have to be release to sync with consumers doing acquire
		// on the same locations.
		myQueue[windex].StoreRelease(nullptr);
		void* old = myQueue[findex].ExchangeRelease(myFirstPending);
		MG_DEV_ASSERT(old == nullptr);
		MG_UNUSED(old);
		myFirstPending = nullptr;
		myFlushIndex = windex;
		return true;
	}

	bool
	MCQBaseSubQueue::Pop(
		void*& aOutItem)
	{
		// Pop works in 3 steps: remember the current read-index,
		// load the element, and try to update the read-index if
		// it is still the same. It can be not the same if another
		// thread simultaneously read the same element and already
		// returned it. In this case the current thread will
		// retry.
		uint32_t rindex = myReadIndex.LoadRelaxed();
		// The loop is not busy, is not a lock, and is not a 'waiting'.
		// Because lock or waiting would mean the thread is blocked on
		// something not yet done by another thread. Here it is not so -
		// the thread is never blocked and always tries to progress. And
		// it progresses in a single iteration almost always.
		do
		{
			if (rindex == mySize)
				return false;
			// Read index itself is fine to be relaxed. Broader sync
			// is only needed for the queue items. To get all the
			// writes done before their push which might be related
			// to them.
			aOutItem = myQueue[rindex].LoadAcquire();
			if (aOutItem == nullptr)
				return true;
		} while (!myReadIndex.CmpExchgWeakRelaxed(rindex, rindex + 1));
		return true;
	}

	inline bool
	MCQBaseSubQueue::IsFull() const
	{
		return myFlushIndex == mySize;
	}

	MCQBaseConsumer::MCQBaseConsumer()
		: mySubQueue(nullptr)
		, myQueue(nullptr)
	{
	}

	MCQBaseConsumer::MCQBaseConsumer(
		MCQBaseQueue* aQueue)
		: mySubQueue(nullptr)
		, myQueue(nullptr)
	{
		Attach(aQueue);
	}

	MCQBaseConsumer::~MCQBaseConsumer()
	{
		Detach();
	}

	void*
	MCQBaseConsumer::Pop()
	{
		MCQBaseSubQueue* cur = mySubQueue;
		void* res;

		// Fast path - pop returns something right away - no
		// locks.
		while (!cur->Pop(res))
		{
			// Slow path - the current sub-queue is exhausted.

			// Here are some ideas how it works.
			// The queue has a list of sub-queues, and maintains
			// 3 positions in them: head, wpos, and tail.
			//
			// * Producer always writes to the wpos, and may add
			// new sub-queues to the tail. It does not care about
			// the head.
			//
			// * Each consumer is somewhere between [head, wpos].
			// Also they move empty and fully read-up sub-queues
			// from the head to the tail (recycle them).
			//
			// * Consumers don't recycle the head right away when
			// it is fully consumed. Because there still may be
			// other consumers, who refer to it. If it would be
			// recycled anyway, the producer may get to it, fill
			// with more elements, and *they would be read out of
			// order!!!*. The head is recycled only when the last
			// consumer leaves it.
			//
			// * Since consumers can't rely on head being not
			// already consumed, they iterate over the sub-queue
			// list starting from their current position. They
			// use the head only when are created. Then each
			// consumer maintains its own position in the
			// sub-queue list.

			// All sub-queue list manipulations shall be done with
			// a lock.
			myQueue->myLock.Lock();
			if (cur == myQueue->myWpos)
			{
				myQueue->myLock.Unlock();
				return nullptr;
			}
			--cur->myConsumerCount;
			MCQBaseSubQueue* next = cur->myNext;
			myQueue->PrivGarbageCollectLocked(cur);
			cur = next;
			++cur->myConsumerCount;
			myQueue->myLock.Unlock();

			mySubQueue = cur;
		}
		if (res != nullptr)
			myQueue->myCount.DecrementRelaxed();
		return res;
	}

	void
	MCQBaseConsumer::Attach(
		MCQBaseQueue* aQueue)
	{
		aQueue->myConsumerCount.IncrementRelaxed();

		aQueue->myLock.Lock();
		MCQBaseSubQueue* head = aQueue->myHead;
		++head->myConsumerCount;
		aQueue->myLock.Unlock();

		mySubQueue = head;
		myQueue = aQueue;
	}

	void
	MCQBaseConsumer::Detach()
	{
		if (myQueue == nullptr)
			return;
		myQueue->myConsumerCount.DecrementRelaxed();

		myQueue->myLock.Lock();
		--mySubQueue->myConsumerCount;
		myQueue->PrivGarbageCollectLocked(mySubQueue);
		myQueue->myLock.Unlock();

		mySubQueue = nullptr;
		myQueue = nullptr;
	}

	MCQBaseQueue::MCQBaseQueue(
		uint32_t aSubQueueSize)
		: myCount(0)
		, myPendingCount(0)
		, mySubQueueCount(1)
		, myConsumerCount(0)
	{
		MG_COMMON_ASSERT(aSubQueueSize > 0);
		myHead = new MCQBaseSubQueue(aSubQueueSize);
		myWpos = myHead;
		myTail = myHead;
	}

	MCQBaseQueue::~MCQBaseQueue()
	{
		myLock.Lock();
		MG_COMMON_ASSERT(myConsumerCount.LoadRelaxed() == 0);
		uint32_t subQueueCount = 0;
		while (myHead != nullptr)
		{
			MCQBaseSubQueue* next = myHead->myNext;
			myHead->myPrev = nullptr;
			myHead->myNext = nullptr;
			delete myHead;
			myHead = next;
			++subQueueCount;
		}
		MG_COMMON_ASSERT(subQueueCount ==
			mySubQueueCount.LoadRelaxed());
		myLock.Unlock();
	}

	bool
	MCQBaseQueue::Push(
		void* aItem)
	{
		int32_t count = myPendingCount + 1;
		myPendingCount = 0;
		// It is very important to update the count before pushing
		// the element. Otherwise a consumer may pop it, decrement
		// the count, and may make it negative.
		bool becameNotEmpty = myCount.FetchAddRelaxed(count) == 0;

		// Fast-path - almost always a lock-free push.
		if (myWpos->Push(aItem))
			return becameNotEmpty;

		// Slow-path - the sub-queue is full. Time to switch.
		myWpos->FlushPending();
		PrivNextWpos();
		myWpos->Push(aItem);
		return becameNotEmpty;
	}

	bool
	MCQBaseQueue::PushPending(
		void* aItem)
	{
		// Here it is important not to increase the queue size.
		// Because pending elements are not visible to consumers
		// until a flush. It wouldn't be fair to increase the
		// count without having new elements visible.

		// Fast-path - almost always a lock-free push.
		if (myWpos->PushPending(aItem))
		{
			++myPendingCount;
			return false;
		}
		// Slow-path - the sub-queue is full. Time to switch.
		bool becameNotEmpty = FlushPending();
		PrivNextWpos();

		myWpos->PushPending(aItem);
		++myPendingCount;
		return becameNotEmpty;
	}

	bool
	MCQBaseQueue::FlushPending()
	{
		if (myPendingCount == 0)
			return false;
		bool becameNotEmpty = myCount.FetchAddRelaxed(
			myPendingCount) == 0;
		myPendingCount = 0;
		myWpos->FlushPending();
		return becameNotEmpty;
	}

	void
	MCQBaseQueue::Reserve(
		uint32_t aCount)
	{
		// The reservation is used for testing only. Out of
		// testing it could only be used once before the load
		// comes. So does not make sense to optimize it much.
		myLock.Lock();
		uint32_t size = myTail->mySize;
		aCount = aCount / size + (aCount % size != 0);
		uint32_t count = mySubQueueCount.LoadRelaxed();
		if (count >= aCount)
		{
			myLock.Unlock();
			return;
		}
		count = aCount - count;
		MCQBaseSubQueue* head = new MCQBaseSubQueue(size);
		MCQBaseSubQueue* tail = head;
		for (uint32_t i = 1; i < count; ++i)
		{
			MCQBaseSubQueue* next = new MCQBaseSubQueue(size);
			tail->myNext = next;
			next->myPrev = tail;
			tail = next;
		}
		mySubQueueCount.AddRelaxed(count);

		head->myPrev = myTail;
		myTail->myNext = head;
		myTail = tail;
		myLock.Unlock();
	}

	void
	MCQBaseQueue::PrivNextWpos()
	{
		MG_DEV_ASSERT(myPendingCount == 0);
		MG_DEV_ASSERT(myWpos->IsFull());
		// All the sub-queue list modifications must be protected
		// with a lock.
		myLock.Lock();
		myWpos = myWpos->myNext;
		if (myWpos == nullptr)
		{
			// Size is the same in all sub-queues, and also it is
			// constant. So can be accessed freely. No need to
			// store the same size in the root queue.
			myWpos = new MCQBaseSubQueue(myTail->mySize);
			mySubQueueCount.IncrementRelaxed();
			myWpos->myPrev = myTail;
			myTail->myNext = myWpos;
			myTail = myWpos;
		}
		myLock.Unlock();
	}

	void
	MCQBaseQueue::PrivGarbageCollectLocked(
		MCQBaseSubQueue* aItem)
	{
		MG_DEV_ASSERT(myLock.IsOwnedByThisThread());
		// Write position is occupied by producer. More data may
		// be written to it, so don't touch.
		if (aItem == myWpos)
			return;
		// The sub-queue is still referenced by some consumer.
		// Can't recycle. It would be as bad as just calling
		// delete on it. Last consumer working with this sub-queue
		// will recycle it later.
		if (aItem->myConsumerCount != 0)
			return;
		// Not all elements are read, obviously can't recycle.
		// Note, that it is safe to read rindex here. Because the
		// sub-queue is not referenced by any consumer, and
		// therefore rindex can't change here concurrently.
		if (aItem->myReadIndex.LoadRelaxed() < aItem->mySize)
			return;
		// Can't be last - wpos is always ahead and this item is not wpos.
		MCQBaseSubQueue* next = aItem->myNext;
		MCQBaseSubQueue* prev = aItem->myPrev;
		MG_DEV_ASSERT(next != nullptr);
		if (prev == nullptr)
		{
			MG_DEV_ASSERT(aItem == myHead);
			next->myPrev = nullptr;
			myHead = next;
		}
		else
		{
			prev->myNext = next;
			next->myPrev = prev;
		}
		// XXX: it may make sense to introduce a smarter recycling
		// technique. For example, if the number of free
		// sub-queues becomes equal to the number of not consumed
		// sub-queues, then stop recycling and use delete.
		aItem->myNext = nullptr;
		aItem->myPrev = nullptr;
		aItem->Reset();

		myTail->myNext = aItem;
		aItem->myPrev = myTail;
		myTail = aItem;
	}

}
}
