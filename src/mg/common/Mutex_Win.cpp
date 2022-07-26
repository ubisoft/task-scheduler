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

#include "Mutex.h"

#include "mg/common/Assert.h"
#include "mg/common/Thread.h"

namespace mg {
namespace common {

	Mutex::Mutex()
		: myOwner(0)
		, myLockCount(0)
	{
		::InitializeCriticalSectionEx(&myHandle, 0, 0);
	}

	Mutex::~Mutex()
	{
		::DeleteCriticalSection(&myHandle);
	}

	void
	Mutex::Lock()
	{
		::EnterCriticalSection(&myHandle);
		++myLockCount;
		myOwner = GetCurrentThreadId();
	}

	bool
	Mutex::TryLock()
	{
		if (!::TryEnterCriticalSection(&myHandle))
			return false;
		++myLockCount;
		myOwner = GetCurrentThreadId();
		return true;
	}

	void
	Mutex::Unlock()
	{
		MG_COMMON_ASSERT(myLockCount > 0);
		MG_COMMON_ASSERT(IsOwnedByThisThread());
		if (--myLockCount == 0)
			myOwner = 0;
		::LeaveCriticalSection(&myHandle);

	}

}
}
