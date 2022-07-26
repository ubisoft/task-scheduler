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

	void
	ConditionVariable::Wait(
		Mutex& aMutex)
	{
		MG_COMMON_ASSERT(aMutex.IsOwnedByThisThread());
		MG_COMMON_ASSERT(aMutex.myLockCount == 1);
		uint32 tid = aMutex.myOwner;
		aMutex.myOwner = 0;
		aMutex.myLockCount = 0;

		PrivWait(aMutex.myHandle);

		MG_COMMON_ASSERT(aMutex.myOwner == 0);
		MG_COMMON_ASSERT(aMutex.myLockCount == 0);
		aMutex.myOwner = tid;
		aMutex.myLockCount = 1;
	}

	void
	ConditionVariable::TimedWait(
		Mutex& aMutex,
		uint32 aTimeoutMs,
		bool* aOutIsTimedOut)
	{
		MG_COMMON_ASSERT(aMutex.IsOwnedByThisThread());
		MG_COMMON_ASSERT(aMutex.myLockCount == 1);
		uint32 tid = aMutex.myOwner;
		aMutex.myOwner = 0;
		aMutex.myLockCount = 0;

		PrivTimedWait(aMutex.myHandle, aTimeoutMs, aOutIsTimedOut);

		MG_COMMON_ASSERT(aMutex.myOwner == 0);
		MG_COMMON_ASSERT(aMutex.myLockCount == 0);
		aMutex.myOwner = tid;
		aMutex.myLockCount = 1;
	}

}
}
