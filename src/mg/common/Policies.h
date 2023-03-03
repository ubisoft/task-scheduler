#pragma once

#include "mg/common/Assert.h"
#include "mg/common/Types.h"
#include "mg/common/TypeTraits.h"

namespace mg {
namespace common {

	// Methods for objects relocation, taking into account their
	// constructors and assignment operators. It is possible to
	// just always use the cycles and operators, but this set of
	// methods is able to use memcpy() and memmove() when
	// possible - this is faster. Memcpy()/memmove() are used for
	// trivial types.
	//
	// Strictly speaking, they could be used even on non-trivial
	// types - for example, a type can have a non-trivial
	// constructor, but still be ok for memcpy()/memmove(). But in
	// clang compiler it is impossible to detect in the same way,
	// if a type has trivial move constructor or not. The same for
	// move assignment. In clang __has_trivial_copy() and
	// __has_trivial_assign() don't take into account the move
	// copy and assign.
	//
	template<typename T, bool IsTrivial = mg::common::IsTrivial<T>::myValue>
	struct ItemsRelocation;

	template<typename T>
	struct ItemsRelocation<T, true>
	{
		static inline void
		Copy(
			T* aDst,
			const T* aSrc,
			uint32_t aCount)
		{
			MG_DEV_ASSERT(aDst >= aSrc + aCount || aSrc >= aDst + aCount);
			// Locations don't intersect - can use memcpy().
			memcpy(aDst, aSrc, aCount * sizeof(T));
		}

		static inline void
		Copy(
			T* aDst,
			const T* aSrc)
		{
			MG_DEV_ASSERT(aDst != aSrc);
			// Locations don't intersect - can use memcpy().
			*aDst = *aSrc;
		}

		static inline void
		MoveForward(
			T* aDst,
			T* aSrc,
			uint32_t aCount)
		{
			MG_DEV_ASSERT(aDst > aSrc);
			memmove(aDst, aSrc, aCount * sizeof(T));
		}

		static inline void
		MoveBack(
			T* aDst,
			T* aSrc,
			uint32_t aCount)
		{
			memmove(aDst, aSrc, aCount * sizeof(T));
		}

		static inline void
		Move(
			T* aDst,
			T* aSrc,
			uint32_t aCount)
		{
			Copy(aDst, aSrc, aCount);
		}

		static inline void
		Move(
			T* aDst,
			T* aSrc)
		{
			Copy(aDst, aSrc);
		}

		static inline void
		MoveConstruct(
			T* aDst,
			T* aSrc,
			uint32_t aCount)
		{
			Copy(aDst, aSrc, aCount);
		}

		static inline void
		MoveConstruct(
			T* aDst,
			T* aSrc)
		{
			Copy(aDst, aSrc);
		}

		static inline void
		CopyConstruct(
			T* aDst,
			const T* aSrc,
			uint32_t aCount)
		{
			Copy(aDst, aSrc, aCount);
		}

		static inline void
		CopyConstruct(
			T* aDst,
			const T* aSrc)
		{
			Copy(aDst, aSrc);
		}
	};

	template<typename T>
	struct ItemsRelocation<T, false>
	{
		static inline void
		Copy(
			T* aDst,
			const T* aSrc,
			uint32_t aCount)
		{
			for (T* dstEnd = aDst + aCount; aDst < dstEnd; ++aDst, ++aSrc)
				*aDst = *aSrc;
		}

		static inline void
		MoveForward(
			T* aDst,
			T* aSrc,
			uint32_t aCount)
		{
			// MoveForward and MoveBack exist separately, because
			// in all cases it is always known to which direction
			// the move is needed.
			if (aCount == 0)
				return;
			MG_DEV_ASSERT(aDst > aSrc);
			T* dstEnd = aDst;
			aDst += aCount - 1;
			aSrc += aCount - 1;
			do
			{
				*aDst = mg::common::Move(*aSrc);
				--aDst;
				--aSrc;
			} while (aDst >= dstEnd);
		}

		static inline void
		MoveBack(
			T* aDst,
			T* aSrc,
			uint32_t aCount)
		{
			// MoveForward and MoveBack exist separately, because
			// in all cases it is always known to which direction
			// the move is needed.
			MG_DEV_ASSERT(aDst < aSrc);
			T* dstEnd = aDst + aCount;
			for (; aDst < dstEnd; ++aDst, ++aSrc)
				*aDst = mg::common::Move(*aSrc);
		}

