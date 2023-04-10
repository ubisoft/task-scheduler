// Provides a C++11 implementation of a multi-producer, multi-consumer lock-free queue.
// An overview, including benchmark results, is provided here:
//     http://moodycamel.com/blog/2014/a-fast-general-purpose-lock-free-queue-for-c++
// The full design is also described in excruciating detail at:
//    http://moodycamel.com/blog/2014/detailed-design-of-a-lock-free-queue

// Simplified BSD license:
// Copyright (c) 2013-2020, Cameron Desrochers.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// - Redistributions of source code must retain the above copyright notice, this list of
// conditions and the following disclaimer.
// - Redistributions in binary form must reproduce the above copyright notice, this list of
// conditions and the following disclaimer in the documentation and/or other materials
// provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
// OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
// TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Also dual-licensed under the Boost Software License (see LICENSE.md)

#pragma once

#if defined(__GNUC__) && !defined(__INTEL_COMPILER)
// Disable -Wconversion warnings (spuriously triggered when Traits::size_t and
// Traits::index_t are set to < 32 bits, causing integer promotion, causing warnings
// upon assigning any computed values)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#endif

#if defined(__APPLE__)
#include "TargetConditionals.h"
#endif

#include <atomic>		// Requires C++11. Sorry VS2010.
#include <cassert>
#include <cstddef>              // for max_align_t
#include <cstdint>
#include <cstdlib>
#include <type_traits>
#include <algorithm>
#include <utility>
#include <limits>
#include <climits>		// for CHAR_BIT
#include <array>
#include <thread>		// partly for __WINPTHREADS_VERSION if on MinGW-w64 w/ POSIX threading
#include <mutex>        // used for thread exit synchronization

// Constexpr if
#ifndef MOODYCAMEL_CONSTEXPR_IF
#if __cplusplus > 201402L
#define MOODYCAMEL_CONSTEXPR_IF if constexpr
#define MOODYCAMEL_MAYBE_UNUSED [[maybe_unused]]
#else
#define MOODYCAMEL_CONSTEXPR_IF if
#define MOODYCAMEL_MAYBE_UNUSED
#endif
#endif

// Exceptions
#ifndef MOODYCAMEL_EXCEPTIONS_ENABLED
#if defined(__GNUC__) && defined(__EXCEPTIONS)
#define MOODYCAMEL_EXCEPTIONS_ENABLED
#endif
#endif
#ifdef MOODYCAMEL_EXCEPTIONS_ENABLED
#define MOODYCAMEL_TRY try
#define MOODYCAMEL_CATCH(...) catch(__VA_ARGS__)
#define MOODYCAMEL_RETHROW throw
#define MOODYCAMEL_THROW(expr) throw (expr)
#else
#define MOODYCAMEL_TRY MOODYCAMEL_CONSTEXPR_IF (true)
#define MOODYCAMEL_CATCH(...) else MOODYCAMEL_CONSTEXPR_IF (false)
#define MOODYCAMEL_RETHROW
#define MOODYCAMEL_THROW(expr)
#endif

#ifndef MOODYCAMEL_NOEXCEPT
#if !defined(MOODYCAMEL_EXCEPTIONS_ENABLED)
#define MOODYCAMEL_NOEXCEPT
#define MOODYCAMEL_NOEXCEPT_CTOR(type, valueType, expr) true
#define MOODYCAMEL_NOEXCEPT_ASSIGN(type, valueType, expr) true
#else
#define MOODYCAMEL_NOEXCEPT noexcept
#define MOODYCAMEL_NOEXCEPT_CTOR(type, valueType, expr) noexcept(expr)
#define MOODYCAMEL_NOEXCEPT_ASSIGN(type, valueType, expr) noexcept(expr)
#endif
#endif

namespace moodycamel { namespace details {
#ifndef MOODYCAMEL_ALIGNAS
	template<typename T> struct identity { typedef T type; };
#define MOODYCAMEL_ALIGNED_TYPE_LIKE(T, obj) alignas(alignof(obj)) typename details::identity<T>::type
#endif
} }

namespace moodycamel {
namespace details {
	template<typename T>
	struct const_numeric_max {
		static_assert(std::is_integral<T>::value, "const_numeric_max can only be used with integers");
		static const T value = std::numeric_limits<T>::is_signed
			? (static_cast<T>(1) << (sizeof(T) * CHAR_BIT - 1)) - static_cast<T>(1)
			: static_cast<T>(-1);
	};

#if defined(__GLIBCXX__)
	typedef ::max_align_t std_max_align_t;      // libstdc++ forgot to add it to std:: for a while
#else
	typedef std::max_align_t std_max_align_t;   // Others (e.g. MSVC) insist it can *only* be accessed via std::
#endif

	// Some platforms have incorrectly set max_align_t to a type with <8 bytes alignment even while supporting
	// 8-byte aligned scalar values (*cough* 32-bit iOS). Work around this with our own union. See issue #64.
	typedef union {
		std_max_align_t x;
		long long y;
		void* z;
	} max_align_t;
}

// Default traits for the ConcurrentQueue. To change some of the
// traits without re-implementing all of them, inherit from this
// struct and shadow the declarations you wish to be different;
// since the traits are used as a template type parameter, the
// shadowed declarations will be used where defined, and the defaults
// otherwise.
struct ConcurrentQueueDefaultTraits
{
	// General-purpose size type. std::size_t is strongly recommended.
	typedef std::size_t size_t;
	
