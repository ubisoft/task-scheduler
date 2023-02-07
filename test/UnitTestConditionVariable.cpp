#include "mg/common/Atomic.h"
#include "mg/common/ConditionVariable.h"
#include "mg/common/ThreadFunc.h"

#include "UnitTest.h"

#define UNIT_TEST_CONDVAR_TIMEOUT 10000

namespace mg {
namespace unittests {

	// The tests require some synchronization between threads to
	// make the tests stable and reproducible. And it should not
	// be cond vars, since they are being tested here. This is a
	// simple ping-pong a couple of functions to sync threads
	// using only CPU means.

	static inline void
	UnitTestCondVarSend(
		int32* aCounter,
		int32* aNextCounter)
	{
		*aNextCounter = mg::common::AtomicIncrement(aCounter) + 1;
	}

	static inline void
	UnitTestCondVarReceive(
		int32* aCounter,
		int32* aNextCounter)
	{
		while (mg::common::AtomicCompareExchange(aCounter, *aNextCounter,
			*aNextCounter) != *aNextCounter);
		++*aNextCounter;
	}

	static void
	UnitTestConditionVariableBasic()
	{
		mg::common::ConditionVariable var;
		mg::common::Mutex mutex;
		int32 stepCounter = 0;
		int32 next = 0;
		mg::common::ThreadFunc worker([&]() {
			int32 workerNext = 1;

			// Test that simple lock/unlock work correct.
			UnitTestCondVarReceive(&stepCounter, &workerNext);

			mutex.Lock();
			UnitTestCondVarSend(&stepCounter, &workerNext);
			var.Wait(mutex);
			MG_COMMON_ASSERT(mutex.IsOwnedByThisThread());
			mutex.Unlock();
			mutex.Lock();
			UnitTestCondVarSend(&stepCounter, &workerNext);

			// Test that timed wait correctly returns a timeout
			// error.
			UnitTestCondVarReceive(&stepCounter, &workerNext);

			bool isTimedOut = false;
			var.TimedWait(mutex, 100, &isTimedOut);
			MG_COMMON_ASSERT(isTimedOut);
			MG_COMMON_ASSERT(mutex.IsOwnedByThisThread());
			UnitTestCondVarSend(&stepCounter, &workerNext);

			// Test that timed wait does not set the flag if
			// there was no a timeout.
			UnitTestCondVarReceive(&stepCounter, &workerNext);

			UnitTestCondVarSend(&stepCounter, &workerNext);
			// Wait signal.
			var.TimedWait(mutex, UNIT_TEST_CONDVAR_TIMEOUT, &isTimedOut);
			MG_COMMON_ASSERT(!isTimedOut);
			MG_COMMON_ASSERT(mutex.IsOwnedByThisThread());

			UnitTestCondVarReceive(&stepCounter, &workerNext);
			UnitTestCondVarSend(&stepCounter, &workerNext);
			// Wait broadcast.
			var.TimedWait(mutex, UNIT_TEST_CONDVAR_TIMEOUT, &isTimedOut);
			MG_COMMON_ASSERT(!isTimedOut);
			MG_COMMON_ASSERT(mutex.IsOwnedByThisThread());

			mutex.Unlock();
		});
		worker.Start();

		// Test that simple lock/unlock work correct.
		UnitTestCondVarSend(&stepCounter, &next);
		UnitTestCondVarReceive(&stepCounter, &next);
		mutex.Lock();
		var.Signal();
		mutex.Unlock();
		UnitTestCondVarReceive(&stepCounter, &next);

		// Test that timed wait correctly returns a timeout
		// error.
		UnitTestCondVarSend(&stepCounter, &next);
		UnitTestCondVarReceive(&stepCounter, &next);

		// Test that timed wait does not set the flag if
		// was no a timeout.
		UnitTestCondVarSend(&stepCounter, &next);
		UnitTestCondVarReceive(&stepCounter, &next);
		mutex.Lock();
		var.Signal();
		mutex.Unlock();

		UnitTestCondVarSend(&stepCounter, &next);
		UnitTestCondVarReceive(&stepCounter, &next);
		mutex.Lock();
		var.Broadcast();
		mutex.Unlock();

		worker.BlockingStop();
	}

	void
	UnitTestConditionVariable()
	{
		TestSuiteGuard suite("ConditionVariable");

		UnitTestConditionVariableBasic();
	}

}
}
