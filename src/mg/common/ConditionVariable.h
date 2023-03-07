#pragma once

#include "mg/common/Mutex.h"

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
			uint32_t aTimeoutMs,
			bool* aOutIsTimedOut);

		void Signal();

		void Broadcast();

	private:
		void PrivWait(
			MutexHandle& aMutex);

		void PrivTimedWait(
			MutexHandle& aMutex,
			uint32_t aTimeoutMs,
			bool* aOutIsTimedOut);

		ConditionVariable(
			const ConditionVariable&) = delete;
		ConditionVariable& operator=(
			const ConditionVariable&) = delete;

		ConditionVarHandle myHandle;
	};

}
}
