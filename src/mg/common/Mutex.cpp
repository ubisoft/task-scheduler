#include "Mutex.h"

#include "mg/common/Thread.h"

#include <atomic>

namespace mg {
namespace common {

	std::atomic<uint64> theMutexStartContentCount(0);

	void
	MutexStatClear()
	{
		theMutexStartContentCount.store(0);
	}

	uint64
	MutexStatContentionCount()
	{
		return theMutexStartContentCount.load();
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
			theMutexStartContentCount.fetch_add(1);
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