	// The type used for the enqueue and dequeue indices. Must be at least as
	// large as size_t. Should be significantly larger than the number of elements
	// you expect to hold at once, especially if you have a high turnover rate;
	// for example, on 32-bit x86, if you expect to have over a hundred million
	// elements or pump several million elements through your queue in a very
	// short space of time, using a 32-bit type *may* trigger a race condition.
	// A 64-bit int type is recommended in that case, and in practice will
	// prevent a race condition no matter the usage of the queue. Note that
	// whether the queue is lock-free with a 64-int type depends on the whether
	// std::atomic<std::uint64_t> is lock-free, which is platform-specific.
	typedef std::size_t index_t;
	
	// Internally, all elements are enqueued and dequeued from multi-element
	// blocks; this is the smallest controllable unit. If you expect few elements
	// but many producers, a smaller block size should be favoured. For few producers
	// and/or many elements, a larger block size is preferred. A sane default
	// is provided. Must be a power of 2.
	static const size_t BLOCK_SIZE = 1024 * 1024;
	
	// For explicit producers (i.e. when using a producer token), the block is
	// checked for being empty by iterating through a list of flags, one per element.
	// For large block sizes, this is too inefficient, and switching to an atomic
	// counter-based approach is faster. The switch is made for block sizes strictly
	// larger than this threshold.
	static const size_t EXPLICIT_BLOCK_EMPTY_COUNTER_THRESHOLD = 32;
	
	// How many full blocks can be expected for a single explicit producer? This should
	// reflect that number's maximum for optimal performance. Must be a power of 2.
	static const size_t EXPLICIT_INITIAL_INDEX_SIZE = 32;
	
	// The maximum number of elements (inclusive) that can be enqueued to a sub-queue.
	// Enqueue operations that would cause this limit to be surpassed will fail. Note
	// that this limit is enforced at the block level (for performance reasons), i.e.
	// it's rounded up to the nearest block size.
	static const size_t MAX_SUBQUEUE_SIZE = details::const_numeric_max<size_t>::value;

	// Whether to recycle dynamically-allocated blocks into an internal free list or
	// not. If false, only pre-allocated blocks (controlled by the constructor
	// arguments) will be recycled, and all others will be `free`d back to the heap.
	// Note that blocks consumed by explicit producers are only freed on destruction
	// of the queue (not following destruction of the token) regardless of this trait.
	static const bool RECYCLE_ALLOCATED_BLOCKS = true;
};

template<typename T, typename Traits> class ConcurrentQueue;
class ConcurrentQueueTests;


namespace details
{	
	template<typename T>
	static inline bool circular_less_than(T a, T b)
	{
		static_assert(std::is_integral<T>::value && !std::numeric_limits<T>::is_signed, "circular_less_than is intended to be used only with unsigned integer types");
		return static_cast<T>(a - b) > static_cast<T>(static_cast<T>(1) << (static_cast<T>(sizeof(T) * CHAR_BIT - 1)));
		// Note: extra parens around rhs of operator<< is MSVC bug: https://developercommunity2.visualstudio.com/t/C4554-triggers-when-both-lhs-and-rhs-is/10034931
		//       silencing the bug requires #pragma warning(disable: 4554) around the calling code and has no effect when done here.
	}
	
	template<typename U>
	static inline char* align_for(char* ptr)
	{
		const std::size_t alignment = std::alignment_of<U>::value;
		return ptr + (alignment - (reinterpret_cast<std::uintptr_t>(ptr) % alignment)) % alignment;
	}

	template<typename T>
	static inline T ceil_to_pow_2(T x)
	{
		static_assert(std::is_integral<T>::value && !std::numeric_limits<T>::is_signed, "ceil_to_pow_2 is intended to be used only with unsigned integer types");

		// Adapted from http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
		--x;
		x |= x >> 1;
		x |= x >> 2;
		x |= x >> 4;
		for (std::size_t i = 1; i < sizeof(T); i <<= 1) {
			x |= x >> (i << 3);
		}
		++x;
		return x;
	}
}

template<typename T, typename Traits = ConcurrentQueueDefaultTraits>
class ConcurrentQueue
{
public:
	typedef typename Traits::index_t index_t;
	typedef typename Traits::size_t size_t;
	
	static const size_t BLOCK_SIZE = static_cast<size_t>(Traits::BLOCK_SIZE);
	static const size_t EXPLICIT_BLOCK_EMPTY_COUNTER_THRESHOLD = static_cast<size_t>(Traits::EXPLICIT_BLOCK_EMPTY_COUNTER_THRESHOLD);
	static const size_t EXPLICIT_INITIAL_INDEX_SIZE = static_cast<size_t>(Traits::EXPLICIT_INITIAL_INDEX_SIZE);
	static const size_t MAX_SUBQUEUE_SIZE = (details::const_numeric_max<size_t>::value - static_cast<size_t>(Traits::MAX_SUBQUEUE_SIZE) < BLOCK_SIZE) ? details::const_numeric_max<size_t>::value : ((static_cast<size_t>(Traits::MAX_SUBQUEUE_SIZE) + (BLOCK_SIZE - 1)) / BLOCK_SIZE * BLOCK_SIZE);

