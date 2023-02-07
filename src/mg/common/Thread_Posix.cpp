#include "Thread.h"

#include "mg/common/Time.h"

#include <sys/syscall.h>
#include <unistd.h>
#include <xmmintrin.h>

namespace mg {
namespace common {

	// Thread ID and PID on Linux are a sad story. There is no a
	// standard library wrapper for gettid() system call, and
	// Linus Torvalds personally says that caching of gettid() or
	// getpid() inside the standard lib is a bad idea. At least,
	// because it is not possible to detect when clone() is
	// called and the tid must be changed in the cache. So the
	// tid either should not be cached, or the application should
	// cache it by own means. Caching significantly improves
	// perf, because syscall(SYS_gettid) appeared to be very
	// slow, as any system call bypassing [vdso] optimization.
	static MG_THREADLOCAL ThreadId theThreadID = 0;

	void
	ThreadSetCurrentName(
		const char* aName)
	{
		// 16 bytes including terminating 0 is a kernel level
		// restriction. Can't be workarounded. Raise of an error
		// or doing nothing are not a solution - most of threads
		// (if not all) have names longer than 15 characters.
		// Name is truncated instead.
		char buffer[16];
		strncpy(buffer, aName, 15);
		buffer[15] = 0;
#if IS_PLATFORM_APPLE
		pthread_setname_np(buffer);
#else
		int rc = pthread_setname_np(pthread_self(), buffer);
		MG_DEV_ASSERT(rc == 0);
#endif

	}

	ThreadId
	GetCurrentThreadId()
	{
		// Formally, it could be possible to always just return
		// the thread ID, and fill it in the thread trampoline
		// before any other functions are called. But there is
		// one thread, not having a trampoline - the main thread.
		// The check is for him.
		if (theThreadID == 0) {
#if IS_PLATFORM_APPLE
			uint64 tid = 0;
			int rc = pthread_threadid_np(pthread_self(), &tid);
			MG_DEV_ASSERT(rc == 0);
#else
			uint64 tid = syscall(SYS_gettid);
#endif
			MG_DEV_ASSERT((uint64)(ThreadId)tid == tid);
			theThreadID = (ThreadId)tid;
		}
		return theThreadID;
	}

	void
	Sleep(
		uint32 aTimeMillis)
	{
		uint32 seconds = aTimeMillis / 1000;
		if (seconds != 0)
			sleep(seconds);
		usleep((aTimeMillis % 1000) * 1000);
	}

}
}
