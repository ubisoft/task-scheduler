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
		::InitializeConditionVariable(&myHandle);
	}

	ConditionVariable::~ConditionVariable()
	{
	}

	void
	ConditionVariable::PrivWait(
		MutexHandle& aMutex)
	{
		::SleepConditionVariableCS(&myHandle, &aMutex, INFINITE);
	}

	void
	ConditionVariable::PrivTimedWait(
		MutexHandle& aMutex,
		uint32 aTimeoutMs,
		bool* aOutIsTimedOut)
	{
		if (::SleepConditionVariableCS(&myHandle, &aMutex, aTimeoutMs))
		{
			*aOutIsTimedOut = false;
		}
		else
		{
			MG_COMMON_ASSERT(::GetLastError() == ERROR_TIMEOUT);
			*aOutIsTimedOut = true;
		}
	}

	void
	ConditionVariable::Signal()
	{
		::WakeConditionVariable(&myHandle);
	}

	void
	ConditionVariable::Broadcast()
	{
		::WakeAllConditionVariable(&myHandle);
	}

}
}
