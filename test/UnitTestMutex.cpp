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
		uint32 counter = 0;
		const uint32 threadCount = 10;

		mg::common::HybridArray<mg::common::ThreadFunc*, threadCount> threads;
		for (uint32 i = 0; i < threadCount; ++i)
		{
			threads.Add(new mg::common::ThreadFunc([&]() {
				uint64 deadline = mg::common::GetMilliseconds() + 2000;
				uint64 yield = 0;
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

	void
	UnitTestMutex()
	{
		TestSuiteGuard suite("Mutex");

		UnitTestMutexBasic();
	}

}
}
