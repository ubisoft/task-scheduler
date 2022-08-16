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

#include "mg/common/Types.h"

#include <stdarg.h>
#include <string.h>

namespace mg {
namespace common {

	static inline uint32
	Strlen(
		const char* aString)
	{
		return (uint32) strlen(aString);
	}

	static inline int
	Strcmp(
		const char* aA,
		const char* aB)
	{
		return strcmp(aA, aB);
	}

	static inline int
	Strncmp(
		const char* aA,
		const char* aB,
		uint32 aCount)
	{
		return strncmp(aA, aB, aCount);
	}

	MG_STRFORMAT_PRINTF(2, 0)
	uint32 Vsprintf(
		char* aBuffer,
		const char* aFmtString,
		va_list aArgList);

	MG_STRFORMAT_PRINTF(3, 0)
	uint32 Vsnprintf(
		char* aBuffer,
		uint32 aBufferSize,
		const char* aFmtString,
		va_list aArgList);

	bool StringToNumber(
		const char* aString,
		uint32& aOutNumber);

	bool StringToNumber(
		const char* aString,
		uint64& aOutNumber);

}
}
