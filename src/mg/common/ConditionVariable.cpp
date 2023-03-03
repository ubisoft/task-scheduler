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
		uint32_t tid = aMutex.myOwner;
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
		uint32_t aTimeoutMs,
		bool* aOutIsTimedOut)
	{
		MG_COMMON_ASSERT(aMutex.IsOwnedByThisThread());
		MG_COMMON_ASSERT(aMutex.myLockCount == 1);
		uint32_t tid = aMutex.myOwner;
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