	static_assert(!std::numeric_limits<size_t>::is_signed && std::is_integral<size_t>::value, "Traits::size_t must be an unsigned integral type");
	static_assert(!std::numeric_limits<index_t>::is_signed && std::is_integral<index_t>::value, "Traits::index_t must be an unsigned integral type");
	static_assert(sizeof(index_t) >= sizeof(size_t), "Traits::index_t must be at least as wide as Traits::size_t");
	static_assert((BLOCK_SIZE > 1) && !(BLOCK_SIZE & (BLOCK_SIZE - 1)), "Traits::BLOCK_SIZE must be a power of 2 (and at least 2)");
	static_assert((EXPLICIT_BLOCK_EMPTY_COUNTER_THRESHOLD > 1) && !(EXPLICIT_BLOCK_EMPTY_COUNTER_THRESHOLD & (EXPLICIT_BLOCK_EMPTY_COUNTER_THRESHOLD - 1)), "Traits::EXPLICIT_BLOCK_EMPTY_COUNTER_THRESHOLD must be a power of 2 (and greater than 1)");

public:
	// Creates a queue with at least `capacity` element slots; note that the
	// actual number of elements that can be inserted without additional memory
	// allocation depends on the number of producers and the block size (e.g. if
	// the block size is equal to `capacity`, only a single block will be allocated
	// up-front, which means only a single producer will be able to enqueue elements
	// without an extra allocation -- blocks aren't shared between producers).
	// This method is not thread safe -- it is up to the user to ensure that the
	// queue is fully constructed before it starts being used by other threads (this
	// includes making the memory effects of construction visible, possibly with a
	// memory barrier).
	explicit ConcurrentQueue(size_t capacity = 32 * BLOCK_SIZE)
		: initialBlockPoolIndex(0)
	{
		producer = new ExplicitProducer(this);
		populate_initial_block_list(capacity / BLOCK_SIZE + ((capacity & (BLOCK_SIZE - 1)) == 0 ? 0 : 1));
	}
	
	// Note: The queue should not be accessed concurrently while it's
	// being deleted. It's up to the user to synchronize this.
	// This method is not thread safe.
	~ConcurrentQueue()
	{
		destroy(producer);	
		// Destroy global free list
		auto block = freeList.head_unsafe();
		while (block != nullptr) {
			auto next = block->freeListNext.load(std::memory_order_seq_cst);
			if (block->dynamicallyAllocated) {
				destroy(block);
			}
			block = next;
		}
		
		// Destroy initial free list
		destroy_array(initialBlockPool, initialBlockPoolSize);
	}

	// Disable copying and copy assignment
	ConcurrentQueue(ConcurrentQueue const&) = delete;
	ConcurrentQueue& operator=(ConcurrentQueue const&) = delete;

	// Enqueues a single item (by copying it) using an explicit producer token.
	// Allocates memory if required. Only fails if memory allocation fails (or
	// Traits::MAX_SUBQUEUE_SIZE has been defined and would be surpassed).
	// Thread-safe.
	inline bool enqueue(T const& item)
	{
		return inner_enqueue(item);
	}
	
	// Enqueues a single item (by moving it, if possible) using an explicit producer token.
	// Allocates memory if required. Only fails if memory allocation fails (or
	// Traits::MAX_SUBQUEUE_SIZE has been defined and would be surpassed).
	// Thread-safe.
	inline bool enqueue(T&& item)
	{
		return inner_enqueue(std::move(item));
	}
	
	// Attempts to dequeue from the queue using an explicit consumer token.
	// Returns false if all producer streams appeared empty at the time they
	// were checked (so, the queue is likely but not guaranteed to be empty).
	// Never allocates. Thread-safe.
	template<typename U>
	bool try_dequeue(U& item)
	{
		// If there was at least one non-empty queue but it appears empty at the time
		// we try to dequeue from it, we need to make sure every queue's been tried
		return producer->dequeue(item);
	}

private:
	struct ExplicitProducer;
	friend struct ExplicitProducer;
	friend class ConcurrentQueueTests;
	
	
	///////////////////////////////
	// Queue methods
	///////////////////////////////
	
	template<typename U>
	inline bool inner_enqueue(U&& element)
	{
		return producer->enqueue(std::forward<U>(element));
	}
	
	///////////////////////////
	// Free list
	///////////////////////////
	
	template <typename N>
	struct FreeListNode
	{
		FreeListNode() : freeListRefs(0), freeListNext(nullptr) { }
		
		std::atomic<std::uint32_t> freeListRefs;
		std::atomic<N*> freeListNext;
	};
	
	// A simple CAS-based lock-free free list. Not the fastest thing in the world under heavy contention, but
	// simple and correct (assuming nodes are never freed until after the free list is destroyed), and fairly
	// speedy under low contention.
	template<typename N>		// N must inherit FreeListNode or have the same fields (and initialization of them)
	struct FreeList
	{
		FreeList() : freeListHead(nullptr) { }
		FreeList(FreeList&& other) : freeListHead(other.freeListHead.load(std::memory_order_seq_cst)) { other.freeListHead.store(nullptr, std::memory_order_seq_cst); }
		
		FreeList(FreeList const&) = delete;
		FreeList& operator=(FreeList const&) = delete;
		
		inline void add(N* node)
		{
			// We know that the should-be-on-freelist bit is 0 at this point, so it's safe to
			// set it using a fetch_add
			if (node->freeListRefs.fetch_add(SHOULD_BE_ON_FREELIST, std::memory_order_seq_cst) == 0) {
				// Oh look! We were the last ones referencing this node, and we know
				// we want to add it to the free list, so let's do it!
		 		add_knowing_refcount_is_zero(node);
			}
		}
		
