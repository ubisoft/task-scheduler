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

	bool
	Mutex::IsOwnedByThisThread() const
	{
		return myOwner == GetCurrentThreadId();
	}

}
}