		static inline void
		Move(
			T* aDst,
			T* aSrc,
			uint32_t aCount)
		{
			MG_DEV_ASSERT(aDst >= aSrc + aCount || aSrc >= aDst + aCount);
			T* dstEnd = aDst + aCount;
			for (; aDst < dstEnd; ++aDst, ++aSrc)
				*aDst = mg::common::Move(*aSrc);
		}

		static inline void
		Move(
			T* aDst,
			T* aSrc)
		{
			MG_DEV_ASSERT(aDst != aSrc);
			*aDst = mg::common::Move(*aSrc);
		}

		static inline void
		MoveConstruct(
			T* aDst,
			T* aSrc,
			uint32_t aCount)
		{
			MG_DEV_ASSERT(aDst >= aSrc + aCount || aSrc >= aDst + aCount);
			T* dstEnd = aDst + aCount;
			for (; aDst < dstEnd; ++aDst, ++aSrc)
				new (aDst) T(mg::common::Move(*aSrc));
		}

		static inline void
		MoveConstruct(
			T* aDst,
			T* aSrc)
		{
			MG_DEV_ASSERT(aDst != aSrc);
			new (aDst) T(mg::common::Move(*aSrc));
		}

		static inline void
		CopyConstruct(
			T* aDst,
			const T* aSrc,
			uint32_t aCount)
		{
			MG_DEV_ASSERT(aDst >= aSrc + aCount || aSrc >= aDst + aCount);
			T* dstEnd = aDst + aCount;
			for (; aDst < dstEnd; ++aDst, ++aSrc)
				new (aDst) T(*aSrc);
		}

		static inline void
		CopyConstruct(
			T* aDst,
			const T* aSrc)
		{
			MG_DEV_ASSERT(aDst != aSrc);
			new (aDst) T(*aSrc);
		}
	};

	template<typename T>
	inline void
	ItemsCopy(
		T* aDst,
		const T* aSrc,
		uint32_t aCount)
	{
		ItemsRelocation<T>::Copy(aDst, aSrc, aCount);
	}

	template<typename T>
	inline void
	ItemsCopy(
		T* aDst,
		const T* aSrc)
	{
		*aDst = *aSrc;
	}

	template<typename T>
	inline void
	ItemsMoveBack(
		T* aDst,
		T* aSrc,
		uint32_t aCount)
	{
		ItemsRelocation<T>::MoveBack(aDst, aSrc, aCount);
	}

	template<typename T>
	inline void
	ItemsMoveForward(
		T* aDst,
		T* aSrc,
		uint32_t aCount)
	{
		ItemsRelocation<T>::MoveForward(aDst, aSrc, aCount);
	}

	template<typename T>
	inline void
	ItemsMove(
		T* aDst,
		T* aSrc,
		uint32_t aCount)
	{
		ItemsRelocation<T>::Move(aDst, aSrc, aCount);
	}

	template<typename T>
	inline void
	ItemsMove(
		T* aDst,
		T* aSrc)
	{
		ItemsRelocation<T>::Move(aDst, aSrc);
	}

	template<typename T>
	inline void
	ItemsMoveConstruct(
		T* aDst,
		T* aSrc,
		uint32_t aCount)
	{
		ItemsRelocation<T>::MoveConstruct(aDst, aSrc, aCount);
	}

	template<typename T>
	inline void
	ItemsMoveConstruct(
		T* aDst,
		T* aSrc)
	{
		ItemsRelocation<T>::MoveConstruct(aDst, aSrc);
	}

	template<typename T>
	inline void
	ItemsCopyConstruct(
		T* aDst,
		const T* aSrc,
		uint32_t aCount)
	{
		ItemsRelocation<T>::CopyConstruct(aDst, aSrc, aCount);
	}

	template<typename T>
	inline void
	ItemsCopyConstruct(
		T* aDst,
		const T* aSrc)
	{
		ItemsRelocation<T>::CopyConstruct(aDst, aSrc);
	}

	// Helper to destroy an array of objects with no-op
	// optimization when the element type does not have a
	// destructor.
	//
	template<typename T, bool HasTrivialDestructor =
		mg::common::HasTrivialDestructor<T>::myValue>
	struct ItemsDestruction;

	template<typename T>
	struct ItemsDestruction<T, true>
	{
		static inline void
		Do(
			T*,
			uint32_t)
		{
		}

		static inline void
		Do(
			T*)
		{
		}
	};