		inline N* try_get()
		{
			auto head = freeListHead.load(std::memory_order_seq_cst);
			while (head != nullptr) {
				auto prevHead = head;
				auto refs = head->freeListRefs.load(std::memory_order_seq_cst);
				if ((refs & REFS_MASK) == 0 || !head->freeListRefs.compare_exchange_strong(refs, refs + 1, std::memory_order_seq_cst, std::memory_order_seq_cst)) {
					head = freeListHead.load(std::memory_order_seq_cst);
					continue;
				}
				
				// Good, reference count has been incremented (it wasn't at zero), which means we can read the
				// next and not worry about it changing between now and the time we do the CAS
				auto next = head->freeListNext.load(std::memory_order_seq_cst);
				if (freeListHead.compare_exchange_strong(head, next, std::memory_order_seq_cst, std::memory_order_seq_cst)) {
					// Yay, got the node. This means it was on the list, which means shouldBeOnFreeList must be false no
					// matter the refcount (because nobody else knows it's been taken off yet, it can't have been put back on).
					assert((head->freeListRefs.load(std::memory_order_seq_cst) & SHOULD_BE_ON_FREELIST) == 0);
					
					// Decrease refcount twice, once for our ref, and once for the list's ref
					head->freeListRefs.fetch_sub(2, std::memory_order_seq_cst);
					return head;
				}
				
				// OK, the head must have changed on us, but we still need to decrease the refcount we increased.
				// Note that we don't need to release any memory effects, but we do need to ensure that the reference
				// count decrement happens-after the CAS on the head.
				refs = prevHead->freeListRefs.fetch_sub(1, std::memory_order_seq_cst);
				if (refs == SHOULD_BE_ON_FREELIST + 1) {
					add_knowing_refcount_is_zero(prevHead);
				}
			}
			
			return nullptr;
		}
		
		// Useful for traversing the list when there's no contention (e.g. to destroy remaining nodes)
		N* head_unsafe() const { return freeListHead.load(std::memory_order_seq_cst); }
		
	private:
		inline void add_knowing_refcount_is_zero(N* node)
		{
			// Since the refcount is zero, and nobody can increase it once it's zero (except us, and we run
			// only one copy of this method per node at a time, i.e. the single thread case), then we know
			// we can safely change the next pointer of the node; however, once the refcount is back above
			// zero, then other threads could increase it (happens under heavy contention, when the refcount
			// goes to zero in between a load and a refcount increment of a node in try_get, then back up to
			// something non-zero, then the refcount increment is done by the other thread) -- so, if the CAS
			// to add the node to the actual list fails, decrease the refcount and leave the add operation to
			// the next thread who puts the refcount back at zero (which could be us, hence the loop).
			auto head = freeListHead.load(std::memory_order_seq_cst);
			while (true) {
				node->freeListNext.store(head, std::memory_order_seq_cst);
				node->freeListRefs.store(1, std::memory_order_seq_cst);
				if (!freeListHead.compare_exchange_strong(head, node, std::memory_order_seq_cst, std::memory_order_seq_cst)) {
					// Hmm, the add failed, but we can only try again when the refcount goes back to zero
					if (node->freeListRefs.fetch_add(SHOULD_BE_ON_FREELIST - 1, std::memory_order_seq_cst) == 1) {
						continue;
					}
				}
				return;
			}
		}
		
	private:
		// Implemented like a stack, but where node order doesn't matter (nodes are inserted out of order under contention)
		std::atomic<N*> freeListHead;
	
	static const std::uint32_t REFS_MASK = 0x7FFFFFFF;
	static const std::uint32_t SHOULD_BE_ON_FREELIST = 0x80000000;
	};
	
	
	///////////////////////////
	// Block
	///////////////////////////
	
	struct Block
	{
		Block()
			: next(nullptr), elementsCompletelyDequeued(0), freeListRefs(0), freeListNext(nullptr), dynamicallyAllocated(true)
		{
		}
		
		inline bool is_empty() const
		{
			// Check counter
			if (elementsCompletelyDequeued.load(std::memory_order_seq_cst) == BLOCK_SIZE) {
				std::atomic_thread_fence(std::memory_order_seq_cst);
				return true;
			}
			assert(elementsCompletelyDequeued.load(std::memory_order_seq_cst) <= BLOCK_SIZE);
			return false;
		}
		
		inline bool set_empty()
		{
			// Increment counter
			auto prevVal = elementsCompletelyDequeued.fetch_add(1, std::memory_order_seq_cst);
			assert(prevVal < BLOCK_SIZE);
			return prevVal == BLOCK_SIZE - 1;
		}
		
		inline void reset_empty()
		{
			// Reset counter
			elementsCompletelyDequeued.store(0, std::memory_order_seq_cst);
		}
		
		inline T* operator[](index_t idx) MOODYCAMEL_NOEXCEPT { return static_cast<T*>(static_cast<void*>(elements)) + static_cast<size_t>(idx & static_cast<index_t>(BLOCK_SIZE - 1)); }
		inline T const* operator[](index_t idx) const MOODYCAMEL_NOEXCEPT { return static_cast<T const*>(static_cast<void const*>(elements)) + static_cast<size_t>(idx & static_cast<index_t>(BLOCK_SIZE - 1)); }
		
