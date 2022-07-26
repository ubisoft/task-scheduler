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

#include "String.h"

#include <cstdio>

namespace mg {
namespace common {

	char&
	IString::operator[](
		int aIndex)
	{
		MG_COMMON_ASSERT(aIndex >= 0);
		MG_COMMON_ASSERT(aIndex == 0 || (uint32)aIndex < mySize);
		return myData[aIndex];
	}

	char&
	IString::operator[](
		unsigned int aIndex)
	{
		MG_COMMON_ASSERT(aIndex == 0 || aIndex < mySize);
		return myData[aIndex];
	}

	const char&
	IString::operator[](
		int aIndex) const
	{
		MG_COMMON_ASSERT(aIndex >= 0);
		MG_COMMON_ASSERT(aIndex == 0 || (uint32)aIndex < mySize);
		return myData[aIndex];
	}

	const char&
	IString::operator[](
		unsigned int aIndex) const
	{
		MG_COMMON_ASSERT(aIndex == 0 || aIndex < mySize);
		return myData[aIndex];
	}

	void
	IString::Set(
		const char* aString)
	{
		if (aString == nullptr)
		{
			SetLength(0);
		}
		else
		{
			uint32 len = mg::common::Strlen(aString);
			SetLength(len);
			memcpy(myData, aString, len);
		}
	}

	void
	IString::Set(
		const char* aString,
		uint32 aLength)
	{
		SetLength(aLength);
		memcpy(myData, aString, aLength);
	}

	void
	IString::Append(
		const char* aString)
	{
		uint32 len = mg::common::Strlen(aString);
		uint32 oldLen = GetLength();
		SetLength(len + oldLen);
		memcpy(myData + oldLen, aString, len);
	}

	IString&
	IString::AppendVFormat(
		const char* aFormatString,
		va_list aParams)
	{
		va_list savepoint;
		va_copy(savepoint, aParams);
		uint32 oldLen = GetLength();
		uint32 bufferSize = mySize - oldLen;
		uint32 newLen = mg::common::Vsnprintf(myData + oldLen, bufferSize,
			aFormatString, aParams);
		uint32 newSize = newLen + 1;
		SetLength(newLen + oldLen);
		if (newSize > bufferSize)
		{
			MG_COMMON_ASSERT(mg::common::Vsnprintf(myData + oldLen, newSize,
				aFormatString, savepoint) == newLen);
		}
		va_end(savepoint);
		return *this;
	}

	void
	IString::PrivRealloc(
		uint32 aNewSize)
	{
		if (aNewSize <= mySize)
			return;

		char* newData = new char[aNewSize];
		memcpy(newData, myData, mySize);
		if (IsDynamic())
			delete [] myData;

		myData = newData;
		mySize = aNewSize;
	}

}
}
