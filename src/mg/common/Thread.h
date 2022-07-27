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
#include "mg/common/String.h"
#include "mg/common/Types.h"

#include <thread>

#define MG_THREADLOCAL thread_local

namespace mg {
namespace common {

	using ThreadId = uint32;

	class Thread
	{
	public:
		// Thread name should be a short string up to 15 bytes not
		// including terminating 0 (15 printable symbols). The
		// reason is that on Linux thread names are truncated to
		// 16 bytes.
		Thread(
			const char*	aName = "anon_thread");

		void Start();

		void Stop();

		void BlockingStop();

		void StopAndDelete();

		bool StopRequested();

		bool IsRunning();

	protected:
		virtual ~Thread();

	private:
		void PrivTrampoline();

		virtual void Run() = 0;

		Mutex myLock;
		ConditionVariable myCond;
		std::thread* myHandle;
		HybridString<64> myName;
		int32 myIsStopRequested;
		int32 myIsRunning;
		bool myWasStarted;
	};

	void Sleep(
		uint32 aTimeMillis);

	ThreadId GetCurrentThreadId();

}
}