	private:
		static_assert(std::alignment_of<T>::value <= sizeof(T), "The queue does not support types with an alignment greater than their size at this time");
		MOODYCAMEL_ALIGNED_TYPE_LIKE(char[sizeof(T) * BLOCK_SIZE], T) elements;
	public:
		Block* next;
		std::atomic<size_t> elementsCompletelyDequeued;
		std::atomic<std::uint32_t> freeListRefs;
		std::atomic<Block*> freeListNext;
		bool dynamicallyAllocated;		// Perhaps a better name for this would be 'isNotPartOfInitialBlockPool'
	};
	static_assert(std::alignment_of<Block>::value >= std::alignment_of<T>::value, "Internal error: Blocks must be at least as aligned as the type they are wrapping");
	
	
	///////////////////////////
	// Explicit queue
	///////////////////////////
		
	struct ExplicitProducer
	{
		explicit ExplicitProducer(ConcurrentQueue* parent_) :
			tailIndex(0),
			headIndex(0),
			dequeueOptimisticCount(0),
			dequeueOvercommit(0),
			tailBlock(nullptr),
			parent(parent_),
			blockIndex(nullptr),
			pr_blockIndexSlotsUsed(0),
			pr_blockIndexSize(EXPLICIT_INITIAL_INDEX_SIZE >> 1),
			pr_blockIndexFront(0),
			pr_blockIndexEntries(nullptr),
			pr_blockIndexRaw(nullptr)
		{
			size_t poolBasedIndexSize = details::ceil_to_pow_2(parent_->initialBlockPoolSize) >> 1;
			if (poolBasedIndexSize > pr_blockIndexSize) {
				pr_blockIndexSize = poolBasedIndexSize;
			}
			
			new_block_index(0);		// This creates an index with double the number of current entries, i.e. EXPLICIT_INITIAL_INDEX_SIZE
		}
		
		~ExplicitProducer()
		{
			// Destruct any elements not yet dequeued.
			// Since we're in the destructor, we can assume all elements
			// are either completely dequeued or completely not (no halfways).
			if (this->tailBlock != nullptr) {		// Note this means there must be a block index too
				// First find the block that's partially dequeued, if any
				Block* halfDequeuedBlock = nullptr;
				if ((this->headIndex.load(std::memory_order_seq_cst) & static_cast<index_t>(BLOCK_SIZE - 1)) != 0) {
					// The head's not on a block boundary, meaning a block somewhere is partially dequeued
					// (or the head block is the tail block and was fully dequeued, but the head/tail are still not on a boundary)
					size_t i = (pr_blockIndexFront - pr_blockIndexSlotsUsed) & (pr_blockIndexSize - 1);
					while (details::circular_less_than<index_t>(pr_blockIndexEntries[i].base + BLOCK_SIZE, this->headIndex.load(std::memory_order_seq_cst))) {
						i = (i + 1) & (pr_blockIndexSize - 1);
					}
					assert(details::circular_less_than<index_t>(pr_blockIndexEntries[i].base, this->headIndex.load(std::memory_order_seq_cst)));
					halfDequeuedBlock = pr_blockIndexEntries[i].block;
				}
				
				// Start at the head block (note the first line in the loop gives us the head from the tail on the first iteration)
				auto block = this->tailBlock;
				do {
					block = block->next;
					if (block->ConcurrentQueue::Block::is_empty()) {
						continue;
					}
					
					size_t i = 0;	// Offset into block
					if (block == halfDequeuedBlock) {
						i = static_cast<size_t>(this->headIndex.load(std::memory_order_seq_cst) & static_cast<index_t>(BLOCK_SIZE - 1));
					}
					
					// Walk through all the items in the block; if this is the tail block, we need to stop when we reach the tail index
					auto lastValidIndex = (this->tailIndex.load(std::memory_order_seq_cst) & static_cast<index_t>(BLOCK_SIZE - 1)) == 0 ? BLOCK_SIZE : static_cast<size_t>(this->tailIndex.load(std::memory_order_seq_cst) & static_cast<index_t>(BLOCK_SIZE - 1));
					while (i != BLOCK_SIZE && (block != this->tailBlock || i != lastValidIndex)) {
						(*block)[i++]->~T();
					}
				} while (block != this->tailBlock);
			}
			
			// Destroy all blocks that we own
			if (this->tailBlock != nullptr) {
				auto block = this->tailBlock;
				do {
					auto nextBlock = block->next;
					this->parent->add_block_to_free_list(block);
					block = nextBlock;
				} while (block != this->tailBlock);
			}
			
			// Destroy the block indices
			auto header = static_cast<BlockIndexHeader*>(pr_blockIndexRaw);
			while (header != nullptr) {
				auto prev = static_cast<BlockIndexHeader*>(header->prev);
				header->~BlockIndexHeader();
				free(header);
				header = prev;
			}
		}
		
