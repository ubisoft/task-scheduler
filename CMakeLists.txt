cmake_minimum_required (VERSION 3.8)

project ("TaskScheduler")

set(CMAKE_CXX_STANDARD 11)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	add_compile_options(
		-Wall -Wextra -Wpedantic -Werror -Wunused-function
		-Wno-invalid-offsetof -Wno-unused-value
		-Wno-deprecated-copy
		-fno-exceptions
		-fno-rtti
	)
	if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		add_compile_options(-Wno-gnu-zero-variadic-macro-arguments)
	endif()

	set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-pthread")
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	add_compile_options(
		/Wall /WX /wd4324 /wd4355 /wd4365 /wd4458 /wd4514 /wd4625 /wd4626 /wd4668 /wd4710
		/wd4820 /wd5026 /wd5027 /wd5039 /wd5045 /wd5219
	)
endif()

add_subdirectory(src)
add_subdirectory(test)
