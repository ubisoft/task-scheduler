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

#include "StringFunctions.h"

#include "mg/common/Assert.h"

#include <cctype>
#include <cstdio>

namespace mg {
namespace common {

	uint32
	Vsprintf(
		char* aBuffer,
		const char* aFmtString,
		va_list aArgList)
	{
		int rc = vsprintf(aBuffer, aFmtString, aArgList);
		MG_DEV_ASSERT(rc >= 0);
		return (uint32) rc;
	}

	uint32
	Vsnprintf(
		char* aBuffer,
		uint32 aBufferSize,
		const char* aFmtString,
		va_list aArgList)
	{
		int rc = vsnprintf(aBuffer, aBufferSize, aFmtString, aArgList);
		MG_DEV_ASSERT(rc >= 0);
		return (uint32) rc;
	}

	bool
	StringToNumber(
		const char* aString,
		uint64&	aOutNumber)
	{
		// Need to check for minus manually, because strtoull()
		// applies negation to the result. Despite the fact it
		// returns an unsigned type. Result is garbage.
		while (isspace(*aString))
			++aString;
		if (*aString == '-')
			return false;
		char* end;
		errno = 0;
		aOutNumber = strtoull(aString, &end, 10);
		return errno == 0 && *aString != 0 && *end == 0;
	}

	bool
	StringToNumber(
		const char* aString,
		uint32& aOutNumber)
	{
		uint64 res;
		if (StringToNumber(aString, res) && res <= UINT32_MAX)
		{
			aOutNumber = (uint32) res;
			return true;
		}
		return false;
	}

}
}
