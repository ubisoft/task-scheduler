#include "mg/common/Assert.h"
#include "mg/common/Signal.h"
#include "mg/common/ThreadFunc.h"

#include "UnitTest.h"

#include <vector>

namespace mg {
namespace unittests {

	static void
	UnitTestSignalBasic()
	{
		mg::common::Signal s;
		MG_COMMON_ASSERT(!s.Receive());

		s.Send();
		MG_COMMON_ASSERT(s.Receive());
		MG_COMMON_ASSERT(!s.Receive());

		s.Send();
		s.ReceiveBlocking();
		MG_COMMON_ASSERT(!s.Receive());

		s.Send();
		MG_COMMON_ASSERT(s.ReceiveTimed(1000000));
		MG_COMMON_ASSERT(!s.Receive());

		s.Send();
		MG_COMMON_ASSERT(s.ReceiveTimed(0));
		MG_COMMON_ASSERT(!s.Receive());

		MG_COMMON_ASSERT(!s.ReceiveTimed(1));
	}

	static void
	UnitTestSignalStressSendAndReceive()
	{
		// The test checks that right after Receive() returns
		// success, it is safe to do with the signal anything.
		// Including deletion. Even if Send() in another thread is
		// not finished yet. Big number of signals is required so
		// as to catch the moment when the Send() thread is
		// interrupted right after setting the new state, but
		// before unlocking the mutex.
		const uint32_t count = 10000000;
		std::vector<mg::common::Signal*> signals;
		signals.reserve(count);
		for (uint32_t i = 0; i < count; ++i)
			signals.push_back(new mg::common::Signal());
		mg::common::ThreadFunc worker([&]() {
			uint32_t count = (uint32_t)signals.size();
			for (uint32_t i = 0; i < count; ++i)
				signals[i]->Send();
		});
		worker.Start();
		uint64_t yield = 0;
		for (uint32_t i = 0; i < count; ++i)
		{
			while (!signals[i]->Receive())
			{
				if (++yield % 10000 == 0)
					mg::common::Sleep(1);
			}
			delete signals[i];
		}
		worker.BlockingStop();
	}

	void
	UnitTestSignal()
	{
		TestSuiteGuard suite("Signal");

		UnitTestSignalBasic();
		UnitTestSignalStressSendAndReceive();
	}

}
}
