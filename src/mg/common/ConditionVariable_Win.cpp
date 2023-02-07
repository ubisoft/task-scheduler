#include "ConditionVariable.h"

#include "mg/common/Assert.h"

namespace mg {
namespace common {

	ConditionVariable::ConditionVariable()
	{
		::InitializeConditionVariable(&myHandle);
	}

	ConditionVariable::~ConditionVariable()
	{
	}

	void
	ConditionVariable::PrivWait(
		MutexHandle& aMutex)
	{
		::SleepConditionVariableCS(&myHandle, &aMutex, INFINITE);
	}

	void
	ConditionVariable::PrivTimedWait(
		MutexHandle& aMutex,
		uint32 aTimeoutMs,
		bool* aOutIsTimedOut)
	{
		if (::SleepConditionVariableCS(&myHandle, &aMutex, aTimeoutMs))
		{
			*aOutIsTimedOut = false;
		}
		else
		{
			MG_COMMON_ASSERT(::GetLastError() == ERROR_TIMEOUT);
			*aOutIsTimedOut = true;
		}
	}

	void
	ConditionVariable::Signal()
	{
		::WakeConditionVariable(&myHandle);
	}

	void
	ConditionVariable::Broadcast()
	{
		::WakeAllConditionVariable(&myHandle);
	}

}
}
