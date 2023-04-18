#pragma once

#include "mg/common/Definitions.h"

#if IS_PLATFORM_POSIX
#include "Atomic_Posix.h"
#else
#include "Atomic_Win.h"
#endif

#include <atomic>

namespace mg {
namespace common {

	template<typename T>
	class Atomic
	{
	public:
		Atomic() = default;
		Atomic(const T& aValue) : myValue(aValue) {}

		T LoadRelaxed() const
		{ return myValue.load(std::memory_order_relaxed); }
		T LoadAcquire() const
		{ return myValue.load(std::memory_order_acquire); }
		T Load() const
		{ return myValue.load(std::memory_order_seq_cst); }

		void StoreRelaxed(
			const T& aValue)
		{ return myValue.store(aValue, std::memory_order_relaxed); }
		void StoreRelease(
			const T& aValue)
		{ return myValue.store(aValue, std::memory_order_release); }
		void Store(
			const T& aValue)
		{ return myValue.store(aValue, std::memory_order_seq_cst); }

		T ExchangeRelaxed(
			const T& aValue)
		{ return myValue.exchange(aValue, std::memory_order_relaxed); }
		T ExchangeAcqRel(
			const T& aValue)
		{ return myValue.exchange(aValue, std::memory_order_acq_rel); }
		T ExchangeRelease(
			const T& aValue)
		{ return myValue.exchange(aValue, std::memory_order_release); }
		T Exchange(
			const T& aValue)
		{ return myValue.exchange(aValue, std::memory_order_seq_cst); }

		T FetchAddRelaxed(
			const T& aValue)
		{ return myValue.fetch_add(aValue, std::memory_order_relaxed); }
		T FetchAddAcqRel(
			const T& aValue)
		{ return myValue.fetch_add(aValue, std::memory_order_acq_rel); }
		T FetchAddRelease(
			const T& aValue)
		{ return myValue.fetch_add(aValue, std::memory_order_release); }
		T FetchAdd(
			const T& aValue)
		{ return myValue.fetch_add(aValue, std::memory_order_seq_cst); }

		T AddFetchRelaxed(
			const T& aValue)
		{ return myValue.fetch_add(aValue, std::memory_order_relaxed) + aValue; }
		T AddFetchAcqRel(
			const T& aValue)
		{ return myValue.fetch_add(aValue, std::memory_order_acq_rel) + aValue; }
		T AddFetchRelease(
			const T& aValue)
		{ return myValue.fetch_add(aValue, std::memory_order_release) + aValue; }
		T AddFetch(
			const T& aValue)
		{ return myValue.fetch_add(aValue, std::memory_order_seq_cst) + aValue; }

		void AddRelaxed(
			const T& aValue)
		{ myValue.fetch_add(aValue, std::memory_order_relaxed) + aValue; }
		void AddAcqRel(
			const T& aValue)
		{ myValue.fetch_add(aValue, std::memory_order_acq_rel) + aValue; }
		void AddRelease(
			const T& aValue)
		{ myValue.fetch_add(aValue, std::memory_order_release) + aValue; }
		void Add(
			const T& aValue)
		{ myValue.fetch_add(aValue, std::memory_order_seq_cst) + aValue; }

		T FetchSubRelaxed(
			const T& aValue)
		{ return myValue.fetch_sub(aValue, std::memory_order_relaxed); }
		T FetchSubAcqRel(
			const T& aValue)
		{ return myValue.fetch_sub(aValue, std::memory_order_acq_rel); }
		T FetchSubRelease(
			const T& aValue)
		{ return myValue.fetch_sub(aValue, std::memory_order_release); }
		T FetchSub(
			const T& aValue)
		{ return myValue.fetch_sub(aValue, std::memory_order_seq_cst); }

