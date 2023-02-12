#include "mg/common/Definitions.h"

#include "UnitTest.h"

#include <cstdio>

namespace mg {
namespace unittests {

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
	void UnitTestUtil();

}
}

int
main()
{
	using namespace mg::unittests;

	Report("======== Unit tests ========");

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
	UnitTestUtil();
	return 0;
}
