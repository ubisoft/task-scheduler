#include "Thread.h"

#include "mg/common/Assert.h"
#include "mg/common/Util.h"

namespace mg {
namespace common {

	void ThreadSetCurrentName(
		const char* aName);

	Thread::Thread(
		const char* aName)
		: myHandle(nullptr)
		, myName(aName)
		, myIsStopRequested(false)
		, myIsRunning(false)
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
		MG_COMMON_ASSERT(!myIsRunning.exchange(true));
		myHandle = new std::thread(&Thread::PrivTrampoline, this);
	}

	void
	Thread::Stop()
	{
		myLock.Lock();
		myIsStopRequested.store(true);
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
		return myIsStopRequested.load();
	}

	bool
	Thread::IsRunning()
	{
		return myIsRunning.load();
	}

	void
	Thread::PrivTrampoline()
	{
		ThreadSetCurrentName(myName.c_str());
		Run();
		myLock.Lock();
		MG_COMMON_ASSERT(myIsRunning.exchange(false));
		myCond.Broadcast();
		myLock.Unlock();
	}

}
}