		T SubFetchRelaxed(
			const T& aValue)
		{ return myValue.fetch_sub(aValue, std::memory_order_relaxed) - aValue; }
		T SubFetchAcqRel(
			const T& aValue)
		{ return myValue.fetch_sub(aValue, std::memory_order_acq_rel) - aValue; }
		T SubFetchRelease(
			const T& aValue)
		{ return myValue.fetch_sub(aValue, std::memory_order_release) - aValue; }
		T SubFetch(
			const T& aValue)
		{ return myValue.fetch_sub(aValue, std::memory_order_seq_cst) - aValue; }

		void SubRelaxed(
			const T& aValue)
		{ myValue.fetch_sub(aValue, std::memory_order_relaxed); }
		void SubAcqRel(
			const T& aValue)
		{ myValue.fetch_sub(aValue, std::memory_order_acq_rel); }
		void SubRelease(
			const T& aValue)
		{ myValue.fetch_sub(aValue, std::memory_order_release); }
		void Sub(
			const T& aValue)
		{ myValue.fetch_sub(aValue, std::memory_order_seq_cst); }

		void IncrementRelaxed()
		{ myValue.fetch_add(1, std::memory_order_relaxed); }
		void IncrementAcqRel()
		{ myValue.fetch_add(1, std::memory_order_acq_rel); }
		void IncrementRelease()
		{ myValue.fetch_add(1, std::memory_order_release); }
		void Increment()
		{ myValue.fetch_add(1, std::memory_order_seq_cst); }

		T FetchIncrementRelaxed()
		{ return myValue.fetch_add(1, std::memory_order_relaxed); }
		T FetchIncrementAcqRel()
		{ return myValue.fetch_add(1, std::memory_order_acq_rel); }
		T FetchIncrementRelease()
		{ return myValue.fetch_add(1, std::memory_order_release); }
		T FetchIncrement()
		{ return myValue.fetch_add(1, std::memory_order_seq_cst); }

		T IncrementFetchRelaxed()
		{ return myValue.fetch_add(1, std::memory_order_relaxed) + 1; }
		T IncrementFetchAcqRel()
		{ return myValue.fetch_add(1, std::memory_order_acq_rel) + 1; }
		T IncrementFetchRelease()
		{ return myValue.fetch_add(1, std::memory_order_release) + 1; }
		T IncrementFetch()
		{ return myValue.fetch_add(1, std::memory_order_seq_cst) + 1; }

		void DecrementRelaxed()
		{ myValue.fetch_sub(1, std::memory_order_relaxed); }
		void DecrementAcqRel()
		{ myValue.fetch_sub(1, std::memory_order_acq_rel); }
		void DecrementRelease()
		{ myValue.fetch_sub(1, std::memory_order_release); }
		void Decrement()
		{ myValue.fetch_sub(1, std::memory_order_seq_cst); }

		T FetchDecrementRelaxed()
		{ return myValue.fetch_sub(1, std::memory_order_relaxed); }
		T FetchDecrementAcqRel()
		{ return myValue.fetch_sub(1, std::memory_order_acq_rel); }
		T FetchDecrementRelease()
		{ return myValue.fetch_sub(1, std::memory_order_release); }
		T FetchDecrement()
		{ return myValue.fetch_sub(1, std::memory_order_seq_cst); }

		T DecrementFetchRelaxed()
		{ return myValue.fetch_sub(1, std::memory_order_relaxed) - 1; }
		T DecrementFetchAcqRel()
		{ return myValue.fetch_sub(1, std::memory_order_acq_rel) - 1; }
		T DecrementFetchRelease()
		{ return myValue.fetch_sub(1, std::memory_order_release) - 1; }
		T DecrementFetch()
		{ return myValue.fetch_sub(1, std::memory_order_seq_cst) - 1; }

		bool CmpExchgWeakRelaxed(
			T& aExpected,
			const T& aValue);
		bool CmpExchgWeakAcquire(
			T& aExpected,
			const T& aValue);
		bool CmpExchgWeakAcqRel(
			T& aExpected,
			const T& aValue);
		bool CmpExchgWeakRelease(
			T& aExpected,
			const T& aValue);
		bool CmpExchgWeak(
			T& aExpected,
			const T& aValue);

