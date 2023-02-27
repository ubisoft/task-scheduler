#include "Time.h"

#include <ctime>

namespace mg {
namespace common {

	uint64_t
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
