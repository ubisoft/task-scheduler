#pragma once

#include "mg/common/ConditionVariable.h"
#include "mg/common/Mutex.h"
#include "mg/common/Types.h"

#include <atomic>
#include <string>
#include <thread>

#define MG_THREADLOCAL thread_local

namespace mg {
namespace common {

	using ThreadId = uint32;

	class Thread
	{
	public:
		// Thread name should be a short string up to 15 bytes not
		// including terminating 0 (15 printable symbols). The
		// reason is that on Linux thread names are truncated to
		// 16 bytes.
		Thread(
			const char*	aName = "anon_thread");

		void Start();

		void Stop();

		void BlockingStop();

		void StopAndDelete();

		bool StopRequested();

		bool IsRunning();

	protected:
		virtual ~Thread();

	private:
		void PrivTrampoline();

		virtual void Run() = 0;

		Mutex myLock;
		ConditionVariable myCond;
		std::thread* myHandle;
		std::string myName;
		std::atomic<bool> myIsStopRequested;
		std::atomic<bool> myIsRunning;
		bool myWasStarted;
	};

	void Sleep(
		uint32 aTimeMillis);

	ThreadId GetCurrentThreadId();

}
}
