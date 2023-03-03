#include "mg/common/HybridArray.h"
#include "mg/common/ThreadFunc.h"
#include "mg/common/Time.h"

#include "UnitTest.h"

namespace mg {
namespace unittests {

	static void
	UnitTestMutexBasic()
	{
		mg::common::Mutex mutex;
		uint32_t counter = 0;
		const uint32_t threadCount = 10;

		mg::common::HybridArray<mg::common::ThreadFunc*, threadCount> threads;
		for (uint32_t i = 0; i < threadCount; ++i)
		{
			threads.Add(new mg::common::ThreadFunc([&]() {
				uint64_t deadline = mg::common::GetMilliseconds() + 2000;
				uint64_t yield = 0;
				while (mg::common::GetMilliseconds() < deadline)
				{
					mg::common::MutexLock lock(mutex);
					MG_COMMON_ASSERT(counter == 0);
					counter++;
					MG_COMMON_ASSERT(counter == 1);
					counter--;
					MG_COMMON_ASSERT(counter == 0);
					if (++yield % 1000 == 0)
						mg::common::Sleep(1);
				}
			}));
		}
		for (mg::common::ThreadFunc* f : threads)
			f->Start();
		threads.DeleteAll();
		MG_COMMON_ASSERT(counter == 0);
	}

	static void
	UnitTestMutexRecursive()
	{
		mg::common::Mutex mutex;
		mutex.Lock();
		mutex.Lock();
		bool rc = true;
		mg::common::ThreadCallback tryLockCallback([&]() {
			rc = mutex.TryLock();
			if (rc)
				mutex.Unlock();
		});
		mg::common::ThreadFunc* tryLockThread =
			new mg::common::ThreadFunc(tryLockCallback);
		tryLockThread->Start();
		tryLockThread->StopAndDelete();
		MG_COMMON_ASSERT(!rc);

		mutex.Unlock();
		rc = true;
		tryLockThread = new mg::common::ThreadFunc(tryLockCallback);
		tryLockThread->Start();
		tryLockThread->StopAndDelete();
		MG_COMMON_ASSERT(!rc);

		mutex.Unlock();
		rc = false;
		tryLockThread = new mg::common::ThreadFunc(tryLockCallback);
		tryLockThread->Start();
		tryLockThread->StopAndDelete();
		MG_COMMON_ASSERT(rc);
	}

	void
	UnitTestMutex()
	{
		TestSuiteGuard suite("Mutex");

		UnitTestMutexBasic();
		UnitTestMutexRecursive();
	}

}
}
