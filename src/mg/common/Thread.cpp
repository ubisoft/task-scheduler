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

#include "Thread.h"

#include "mg/common/Assert.h"
#include "mg/common/Atomic.h"
#include "mg/common/Util.h"

namespace mg {
namespace common {

	void ThreadSetCurrentName(
		const char* aName);

	Thread::Thread(
		const char* aName)
		: myHandle(nullptr)
		, myName(aName)
		, myIsStopRequested(0)
		, myIsRunning(0)
		, myWasStarted(false)
	{
	}

	Thread::~Thread()
	{
		// Owner of the thread *must* stop it before destruction.
		// Or at least wait until it starts saying, that it is
		// not running.
		MG_COMMON_ASSERT(!IsRunning());
		// But even if a thread "is not running", in fact it
		// still can be
		// 1) in the trampoline, cleaning up some resources,
		//    calling termination callbacks, etc. Deletion of
		//    the thread memory can cause use-after-free in the
		//    trampoline;
		// 2) not freed, because CloseHandle/pthread_join were
		//    not called.
		//
		// The latter can happen IFF a user didn't do it by
		// himself. For example, if his thread function was not a
		// cycle waiting on 'StopRequested()'. A user could just
		// check 'IsRunning() == false', and delete the thread.
		// In this case the call below is rather about collection
		// of the thread handle, than about real waiting for a
		// termination.
		BlockingStop();
	}

	void
	Thread::Start()
	{
		MG_COMMON_ASSERT(!myWasStarted);
		MG_COMMON_ASSERT(!StopRequested());
		myWasStarted = true;
		MG_COMMON_ASSERT(mg::common::AtomicFlagSet(&myIsRunning) == 0);
		myHandle = new std::thread(&Thread::PrivTrampoline, this);
	}

	void
	Thread::Stop()
	{
		myLock.Lock();
		mg::common::AtomicFlagSet(&myIsStopRequested);
		myCond.Broadcast();
		myLock.Unlock();
	}

	void
	Thread::BlockingStop()
	{
		Stop();
		myLock.Lock();
		while (IsRunning())
			myCond.Wait(myLock);
		if (myHandle != nullptr)
		{
			myHandle->join();
			delete myHandle;
			myHandle = nullptr;
		}
		myLock.Unlock();
	}

	void
	Thread::StopAndDelete()
	{
		BlockingStop();
		delete this;
	}

	bool
	Thread::StopRequested()
	{
		return mg::common::AtomicFlagTest(&myIsStopRequested) != 0;
	}

	bool
	Thread::IsRunning()
	{
		return mg::common::AtomicFlagTest(&myIsRunning) != 0;
	}

	void
	Thread::PrivTrampoline()
	{
		ThreadSetCurrentName(myName);
		Run();
		myLock.Lock();
		MG_COMMON_ASSERT(mg::common::AtomicFlagClear(&myIsRunning) != 0);
		myCond.Broadcast();
		myLock.Unlock();
	}

}
}
