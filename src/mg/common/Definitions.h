#pragma once

#ifdef __linux__
#define IS_PLATFORM_POSIX 1
#define IS_PLATFORM_WIN 0
#define IS_PLATFORM_APPLE 0
#elif _WIN32
#define IS_PLATFORM_POSIX 0
#define IS_PLATFORM_WIN 1
#define IS_PLATFORM_APPLE 0
#elif __APPLE__
#define IS_PLATFORM_POSIX 1
#define IS_PLATFORM_WIN 0
#define IS_PLATFORM_APPLE 1
#else
#error "Uknown platform"
#endif

#ifdef __clang__
#define IS_COMPILER_CLANG 1
#define IS_COMPILER_GCC 0
#define IS_COMPILER_MSVC 0
#elif defined(_MSC_VER)
#define IS_COMPILER_CLANG 0
#define IS_COMPILER_GCC 0
#define IS_COMPILER_MSVC 1
#elif defined(__GNUC__)
#define IS_COMPILER_CLANG 0
#define IS_COMPILER_GCC 1
#define IS_COMPILER_MSVC 0
#else
#error "Unknown compiler"
#endif


#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L )
#define IS_CPP_AT_LEAST_17 1
#endif

#if NDEBUG
#define IS_BUILD_RELEASE 1
#define IS_BUILD_DEBUG 0
#else
#define IS_BUILD_RELEASE 0
#define IS_BUILD_DEBUG 1
#endif

#if IS_PLATFORM_WIN

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define _CRT_RAND_S
#define _CRT_SECURE_NO_WARNINGS

#endif

#if IS_PLATFORM_POSIX
#define MG_FORCEINLINE inline __attribute((always_inline))
#define MG_NOINLINE __attribute((noinline))
#elif IS_PLATFORM_WIN
#define MG_FORCEINLINE __forceinline
#define MG_NOINLINE __declspec(noinline)
#endif

// Use this value when need an infinite deadline. It can be
// handled in a special cheap way in some subsystems, and is
// more explicit than inlined UINT64_MAX.
#define MG_DEADLINE_INFINITE UINT64_MAX

// Prepend with "1" in case the arguments are empty. To prevent
// sizeof() compilation fail.
// Even if the arguments perform calculations, they won't happen
// during runtime. The compiler deducts sizeof() result at compile
// time.
#define MG_UNUSED(...) ((void)sizeof(1, ##__VA_ARGS__))

#if IS_COMPILER_CLANG
// sindex - format string argument index, 1 based.
// argindex - index of the first parameter of the format string,
//   1 based. If arguments are passed as va_list, or not passed,
//   then set it to 0.
// For non static class methods the implicit 'this' argument
// should be accounted, making the indexes 2 based.
#define MG_STRFORMAT_PRINTF(sindex, argindex) \
	__attribute__ ((format (printf, sindex, argindex)))
#define MG_STRFORMAT_SCANF(sindex, argindex) \
	__attribute__ ((format (scanf, sindex, argindex)))
#define MG_UNUSED_MEMBER __attribute__((unused))
#else
#define MG_STRFORMAT_PRINTF(sindex, argindex)
#define MG_STRFORMAT_SCANF(sindex, argindex)
#define MG_UNUSED_MEMBER
#endif

