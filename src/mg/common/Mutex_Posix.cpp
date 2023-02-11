#include "Mutex.h"

#include "mg/common/Assert.h"
#include "mg/common/Thread.h"

namespace mg {
namespace common {

	Mutex::Mutex()
		: myOwner(0)
		, myLockCount(0)
	{
		pthread_mutexattr_t attr;

		MG_COMMON_ASSERT(pthread_mutexattr_init(&attr) == 0);
		MG_COMMON_ASSERT(pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE) == 0);
		MG_COMMON_ASSERT(pthread_mutex_init(&myHandle, &attr) == 0);
		MG_COMMON_ASSERT(pthread_mutexattr_destroy(&attr) == 0);
	}

	Mutex::~Mutex()
	{
		MG_COMMON_ASSERT(myLockCount == 0);
		MG_COMMON_ASSERT(pthread_mutex_destroy(&myHandle) == 0);
	}

	void
	Mutex::Lock()
	{
		if (TryLock())
			return;
		MG_COMMON_ASSERT(pthread_mutex_lock(&myHandle) == 0);
		++myLockCount;
		myOwner = GetCurrentThreadId();
	}

	bool
	Mutex::TryLock()
	{
		if (pthread_mutex_trylock(&myHandle) != 0)
		{
			theMutexStartContentCount.fetch_add(1);
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
		MG_COMMON_ASSERT(pthread_mutex_unlock(&myHandle) == 0);
	}

}
}
