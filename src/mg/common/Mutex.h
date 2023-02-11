#pragma once

#include "mg/common/Types.h"

#if IS_PLATFORM_POSIX
#include <pthread.h>
#endif

#include <atomic>

F_DECLARE_CLASS(mg, common, ConditionVariable)

namespace mg {
namespace common {

#if IS_PLATFORM_POSIX
	using MutexHandle = pthread_mutex_t;
#else
	using MutexHandle = CRITICAL_SECTION;
#endif

	extern std::atomic<uint64> theMutexStartContentCount;

	void MutexStatClear();

	uint64 MutexStatContentionCount();

	class Mutex
	{
	public:
		Mutex();

		~Mutex();

		void Lock();

		bool TryLock();

		void Unlock();

		bool IsOwnedByThisThread() const;

	private:
		Mutex(
			const Mutex&) = delete;

		MutexHandle myHandle;
		uint32 myOwner;
		int myLockCount;

		friend class ConditionVariable;
	};

	class MutexLock
	{
	public:
		MutexLock(
			Mutex& aMutex);

		~MutexLock();

		void Unlock();

	private:
		MutexLock(
			const MutexLock&) = delete;
		MutexLock& operator=(
			const MutexLock&) = delete;

		Mutex* myMutex;
	};

	//////////////////////////////////////////////////////////////

	inline
	MutexLock::MutexLock(
		Mutex& aMutex)
		: myMutex(&aMutex)
	{
		myMutex->Lock();
	}

	inline
	MutexLock::~MutexLock()
	{
		if (myMutex != nullptr)
			myMutex->Unlock();
	}

	inline void
	MutexLock::Unlock()
	{
		if (myMutex == nullptr)
			return;
		myMutex->Unlock();
		myMutex = nullptr;
	}

}
}