		bool CmpExchgStrongRelaxed(
			T& aExpected,
			const T& aValue);
		bool CmpExchgStrongAcquire(
			T& aExpected,
			const T& aValue);
		bool CmpExchgStrongAcqRel(
			T& aExpected,
			const T& aValue);
		bool CmpExchgStrongRelease(
			T& aExpected,
			const T& aValue);
		bool CmpExchgStrong(
			T& aExpected,
			const T& aValue);

	private:
		std::atomic<T> myValue;
	};

	using AtomicU8 = Atomic<uint8_t>;
	using AtomicU16 = Atomic<uint16_t>;
	using AtomicU32 = Atomic<uint32_t>;
	using AtomicU64 = Atomic<uint64_t>;
	using AtomicI8 = Atomic<uint8_t>;
	using AtomicI16 = Atomic<int16_t>;
	using AtomicI32 = Atomic<int32_t>;
	using AtomicI64 = Atomic<int64_t>;
	using AtomicBool = Atomic<bool>;
	using AtomicVoidPtr = Atomic<void*>;

	////////////////////////////////////////////////////////////////////////

	template<typename T>
	inline bool
	Atomic<T>::CmpExchgWeakRelaxed(
		T& aExpected,
		const T& aValue)
	{
		return myValue.compare_exchange_weak(aExpected, aValue,
			std::memory_order_relaxed,
			std::memory_order_relaxed);
	}

	template<typename T>
	inline bool
	Atomic<T>::CmpExchgWeakAcquire(
		T& aExpected,
		const T& aValue)
	{
		return myValue.compare_exchange_weak(aExpected, aValue,
			std::memory_order_acquire,
			std::memory_order_relaxed);
	}

	template<typename T>
	inline bool
	Atomic<T>::CmpExchgWeakAcqRel(
		T& aExpected,
		const T& aValue)
	{
		return myValue.compare_exchange_weak(aExpected, aValue,
			std::memory_order_acq_rel,
			std::memory_order_relaxed);
	}

	template<typename T>
	inline bool
	Atomic<T>::CmpExchgWeakRelease(
		T& aExpected,
		const T& aValue)
	{
		return myValue.compare_exchange_weak(aExpected, aValue,
			std::memory_order_release,
			std::memory_order_relaxed);
	}

	template<typename T>
	inline bool
	Atomic<T>::CmpExchgWeak(
		T& aExpected,
		const T& aValue)
	{
		return myValue.compare_exchange_weak(aExpected, aValue,
			std::memory_order_seq_cst,
			std::memory_order_relaxed);
	}

	template<typename T>
	inline bool
	Atomic<T>::CmpExchgStrongRelaxed(
		T& aExpected,
		const T& aValue)
	{
		return myValue.compare_exchange_strong(aExpected,
			aValue, std::memory_order_relaxed,
			std::memory_order_relaxed);
	}

	template<typename T>
	inline bool
	Atomic<T>::CmpExchgStrongAcquire(
		T& aExpected,
		const T& aValue)
	{
		return myValue.compare_exchange_strong(aExpected,
			aValue, std::memory_order_acquire,
			std::memory_order_relaxed);
	}

	template<typename T>
	inline bool
	Atomic<T>::CmpExchgStrongAcqRel(
		T& aExpected,
		const T& aValue)
	{
		return myValue.compare_exchange_strong(aExpected,
			aValue, std::memory_order_acq_rel,
			std::memory_order_relaxed);
	}

	template<typename T>
	inline bool
	Atomic<T>::CmpExchgStrongRelease(
		T& aExpected,
		const T& aValue)
	{
		return myValue.compare_exchange_strong(aExpected,
			aValue, std::memory_order_release,
			std::memory_order_relaxed);
	}

	template<typename T>
	inline bool
	Atomic<T>::CmpExchgStrong(
		T& aExpected,
		const T& aValue)
	{
		return myValue.compare_exchange_strong(aExpected,
			aValue, std::memory_order_seq_cst,
			std::memory_order_relaxed);
	}

}
}
