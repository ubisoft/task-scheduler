// Copyright (C) 2022 Ubisoft Entertainment
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 2 of
// the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this program.  If not, see
// <https://www.gnu.org/licenses/>.

#pragma once

#include "mg/common/Types.h"

#if IS_PLATFORM_POSIX
#include <pthread.h>
#endif

F_DECLARE_CLASS(mg, common, ConditionVariable)

namespace mg {
namespace common {

#if IS_PLATFORM_POSIX
	using MutexHandle = pthread_mutex_t;
#else
	using MutexHandle = CRITICAL_SECTION;
#endif

	extern int64 theMutexStartContentCount;

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
