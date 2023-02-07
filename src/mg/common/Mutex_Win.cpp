#include "Mutex.h"

#include "mg/common/Assert.h"
#include "mg/common/Atomic.h"
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
		if (TryLock())
			return;
		::EnterCriticalSection(&myHandle);
		++myLockCount;
		myOwner = GetCurrentThreadId();
	}

	bool
	Mutex::TryLock()
	{
		if (!::TryEnterCriticalSection(&myHandle))
		{
			mg::common::AtomicIncrement64(&theMutexStartContentCount);
			return false;
		}
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
