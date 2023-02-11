#include "Mutex.h"

#include "mg/common/Atomic.h"
#include "mg/common/Thread.h"

namespace mg {
namespace common {

	int64_t theMutexStartContentCount = 0;

	void
	MutexStatClear()
	{
		mg::common::AtomicExchange64(&theMutexStartContentCount, 0);
	}

	uint64_t
	MutexStatContentionCount()
	{
		return (uint64_t)mg::common::AtomicLoad64(&theMutexStartContentCount);
	}

	void
	Mutex::Lock()
	{
		if (TryLock())
			return;
		myHandle.lock();
		myOwner = GetCurrentThreadId();
	}

	bool
	Mutex::TryLock()
	{
		if (!myHandle.try_lock())
		{
			mg::common::AtomicIncrement64(&theMutexStartContentCount);
			return false;
		}
		myOwner = GetCurrentThreadId();
		return true;
	}

	void
	Mutex::Unlock()
	{
		MG_COMMON_ASSERT(IsOwnedByThisThread());
		myOwner = 0;
		myHandle.unlock();
	}

	bool
	Mutex::IsOwnedByThisThread() const
	{
		return myOwner == GetCurrentThreadId();
	}

}
}