		template<typename U>
		inline bool enqueue(U&& element)
		{
			index_t currentTailIndex = this->tailIndex.load(std::memory_order_seq_cst);
			index_t newTailIndex = 1 + currentTailIndex;
			if ((currentTailIndex & static_cast<index_t>(BLOCK_SIZE - 1)) == 0) {
				// We reached the end of a block, start a new one
				auto startBlock = this->tailBlock;
				auto originalBlockIndexSlotsUsed = pr_blockIndexSlotsUsed;
				if (this->tailBlock != nullptr && this->tailBlock->next->is_empty()) {
					// We can re-use the block ahead of us, it's empty!					
					this->tailBlock = this->tailBlock->next;
					this->tailBlock->reset_empty();
					
					// We'll put the block on the block index (guaranteed to be room since we're conceptually removing the
					// last block from it first -- except instead of removing then adding, we can just overwrite).
					// Note that there must be a valid block index here, since even if allocation failed in the ctor,
					// it would have been re-attempted when adding the first block to the queue; since there is such
					// a block, a block index must have been successfully allocated.
				}
				else {
					// Whatever head value we see here is >= the last value we saw here (relatively),
					// and <= its current value. Since we have the most recent tail, the head must be
					// <= to it.
					auto head = this->headIndex.load(std::memory_order_seq_cst);
					assert(!details::circular_less_than<index_t>(currentTailIndex, head));
					if (!details::circular_less_than<index_t>(head, currentTailIndex + BLOCK_SIZE)
						|| (MAX_SUBQUEUE_SIZE != details::const_numeric_max<size_t>::value && (MAX_SUBQUEUE_SIZE == 0 || MAX_SUBQUEUE_SIZE - BLOCK_SIZE < currentTailIndex - head))) {
						// We can't enqueue in another block because there's not enough leeway -- the
						// tail could surpass the head by the time the block fills up! (Or we'll exceed
						// the size limit, if the second part of the condition was true.)
						return false;
					}
					// We're going to need a new block; check that the block index has room
					if (pr_blockIndexRaw == nullptr || pr_blockIndexSlotsUsed == pr_blockIndexSize) {
						// Hmm, the circular block index is already full -- we'll need
						// to allocate a new index. Note pr_blockIndexRaw can only be nullptr if
						// the initial allocation failed in the constructor.
						if (!new_block_index(pr_blockIndexSlotsUsed)) {
							return false;
						}
					}
					
					// Insert a new block in the circular linked list
					auto newBlock = this->parent->requisition_block();
					if (newBlock == nullptr)
						return false;
					newBlock->reset_empty();
					if (this->tailBlock == nullptr) {
						newBlock->next = newBlock;
					}
					else {
						newBlock->next = this->tailBlock->next;
						this->tailBlock->next = newBlock;
					}
					this->tailBlock = newBlock;
					++pr_blockIndexSlotsUsed;
				}

				MOODYCAMEL_CONSTEXPR_IF (!MOODYCAMEL_NOEXCEPT_CTOR(T, U, new (static_cast<T*>(nullptr)) T(std::forward<U>(element)))) {
					// The constructor may throw. We want the element not to appear in the queue in
					// that case (without corrupting the queue):
					MOODYCAMEL_TRY {
						new ((*this->tailBlock)[currentTailIndex]) T(std::forward<U>(element));
					}
					MOODYCAMEL_CATCH (...) {
						// Revert change to the current block, but leave the new block available
						// for next time
						pr_blockIndexSlotsUsed = originalBlockIndexSlotsUsed;
						this->tailBlock = startBlock == nullptr ? this->tailBlock : startBlock;
						MOODYCAMEL_RETHROW;
					}
				}
				else {
					(void)startBlock;
					(void)originalBlockIndexSlotsUsed;
				}
				
				// Add block to block index
				auto& entry = blockIndex.load(std::memory_order_seq_cst)->entries[pr_blockIndexFront];
				entry.base = currentTailIndex;
				entry.block = this->tailBlock;
				blockIndex.load(std::memory_order_seq_cst)->front.store(pr_blockIndexFront, std::memory_order_seq_cst);
				pr_blockIndexFront = (pr_blockIndexFront + 1) & (pr_blockIndexSize - 1);
				
				MOODYCAMEL_CONSTEXPR_IF (!MOODYCAMEL_NOEXCEPT_CTOR(T, U, new (static_cast<T*>(nullptr)) T(std::forward<U>(element)))) {
					this->tailIndex.store(newTailIndex, std::memory_order_seq_cst);
					return true;
				}
			}
			
			// Enqueue
			new ((*this->tailBlock)[currentTailIndex]) T(std::forward<U>(element));
			
			this->tailIndex.store(newTailIndex, std::memory_order_seq_cst);
			return true;
		}
		
