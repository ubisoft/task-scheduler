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
