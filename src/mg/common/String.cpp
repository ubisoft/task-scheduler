#include "String.h"

#include <cstdio>

namespace mg {
namespace common {

	char&
	IString::operator[](
		int aIndex)
	{
		MG_COMMON_ASSERT(aIndex >= 0);
		MG_COMMON_ASSERT(aIndex == 0 || (uint32_t)aIndex < mySize);
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
		MG_COMMON_ASSERT(aIndex == 0 || (uint32_t)aIndex < mySize);
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
			uint32_t len = mg::common::Strlen(aString);
			SetLength(len);
			memcpy(myData, aString, len);
		}
	}

	void
	IString::Set(
		const char* aString,
		uint32_t aLength)
	{
		SetLength(aLength);
		memcpy(myData, aString, aLength);
	}

	void
	IString::Append(
		const char* aString)
	{
		uint32_t len = mg::common::Strlen(aString);
		uint32_t oldLen = GetLength();
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
		uint32_t oldLen = GetLength();
		uint32_t bufferSize = mySize - oldLen;
		uint32_t newLen = mg::common::Vsnprintf(myData + oldLen, bufferSize,
			aFormatString, aParams);
		uint32_t newSize = newLen + 1;
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
		uint32_t aNewSize)
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
