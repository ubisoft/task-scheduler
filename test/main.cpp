#include "mg/common/Types.h"

#include "UnitTest.h"

#include <cstdio>

namespace mg {
namespace unittests {

	void UnitTestArray();
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
