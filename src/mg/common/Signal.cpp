#include "Signal.h"

#include "mg/common/Assert.h"
#include "mg/common/Atomic.h"

namespace mg {
namespace common {

	Signal::Signal()
		: myState(SIGNAL_STATE_EMPTY)
	{
	}

	Signal::~Signal()
	{
		// Lock-unlock to purge all the signal senders who didn't
		// leave the critical section yet. Deletion could be done
		// as a result of successful Receive() making it possible
		// that the signal sender didn't unlock the mutex yet
		// after setting the state signaled.
		myLock.Lock();
		myLock.Unlock();
	}

	void
	Signal::Send()
	{
		State oldState = SIGNAL_STATE_EMPTY;
		// If it was already set, then no need to proceed. The
		// condition variable is signaled by another thread. That
		// is one of the key features of Signal - lock-free signal
		// re-send if it is not consumed yet.
		if (!myState.CmpExchgStrongRelease(oldState, SIGNAL_STATE_PROBE))
			return;

		// The signal must be done under a lock. Consider how the
		// blocking receive works:
		//
		//     lock();
		//     while (!receive())  (1)
		//	       wait();         (2)
		//	   unlock();
		//
		// If the signal wouldn't be locked, it could be emitted
		// between (1) and (2). Then wait() would be infinite even
		// though there is a pending signal.
		//
		// Another point why the send must be finished with a
		// lock - the receiver thread may decide to delete the
		// Signal object if Receive() succeeds. Consider this
		// example:
		//
		//     Sender:              Receiver:
		//   sig.Send();        if (sig->Receive())
		//                          delete sig;
		//
		// In case Send() wouldn't lock the mutex during setting
		// the signaled state, the receiver thread would receive
		// the signal and delete it making the sender thread
		// crash.
		myLock.Lock();
		oldState = myState.ExchangeRelease(SIGNAL_STATE_SIGNALED);
		MG_COMMON_ASSERT(oldState == SIGNAL_STATE_PROBE);
		myCond.Signal();
		myLock.Unlock();
	}

	bool
	Signal::Receive()
	{
		// It is not safe to return success if the state is probe.
		// Because the signal owner may decide to delete the
		// signal then, and the signal sender will try to lock
		// the mutex after its deletion.
		// Still the operation must remain lock-free, as it is one
		// of the key features of Signal.
		State oldState = SIGNAL_STATE_SIGNALED;
		return myState.CmpExchgStrongAcquire(oldState, SIGNAL_STATE_EMPTY);
	}

	void
	Signal::ReceiveBlocking()
	{
		if (Receive())
			return;

		myLock.Lock();
		while (!Receive())
			myCond.Wait(myLock);
		myLock.Unlock();
	}

	bool
	Signal::ReceiveTimed(
		uint32_t aTimeout)
	{
		if (Receive())
			return true;

		myLock.Lock();
		bool rc = Receive();
		if (!rc)
		{
			myCond.TimedWait(myLock, aTimeout, &rc);
			rc = Receive();
			// The receive still may return false, even if the
			// timeout didn't pass yet. This is usually enough,
			// and allows not to care about retries, getting
			// current time, calculating the timeout again, etc.
			// In case it would be ever needed, better introduce
			// ReceiveBlockingTimed() separately.
		}
		myLock.Unlock();
		return rc;
	}

}
}
