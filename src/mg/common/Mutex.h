#pragma once

#include "mg/common/Assert.h"
#include "mg/common/Types.h"

#include <atomic>
#include <mutex>

F_DECLARE_CLASS(mg, common, ConditionVariable)

namespace mg {
namespace common {

	extern std::atomic<uint64> theMutexStartContentCount;

	void MutexStatClear();

	uint64 MutexStatContentionCount();

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
		uint32 myOwner;

		friend class ConditionVariable;
	};

	class MutexLock
	{
	public:
		MutexLock(
			Mutex& aMutex) : myMutex(&aMutex) { myMutex->Lock(); }
		~MutexLock() { if (myMutex != nullptr) myMutex->Unlock(); }

		void Unlock();

	private:
		MutexLock(
			const MutexLock&) = delete;
		MutexLock& operator=(
			const MutexLock&) = delete;

		Mutex* myMutex;
	};

	//////////////////////////////////////////////////////////////

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
