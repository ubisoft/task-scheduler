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

#include "mg/common/ConditionVariable.h"
#include "mg/common/Mutex.h"

namespace mg {
namespace common {

	// Signal is a highly efficient way of blocking delivery of a
	// flag between threads.
	//
	// Firstly, signal send wakes up only one thread instead of
	// all the threads waiting for it. That helps to avoid
	// unnecessary wakeups, when the signal can only be handled by
	// one thread anyway. So called "thundering herd problem".
	//
	// Secondly, the signal can be received in a single
	// lock-unlock combination. No need for a separate clear()
	// call.
	//
	// Thirdly, the signal does not care about spurious wakeups.
	// So the caller's code must have a protection against that
	// if necessary. Usually it is not necessary, and in Signal
	// you don't pay for it.
	//
	// Fourthly, Signal attempts to do all the operations in a
	// lock-free way first.
	class Signal
	{
	public:
		Signal();

		~Signal();

		// Send wakes up one waiting thread, if there are any. If
		// there are no waiters, the signal stays here until a
		// receipt. If the signal is already installed, nothing
		// changes. To simulate broadcast the thread received the
		// signal first can check if there is more work to handle,
		// and re-send the signal to wake a next thread.
		bool Send();

		bool Receive();

		void ReceiveBlocking();

		// May return false not only in case of a timeout - also a
		// spurious wakeup is possible.
		bool ReceiveTimed(
			uint32 aTimeout);

	private:
		// State of the signal is a protection against the case
		// when Send() is done, it appears to be the first Send(),
		// but it didn't took the lock yet. And now Receive() is
		// called, takes the signal, and decides to delete the
		// Signal object thinking it must not be used already. It
		// would not be true, because Send() would try to take the
		// lock now, and would crash if the Signal is deleted
		// already.
		enum State
		{
			// Signal is empty.
			SIGNAL_STATE_EMPTY,
			// Signal send is in progress. Sender will try to take
			// the lock now to finish the signaling.
			SIGNAL_STATE_PROBE,
			// Signal send is fully finished. Or is not finished,
			// but the lock is taken then.
			SIGNAL_STATE_SIGNALED,
		};

		mg::common::ConditionVariable myCond;
		mg::common::Mutex myLock;
		int32 myState;
	};

}
}