		template<typename U>
		bool dequeue(U& element)
		{
			auto tail = this->tailIndex.load(std::memory_order_seq_cst);
			auto overcommit = this->dequeueOvercommit.load(std::memory_order_seq_cst);
			if (!details::circular_less_than<index_t>(this->dequeueOptimisticCount.load(std::memory_order_seq_cst) - overcommit, tail))
				return false;
			// Might be something to dequeue, let's give it a try
			
			// Note that this if is purely for performance purposes in the common case when the queue is
			// empty and the values are eventually consistent -- we may enter here spuriously.
			
			// Note that whatever the values of overcommit and tail are, they are not going to change (unless we
			// change them) and must be the same value at this point (inside the if) as when the if condition was
			// evaluated.

			// We insert an acquire fence here to synchronize-with the release upon incrementing dequeueOvercommit below.
			// This ensures that whatever the value we got loaded into overcommit, the load of dequeueOptisticCount in
			// the fetch_add below will result in a value at least as recent as that (and therefore at least as large).
			// Note that I believe a compiler (signal) fence here would be sufficient due to the nature of fetch_add (all
			// read-modify-write operations are guaranteed to work on the latest value in the modification order), but
			// unfortunately that can't be shown to be correct using only the C++11 standard.
			// See http://stackoverflow.com/questions/18223161/what-are-the-c11-memory-ordering-guarantees-in-this-corner-case
			std::atomic_thread_fence(std::memory_order_seq_cst);
			
			// Increment optimistic counter, then check if it went over the boundary
			auto myDequeueCount = this->dequeueOptimisticCount.fetch_add(1, std::memory_order_seq_cst);
			
			// Note that since dequeueOvercommit must be <= dequeueOptimisticCount (because dequeueOvercommit is only ever
			// incremented after dequeueOptimisticCount -- this is enforced in the `else` block below), and since we now
			// have a version of dequeueOptimisticCount that is at least as recent as overcommit (due to the release upon
			// incrementing dequeueOvercommit and the acquire above that synchronizes with it), overcommit <= myDequeueCount.
			// However, we can't assert this since both dequeueOptimisticCount and dequeueOvercommit may (independently)
			// overflow; in such a case, though, the logic still holds since the difference between the two is maintained.
			
			// Note that we reload tail here in case it changed; it will be the same value as before or greater, since
			// this load is sequenced after (happens after) the earlier load above. This is supported by read-read
			// coherency (as defined in the standard), explained here: http://en.cppreference.com/w/cpp/atomic/memory_order
			tail = this->tailIndex.load(std::memory_order_seq_cst);
			if (details::circular_less_than<index_t>(myDequeueCount - overcommit, tail)) {
				// Guaranteed to be at least one element to dequeue!
				
				// Get the index. Note that since there's guaranteed to be at least one element, this
				// will never exceed tail. We need to do an acquire-release fence here since it's possible
				// that whatever condition got us to this point was for an earlier enqueued element (that
				// we already see the memory effects for), but that by the time we increment somebody else
				// has incremented it, and we need to see the memory effects for *that* element, which is
				// in such a case is necessarily visible on the thread that incremented it in the first
				// place with the more current condition (they must have acquired a tail that is at least
				// as recent).
				auto index = this->headIndex.fetch_add(1, std::memory_order_seq_cst);
				
				
				// Determine which block the element is in
				
				auto localBlockIndex = blockIndex.load(std::memory_order_seq_cst);
				auto localBlockIndexHead = localBlockIndex->front.load(std::memory_order_seq_cst);
				
				// We need to be careful here about subtracting and dividing because of index wrap-around.
				// When an index wraps, we need to preserve the sign of the offset when dividing it by the
				// block size (in order to get a correct signed block count offset in all cases):
				auto headBase = localBlockIndex->entries[localBlockIndexHead].base;
				auto blockBaseIndex = index & ~static_cast<index_t>(BLOCK_SIZE - 1);
				auto offset = static_cast<size_t>(static_cast<typename std::make_signed<index_t>::type>(blockBaseIndex - headBase) / static_cast<typename std::make_signed<index_t>::type>(BLOCK_SIZE));
				auto block = localBlockIndex->entries[(localBlockIndexHead + offset) & (localBlockIndex->size - 1)].block;
				
				// Dequeue
				auto& el = *((*block)[index]);
				if (!MOODYCAMEL_NOEXCEPT_ASSIGN(T, T&&, element = std::move(el))) {
					// Make sure the element is still fully dequeued and destroyed even if the assignment
					// throws
					struct Guard {
						Block* block;
						index_t index;
						
						~Guard()
						{
							(*block)[index]->~T();
							block->set_empty();
						}
					} guard = { block, index };

					element = std::move(el); // NOLINT
				}
				else {
					element = std::move(el); // NOLINT
					el.~T(); // NOLINT
					block->set_empty();
				}
				
				return true;
			}
			else {
				// Wasn't anything to dequeue after all; make the effective dequeue count eventually consistent
				this->dequeueOvercommit.fetch_add(1, std::memory_order_seq_cst);		// Release so that the fetch_add on dequeueOptimisticCount is guaranteed to happen before this write
			}
			return false;
		}
		
	private:
		struct BlockIndexEntry
		{
			index_t base;
			Block* block;
		};
		
		struct BlockIndexHeader
		{
			size_t size;
			std::atomic<size_t> front;		// Current slot (not next, like pr_blockIndexFront)
			BlockIndexEntry* entries;
			void* prev;
		};
		
		
		bool new_block_index(size_t numberOfFilledSlotsToExpose)
		{
			auto prevBlockSizeMask = pr_blockIndexSize - 1;
			
			// Create the new block
			pr_blockIndexSize <<= 1;
			auto newRawPtr = (char*)malloc(sizeof(BlockIndexHeader) + std::alignment_of<BlockIndexEntry>::value - 1 + sizeof(BlockIndexEntry) * pr_blockIndexSize);
			if (newRawPtr == nullptr) {
				pr_blockIndexSize >>= 1;		// Reset to allow graceful retry
				return false;
			}
			
			auto newBlockIndexEntries = reinterpret_cast<BlockIndexEntry*>(details::align_for<BlockIndexEntry>(newRawPtr + sizeof(BlockIndexHeader)));
			
			// Copy in all the old indices, if any
			size_t j = 0;
			if (pr_blockIndexSlotsUsed != 0) {
				auto i = (pr_blockIndexFront - pr_blockIndexSlotsUsed) & prevBlockSizeMask;
				do {
					newBlockIndexEntries[j++] = pr_blockIndexEntries[i];
					i = (i + 1) & prevBlockSizeMask;
				} while (i != pr_blockIndexFront);
			}
			
			// Update everything
			auto header = new (newRawPtr) BlockIndexHeader;
			header->size = pr_blockIndexSize;
			header->front.store(numberOfFilledSlotsToExpose - 1, std::memory_order_seq_cst);
			header->entries = newBlockIndexEntries;
			header->prev = pr_blockIndexRaw;		// we link the new block to the old one so we can free it later
			
			pr_blockIndexFront = j;
			pr_blockIndexEntries = newBlockIndexEntries;
			pr_blockIndexRaw = newRawPtr;
			blockIndex.store(header, std::memory_order_seq_cst);
			
			return true;
		}
		
