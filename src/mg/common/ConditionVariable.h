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

#include "mg/common/Mutex.h"
#include "mg/common/Types.h"

namespace mg {
namespace common {

#if IS_PLATFORM_POSIX
	using ConditionVarHandle = pthread_cond_t;
#else
	using ConditionVarHandle = CONDITION_VARIABLE;
#endif

	// Condition variable allows to atomically unlock a mutex and
	// lock on a condition until a signal. This is vital for some
	// tasks (or very strongly simplifies them). For example,
	// assume that a thread wants an event. Event is a certain
	// value of a variable. The variable is protected by a mutex.
	// To check its value the thread needs to lock the mutex,
	// check the variable. If it does not match the needed value,
	// the thread should unlock the mutex and go to sleep.
	//
	// What if the event happens exactly after the thread unlocked
	// the mutex? In that case it will go to sleep forever,
	// because it missed the needed value already. Condition
	// variable solves exactly that problem - unlock a mutex and
	// go to sleep atomically.
	//
	class ConditionVariable
	{
	public:
		ConditionVariable();

		~ConditionVariable();

		void Wait(
			Mutex& aMutex);

		void TimedWait(
			Mutex& aMutex,
			uint32 aTimeoutMs,
			bool* aOutIsTimedOut);

		void Signal();

		void Broadcast();

	private:
		void PrivWait(
			MutexHandle& aMutex);

		void PrivTimedWait(
			MutexHandle& aMutex,
			uint32 aTimeoutMs,
			bool* aOutIsTimedOut);

		ConditionVariable(
			const ConditionVariable&) = delete;
		ConditionVariable& operator=(
			const ConditionVariable&) = delete;

		ConditionVarHandle myHandle;
	};

}
}
