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

	bool
	Mutex::IsOwnedByThisThread() const
	{
		return myOwner == GetCurrentThreadId();
	}

}
}
