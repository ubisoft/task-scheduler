cmake_minimum_required (VERSION 3.8)

add_executable(test
	main.cpp
	UnitTest.cpp
	UnitTest.h
	UnitTestBinaryHeap.cpp
	UnitTestConditionVariable.cpp
	UnitTestForwardList.cpp
	UnitTestMultiConsumerQueue.cpp
	UnitTestMultiProducerQueue.cpp
	UnitTestMutex.cpp
	UnitTestSignal.cpp
	UnitTestTaskScheduler.cpp
	UnitTestUtil.cpp
)

target_include_directories(test PUBLIC
	${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(test
	mgserverbox
)
