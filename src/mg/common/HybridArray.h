#pragma once

#include "mg/common/Array.h"

namespace mg {
namespace common {

	template<typename T, uint32_t aStaticSize = 1>
	class HybridArray
		: public Array<T>
	{
	public:
		HybridArray();

		explicit HybridArray(
			const Array<T>& aOther);

		HybridArray(
			const HybridArray& aOther);

		explicit HybridArray(
			Array<T>&& aOther);

		HybridArray(
			HybridArray&& aOther);

		HybridArray& operator=(
			const Array<T>& aOther);

		HybridArray& operator=(
			const HybridArray& aOther);

		HybridArray& operator=(
			Array<T>&& aOther);

		HybridArray& operator=(
			HybridArray&& aOther);

	private:
		alignas(alignof(T)) uint8_t myBuffer[aStaticSize * sizeof(T)];
	};

	//////////////////////////////////////////////////////////////

	template <typename T, uint32_t aStaticSize>
	inline
	HybridArray<T, aStaticSize>::HybridArray()
		: Array<T>(aStaticSize, myBuffer)
	{
		static_assert(
			offsetof(HybridArray, myBuffer) == sizeof(Array<T>),
			"Static data should follow the base class"
		);
	}

	template <typename T, uint32_t aStaticSize>
	inline
	HybridArray<T, aStaticSize>::HybridArray(
		const Array<T>& aOther)
		: Array<T>(aStaticSize, myBuffer, aOther)
	{
	}

	template <typename T, uint32_t aStaticSize>
	inline
	HybridArray<T, aStaticSize>::HybridArray(
		const HybridArray& aOther)
		: Array<T>(aStaticSize, myBuffer, aOther)
	{
	}

	template <typename T, uint32_t aStaticSize>
	inline
	HybridArray<T, aStaticSize>::HybridArray(
		Array<T>&& aOther)
		: Array<T>(aStaticSize, myBuffer, Move(aOther))
	{
	}

	template <typename T, uint32_t aStaticSize>
	inline
	HybridArray<T, aStaticSize>::HybridArray(
		HybridArray&& aOther)
		: Array<T>(aStaticSize, myBuffer, Move(aOther))
	{
	}

	template <typename T, uint32_t aStaticSize>
	inline HybridArray<T, aStaticSize>&
	HybridArray<T, aStaticSize>::operator=(
		const Array<T>& aOther)
	{
		return (HybridArray&)Array<T>::operator=(aOther);
	}

	template <typename T, uint32_t aStaticSize>
	inline HybridArray<T, aStaticSize>&
	HybridArray<T, aStaticSize>::operator=(
		const HybridArray& aOther)
	{
		return (HybridArray&)Array<T>::operator=(aOther);
	}

	template <typename T, uint32_t aStaticSize>
	inline HybridArray<T, aStaticSize>&
	HybridArray<T, aStaticSize>::operator=(
		Array<T>&& aOther)
	{
		return (HybridArray&)Array<T>::operator=(Move(aOther));
	}

	template <typename T, uint32_t aStaticSize>
	inline HybridArray<T, aStaticSize>&
	HybridArray<T, aStaticSize>::operator=(
		HybridArray&& aOther)
	{
		return (HybridArray&)Array<T>::operator=(Move(aOther));
	}

}
}
