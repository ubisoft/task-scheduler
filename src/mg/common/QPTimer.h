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

namespace mg {
namespace common {

	class QPTimer
	{
	public:
		QPTimer();

		void Start();

		double GetMilliSeconds();

	private:
#if IS_PLATFORM_POSIX
		uint64 myStartNs;
#else
		uint64 myStartQuant;
#endif
	};

}
}
