cmake_minimum_required (VERSION 3.8)

set(mgcommon_src
	Array.h
	Assert.cpp
	Assert.h
	BinaryHeap.h
	Callback.h
	ConditionVariable.cpp
	ConditionVariable.h
	Definitions.h
	ForwardList.h
	HybridArray.h
	MultiConsumerQueue.h
	MultiConsumerQueueBase.cpp
	MultiConsumerQueueBase.h
	MultiProducerQueueIntrusive.h
	Mutex.cpp
	Mutex.h
	Policies.h
	PseudoVirtual.h
	QPTimer.h
	Random.cpp
	Random.h
	Signal.cpp
	Signal.h
	String.cpp
	String.h
	StringFunctions.cpp
	StringFunctions.h
	Thread.cpp
	Thread.h
	ThreadFunc.h
	Time.h
	Types.h
	TypeTraits.h
	Util.cpp
	Util.h
)

if(WIN32)
	set(mgcommon_src ${mgcommon_src}
		QPTimer_Win.cpp
		Thread_Win.cpp
		Time_Win.cpp
	)
else()
	set(mgcommon_src ${mgcommon_src}
		QPTimer_Posix.cpp
		Thread_Posix.cpp
		Time_Posix.cpp
	)
endif()

add_library(mgcommon ${mgcommon_src} "ThreadFunc.h")

target_include_directories(mgcommon PUBLIC
	${CMAKE_SOURCE_DIR}/src/
)
