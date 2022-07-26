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

#include "mg/common/Assert.h"
#include "mg/common/HybridArray.h"
#include "mg/common/Signal.h"
#include "mg/common/ThreadFunc.h"

#include "UnitTest.h"

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
		const uint32 count = 10000000;
		mg::common::HybridArray<mg::common::Signal*> signals;
		signals.Reserve(count);
		for (uint32 i = 0; i < count; ++i)
			signals.Add(new mg::common::Signal());
		mg::common::ThreadFunc worker([&]() {
			uint32 count = signals.Count();
			for (uint32 i = 0; i < count; ++i)
				signals[i]->Send();
		});
		worker.Start();
		uint64 yield = 0;
		for (uint32 i = 0; i < count; ++i)
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
