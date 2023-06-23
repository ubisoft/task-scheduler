cmake_minimum_required (VERSION 3.8)

add_library(mgserverbox
	Task.cpp
	Task.h
	TaskScheduler.cpp
	TaskScheduler.h
)

target_include_directories(mgserverbox PUBLIC
	${CMAKE_SOURCE_DIR}/src/
)

target_link_libraries(mgserverbox
	mgcommon
)

set(install_headers
	Task.h
	TaskScheduler.h
)

install(TARGETS mgserverbox DESTINATION "${install_lib_root}")
install(FILES ${install_headers} DESTINATION "${install_include_root}/mg/serverbox/")
