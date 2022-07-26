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

namespace mg {
namespace common {

	static_assert(sizeof(LARGE_INTEGER) == sizeof(uint64), "LARGE_INTEGER is 64 bit");

	static LONGLONG
	QPTimerCalcFrequency()
	{
		LARGE_INTEGER freq;
		::QueryPerformanceFrequency(&freq);
		return freq.QuadPart;
	}

	static double
	QPTimerGetUnitsPerMs()
	{
		// Calculate it just once when request it first time.
		static double res = 1000.0 / QPTimerCalcFrequency();
		return res;
	}

	QPTimer::QPTimer()
		: myStartQuant(0)
	{
	}

	void
	QPTimer::Start()
	{
		::QueryPerformanceCounter((LARGE_INTEGER*)&myStartQuant);
	}

	double
	QPTimer::GetMilliSeconds()
	{
		LARGE_INTEGER ts;
		QueryPerformanceCounter(&ts);
		return (ts.QuadPart - myStartQuant) * QPTimerGetUnitsPerMs();
	}

}
}
