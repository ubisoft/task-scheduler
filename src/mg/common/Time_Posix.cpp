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

#include "Time.h"

#include <ctime>

namespace mg {
namespace common {

	uint64
	GetMilliseconds()
	{
		timespec ts;
#if IS_PLATFORM_APPLE
		const clockid_t clockid = CLOCK_MONOTONIC;
#else
		// Boottime is preferable - it takes system suspension time into account.
		const clockid_t clockid = CLOCK_BOOTTIME;
#endif
		clock_gettime(clockid, &ts);
		return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
	}

}
}