	template<typename T>
	struct ItemsDestruction<T, false>
	{
		static inline void
		Do(
			T* aData,
			uint32_t aCount)
		{
			for (T* end = aData + aCount; aData < end; ++aData)
				aData->~T();
		}

		static inline void
		Do(
			T* aData)
		{
			aData->~T();
		}
	};

	template<typename T>
	inline void
	ItemsDestroy(
		T* aData,
		uint32_t aCount)
	{
		ItemsDestruction<T>::Do(aData, aCount);
	}

	template<typename T>
	inline void
	ItemsDestroy(
		T* aData)
	{
		ItemsDestruction<T>::Do(aData);
	}

	// Helper to delete an array of objects or raise a friendly
	// error if the type is not a pointer.
	//
	template<typename T>
	inline void
	ItemsDelete(
		T* aData,
		uint32_t aCount)
	{
		// The assertion and 'delete' won't fail the compilation
		// for non-pointer types as long as the function is
		// never actually called for them. Even if they are used
		// in a class which offers deletion of its members via
		// this function and can store non-pointers.
		// The trick is to involve the template parameter into
		// the expressions - then via SFINAE the compiler will try
		// not to compile this function at all while possible.
		static_assert(mg::common::IsPointer<T>::myValue,
			"Can not delete an array of non-pointers");
		for (T* end = aData + aCount; aData < end; ++aData)
			delete *aData;
	}

	template<typename T>
	inline void
	ItemsDelete(
		T* aData)
	{
		static_assert(mg::common::IsPointer<T>::myValue,
			"Can not delete a non-pointer");
		delete *aData;
	}

	// Helper to construct an array of objects with no-op
	// optimization when the element type does not have a
	// constructor.
	//
	template<typename T, bool HasTrivialConstructor =
		mg::common::HasTrivialConstructor<T>::myValue>
	struct ItemsConstruction;

	template<typename T>
	struct ItemsConstruction<T, true>
	{
		static inline void
		Do(
			T*,
			uint32_t)
		{
		}

		static inline void
		Do(
			T*)
		{
		}
	};

	template<typename T>
	struct ItemsConstruction<T, false>
	{
		static inline void
		Do(
			T* aData,
			uint32_t aCount)
		{
			for (T* end = aData + aCount; aData < end; ++aData)
				new (aData) T();
		}

		static inline void
		Do(
			T* aData)
		{
			new (aData) T();
		}
	};

	template<typename T>
	inline void
	ItemsConstruct(
		T* aData,
		uint32_t aCount)
	{
		ItemsConstruction<T>::Do(aData, aCount);
	}

	template<typename T>
	inline void
	ItemsConstruct(
		T* aData)
	{
		ItemsConstruction<T>::Do(aData);
	}

	// Helper to compare elements only when they are comparable.
	// Otherwise it will crash, to avoid surprises.
	//
	template<typename T, bool AreComparable = mg::common::AreComparable<T, T>::myValue>
	struct ItemsComparisonEq;

	template<typename T>
	struct ItemsComparisonEq<T, true>
	{
		static inline bool
		Do(
			const T& aObj1,
			const T& aObj2)
		{
			return aObj1 == aObj2;
		}
	};

	template<typename T>
	struct ItemsComparisonEq<T, false>
	{
		static inline bool
		Do(
			const T&,
			const T&)
		{
			MG_COMMON_ASSERT(false);
			return false;
		}
	};

	template<typename T>
	inline bool
	ItemsAreEq(
		const T& aObj1,
		const T& aObj2)
	{
		return ItemsComparisonEq<T>::Do(aObj1, aObj2);
	}

	template<typename T>
	inline void
	ItemsSwap(
		T* aItem1,
		T* aItem2)
	{
		T tmp = mg::common::Move(*aItem1);
		*aItem1 = mg::common::Move(*aItem2);
		*aItem2 = mg::common::Move(tmp);
	}

	template<typename T>
	inline void
	ItemsSwap(
		T* aItems1,
		T* aItems2,
		uint32_t aCount)
	{
		// Check first iteration separately to avoid the temporary
		// object creation.
		if (aCount == 0)
			return;
		T tmp;
		T* end1 = aItems1 + aCount;
		do {
			tmp = mg::common::Move(*aItems1);
			*aItems1 = mg::common::Move(*aItems2);
			*aItems2 = mg::common::Move(tmp);
			++aItems1;
			++aItems2;
		} while (aItems1 < end1);
	}

}
}
