#include "Mutex.h"

#include "mg/common/Thread.h"

namespace mg {
namespace common {

	mg::common::AtomicU64 theMutexStartContentCount(0);

	void
	MutexStatClear()
	{
		theMutexStartContentCount.StoreRelaxed(0);
	}

	uint64_t
	MutexStatContentionCount()
	{
		return theMutexStartContentCount.LoadRelaxed();
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
			theMutexStartContentCount.IncrementRelaxed();
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
