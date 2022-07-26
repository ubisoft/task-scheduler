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

#include "ConditionVariable.h"

#include "mg/common/Assert.h"

namespace mg {
namespace common {

	ConditionVariable::ConditionVariable()
	{
		pthread_condattr_t attr;
		MG_COMMON_ASSERT(pthread_condattr_init(&attr) == 0);
		MG_COMMON_ASSERT(pthread_condattr_setclock(&attr, CLOCK_MONOTONIC) == 0);
		MG_COMMON_ASSERT(pthread_cond_init(&myHandle, &attr) == 0);
		MG_COMMON_ASSERT(pthread_condattr_destroy(&attr) == 0);
	}

	ConditionVariable::~ConditionVariable()
	{
		MG_COMMON_ASSERT(pthread_cond_destroy(&myHandle) == 0);
	}

	void
	ConditionVariable::PrivWait(
		MutexHandle& aMutex)
	{
		MG_COMMON_ASSERT(pthread_cond_wait(&myHandle, &aMutex) == 0);
	}

	void
	ConditionVariable::PrivTimedWait(
		MutexHandle& aMutex,
		uint32 aTimeoutMs,
		bool* aOutIsTimedOut)
	{
		struct timespec ts;
		MG_COMMON_ASSERT(clock_gettime(CLOCK_MONOTONIC, &ts) == 0);
		if (aTimeoutMs < 1000)
		{
			ts.tv_nsec += aTimeoutMs * 1000000;
		}
		else
		{
			// Avoid calling '%' - division is expensive. '/' is
			// unavoidable.
			time_t sec = aTimeoutMs / 1000;
			ts.tv_sec += sec;
			ts.tv_nsec += (aTimeoutMs - sec * 1000) * 1000000;
		}
		if (ts.tv_nsec >= 1000000000)
		{
			ts.tv_nsec -= 1000000000;
			++ts.tv_sec;
		}
		int rc = pthread_cond_timedwait(&myHandle, &aMutex, &ts);
		MG_COMMON_ASSERT(rc == 0 || rc == ETIMEDOUT);
		*aOutIsTimedOut = (rc == ETIMEDOUT);
	}

	void
	ConditionVariable::Signal()
	{
		MG_COMMON_ASSERT(pthread_cond_signal(&myHandle) == 0);
	}

	void
	ConditionVariable::Broadcast()
	{
		MG_COMMON_ASSERT(pthread_cond_broadcast(&myHandle) == 0);
	}

}
}
