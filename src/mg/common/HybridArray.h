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

#pragma once

#include "mg/common/Array.h"

namespace mg {
namespace common {

	template<typename T, uint32 aStaticSize = 1>
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
		alignas(alignof(T)) uint8 myBuffer[aStaticSize * sizeof(T)];
	};

	//////////////////////////////////////////////////////////////

	template <typename T, uint32 aStaticSize>
	inline
	HybridArray<T, aStaticSize>::HybridArray()
		: Array<T>(aStaticSize, myBuffer)
	{
		static_assert(
			offsetof(HybridArray, myBuffer) == sizeof(Array<T>),
			"Static data should follow the base class"
		);
	}

	template <typename T, uint32 aStaticSize>
	inline
	HybridArray<T, aStaticSize>::HybridArray(
		const Array<T>& aOther)
		: Array<T>(aStaticSize, myBuffer, aOther)
	{
	}

	template <typename T, uint32 aStaticSize>
	inline
	HybridArray<T, aStaticSize>::HybridArray(
		const HybridArray& aOther)
		: Array<T>(aStaticSize, myBuffer, aOther)
	{
	}

	template <typename T, uint32 aStaticSize>
	inline
	HybridArray<T, aStaticSize>::HybridArray(
		Array<T>&& aOther)
		: Array<T>(aStaticSize, myBuffer, Move(aOther))
	{
	}

	template <typename T, uint32 aStaticSize>
	inline
	HybridArray<T, aStaticSize>::HybridArray(
		HybridArray&& aOther)
		: Array<T>(aStaticSize, myBuffer, Move(aOther))
	{
	}

	template <typename T, uint32 aStaticSize>
	inline HybridArray<T, aStaticSize>&
	HybridArray<T, aStaticSize>::operator=(
		const Array<T>& aOther)
	{
		return (HybridArray&)Array<T>::operator=(aOther);
	}

	template <typename T, uint32 aStaticSize>
	inline HybridArray<T, aStaticSize>&
	HybridArray<T, aStaticSize>::operator=(
		const HybridArray& aOther)
	{
		return (HybridArray&)Array<T>::operator=(aOther);
	}

	template <typename T, uint32 aStaticSize>
	inline HybridArray<T, aStaticSize>&
	HybridArray<T, aStaticSize>::operator=(
		Array<T>&& aOther)
	{
		return (HybridArray&)Array<T>::operator=(Move(aOther));
	}

	template <typename T, uint32 aStaticSize>
	inline HybridArray<T, aStaticSize>&
	HybridArray<T, aStaticSize>::operator=(
		HybridArray&& aOther)
	{
		return (HybridArray&)Array<T>::operator=(Move(aOther));
	}

}
}
