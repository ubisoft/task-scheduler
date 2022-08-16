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