	private:
		std::atomic<index_t> tailIndex;		// Where to enqueue to next
		std::atomic<index_t> headIndex;		// Where to dequeue from next
		
		std::atomic<index_t> dequeueOptimisticCount;
		std::atomic<index_t> dequeueOvercommit;
		
		Block* tailBlock;
		ConcurrentQueue* parent;
		std::atomic<BlockIndexHeader*> blockIndex;
		
		// To be used by producer only -- consumer must use the ones in referenced by blockIndex
		size_t pr_blockIndexSlotsUsed;
		size_t pr_blockIndexSize;
		size_t pr_blockIndexFront;		// Next slot (not current)
		BlockIndexEntry* pr_blockIndexEntries;
		void* pr_blockIndexRaw;
	};
	
	
	//////////////////////////////////
	// Block pool manipulation
	//////////////////////////////////
	
	void populate_initial_block_list(size_t blockCount)
	{
		initialBlockPoolSize = blockCount;
		if (initialBlockPoolSize == 0) {
			initialBlockPool = nullptr;
			return;
		}
		
		initialBlockPool = create_array<Block>(blockCount);
		if (initialBlockPool == nullptr) {
			initialBlockPoolSize = 0;
		}
		for (size_t i = 0; i < initialBlockPoolSize; ++i) {
			initialBlockPool[i].dynamicallyAllocated = false;
		}
	}
	
	inline Block* try_get_block_from_initial_pool()
	{
		if (initialBlockPoolIndex.load(std::memory_order_seq_cst) >= initialBlockPoolSize) {
			return nullptr;
		}
		
		auto index = initialBlockPoolIndex.fetch_add(1, std::memory_order_seq_cst);
		
		return index < initialBlockPoolSize ? (initialBlockPool + index) : nullptr;
	}
	
	inline void add_block_to_free_list(Block* block)
	{
		if (!Traits::RECYCLE_ALLOCATED_BLOCKS && block->dynamicallyAllocated) {
			destroy(block);
		}
		else {
			freeList.add(block);
		}
	}
	
	inline void add_blocks_to_free_list(Block* block)
	{
		while (block != nullptr) {
			auto next = block->next;
			add_block_to_free_list(block);
			block = next;
		}
	}
	
	inline Block* try_get_block_from_free_list()
	{
		return freeList.try_get();
	}
	
	// Gets a free block from one of the memory pools, or allocates a new one (if applicable)
	Block* requisition_block()
	{
		auto block = try_get_block_from_initial_pool();
		if (block != nullptr) {
			return block;
		}
		
		block = try_get_block_from_free_list();
		if (block != nullptr) {
			return block;
		}
		return create<Block>();
	}
	//////////////////////////////////
	// Utility functions
	//////////////////////////////////

	template<typename TAlign>
	static inline void* aligned_malloc(size_t size)
	{
		MOODYCAMEL_CONSTEXPR_IF (std::alignment_of<TAlign>::value <= std::alignment_of<details::max_align_t>::value)
			return malloc(size);
		else {
			size_t alignment = std::alignment_of<TAlign>::value;
			void* raw = malloc(size + alignment - 1 + sizeof(void*));
			if (!raw)
				return nullptr;
			char* ptr = details::align_for<TAlign>(reinterpret_cast<char*>(raw) + sizeof(void*));
			*(reinterpret_cast<void**>(ptr) - 1) = raw;
			return ptr;
		}
	}

	template<typename TAlign>
	static inline void aligned_free(void* ptr)
	{
		MOODYCAMEL_CONSTEXPR_IF (std::alignment_of<TAlign>::value <= std::alignment_of<details::max_align_t>::value)
			return free(ptr);
		else
			free(ptr ? *(reinterpret_cast<void**>(ptr) - 1) : nullptr);
	}

	template<typename U>
	static inline U* create_array(size_t count)
	{
		assert(count > 0);
		U* p = static_cast<U*>(aligned_malloc<U>(sizeof(U) * count));
		if (p == nullptr)
			return nullptr;

		for (size_t i = 0; i != count; ++i)
			new (p + i) U();
		return p;
	}

	template<typename U>
	static inline void destroy_array(U* p, size_t count)
	{
		if (p != nullptr) {
			assert(count > 0);
			for (size_t i = count; i != 0; )
				(p + --i)->~U();
		}
		aligned_free<U>(p);
	}

	template<typename U>
	static inline U* create()
	{
		void* p = aligned_malloc<U>(sizeof(U));
		return p != nullptr ? new (p) U : nullptr;
	}

	template<typename U, typename A1>
	static inline U* create(A1&& a1)
	{
		void* p = aligned_malloc<U>(sizeof(U));
		return p != nullptr ? new (p) U(std::forward<A1>(a1)) : nullptr;
	}

	template<typename U>
	static inline void destroy(U* p)
	{
		if (p != nullptr)
			p->~U();
		aligned_free<U>(p);
	}

private:
	ExplicitProducer* producer;
	
	std::atomic<size_t> initialBlockPoolIndex;
	Block* initialBlockPool;
	size_t initialBlockPoolSize;
	FreeList<Block> freeList;
};

}

#if defined(__GNUC__) && !defined(__INTEL_COMPILER)
#pragma GCC diagnostic pop
#endif