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

#include "QPTimer.h"

#include "mg/common/Assert.h"

#include <ctime>

namespace mg {
namespace common {

	static uint64
	QPTimerGetNs()
	{
		struct timespec ts;
		MG_COMMON_ASSERT(clock_gettime(CLOCK_MONOTONIC_RAW, &ts) == 0);
		return ts.tv_sec * 1000000000 + ts.tv_nsec;
	}

	QPTimer::QPTimer()
		: myStartNs(0)
	{
	}

	void
	QPTimer::Start()
	{
		myStartNs = QPTimerGetNs();
	}

	double
	QPTimer::GetMilliSeconds()
	{
		return (QPTimerGetNs() - myStartNs) / 1000000.0;
	}

}
}
