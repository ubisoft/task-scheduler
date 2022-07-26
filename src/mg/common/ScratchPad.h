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

#include "mg/common/String.h"

namespace mg {
namespace common {

	// Simplified version. In the original code the scratch pad is
	// a thread-local buffer which helps to avoid heap allocations
	// for temporary scoped data of big or unknown size.

	using ScratchBuffer = mg::common::HybridString<256>;

	MG_STRFORMAT_PRINTF(1, 2)
	static inline ScratchBuffer
	ScratchSprintf(
		const char* aFormat,
		...)
	{
		va_list va;
		va_start(va, aFormat);
		ScratchBuffer res;
		res.VFormat(aFormat, va);
		va_end(va);
		return res;
	}

	MG_STRFORMAT_PRINTF(1, 0)
	static inline ScratchBuffer
	ScratchVsprintf(
		const char* aFormat,
		va_list aArg)
	{
		ScratchBuffer res;
		res.VFormat(aFormat, aArg);
		return res;
	}

}
}
