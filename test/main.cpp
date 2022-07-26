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

#include "mg/common/Types.h"

#include "UnitTest.h"

#include <cstdio>

namespace mg {
namespace unittests {

	void UnitTestArray();
	void UnitTestAtomic();
	void UnitTestBinaryHeap();
	void UnitTestCallback();
	void UnitTestConditionVariable();
	void UnitTestForwardList();
	void UnitTestMultiConsumerQueue();
	void UnitTestMultiProducerQueue();
	void UnitTestMutex();
	void UnitTestPolicies();
	void UnitTestSignal();
	void UnitTestTaskScheduler();

}
}

int
main()
{
	using namespace mg::unittests;

	Report("======== Unit tests ========");

	UnitTestArray();
	UnitTestAtomic();
	UnitTestBinaryHeap();
	UnitTestCallback();
	UnitTestConditionVariable();
	UnitTestForwardList();
	UnitTestMultiConsumerQueue();
	UnitTestMultiProducerQueue();
	UnitTestMutex();
	UnitTestPolicies();
	UnitTestSignal();
	UnitTestTaskScheduler();
	return 0;
}
