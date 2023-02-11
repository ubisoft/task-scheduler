#pragma once

#include "mg/common/Assert.h"

#include <atomic>
#include <mutex>

F_DECLARE_CLASS(mg, common, ConditionVariable)

namespace mg {
namespace common {

	extern int64_t theMutexStartContentCount;

	void MutexStatClear();

	uint64_t MutexStatContentionCount();

	class Mutex
	{
	public:
		Mutex() : myOwner(0) {}
		~Mutex() { MG_COMMON_ASSERT(myOwner == 0); }

		void Lock();
		bool TryLock();
		void Unlock();
		bool IsOwnedByThisThread() const;

	private:
		Mutex(
			const Mutex&) = delete;

		std::mutex myHandle;
		uint32_t myOwner;

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
