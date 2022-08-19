cmake_minimum_required (VERSION 3.8)

add_library(bench
	Bench.cpp
	Bench.h
)
target_link_libraries(bench
	mgcommon
)
target_include_directories(bench PUBLIC
	${CMAKE_SOURCE_DIR}/src
)

add_executable(bench_taskscheduler
	BenchTaskScheduler.cpp
	BenchTaskSchedulerTemplate.hpp
)
target_link_libraries(bench_taskscheduler
	mgserverbox
	bench
)

add_executable(bench_taskscheduler_trivial
	BenchTaskSchedulerTrivial.cpp
	BenchTaskSchedulerTemplate.hpp
)
target_link_libraries(bench_taskscheduler_trivial
	mgserverbox
	bench
)

add_executable(bench_mcspqueue
	BenchMultiConsumerQueue.cpp
	BenchMultiConsumerQueueTemplate.hpp
)
target_link_libraries(bench_mcspqueue
	mgcommon
	bench
)

add_executable(bench_mcspqueue_trivial
	BenchMultiConsumerQueueTrivial.cpp
	BenchMultiConsumerQueueTemplate.hpp
)
target_link_libraries(bench_mcspqueue_trivial
	mgcommon
	bench
)

add_executable(bench_mpscqueue
	BenchMultiProducerQueue.cpp
	BenchMultiProducerQueueTemplate.hpp
)
target_link_libraries(bench_mpscqueue
	mgcommon
	bench
)

add_executable(bench_mpscqueue_trivial
	BenchMultiProducerQueueTrivial.cpp
	BenchMultiProducerQueueTemplate.hpp
)
target_link_libraries(bench_mpscqueue_trivial
	mgcommon
	bench
)

add_executable(bench_signal
	BenchSignal.cpp
	BenchSignalTemplate.hpp
)
target_link_libraries(bench_signal
	mgcommon
	bench
)

add_executable(bench_signal_trivial
	BenchSignalTrivial.cpp
	BenchSignalTemplate.hpp
)
target_link_libraries(bench_signal_trivial
	mgcommon
	bench
)
