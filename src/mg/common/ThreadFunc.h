#pragma once

#include "mg/common/Thread.h"

#include <functional>

namespace mg {
namespace common {

	using ThreadCallback = std::function<void()>;

	// Wrap a callback into a thread. It is essentially
	// syntax sugar which allows not to inherit Thread class to
	// make something trivial. Similar to std::thread.
	class ThreadFunc
		: public Thread
	{
	public:
		ThreadFunc(
			const ThreadCallback& aCallback);

		~ThreadFunc();

	private:
		void Run() override;

		ThreadCallback myCallback;
	};

	//////////////////////////////////////////////////////////////

	inline
	ThreadFunc::ThreadFunc(
		const ThreadCallback& aCallback)
		: myCallback(aCallback)
	{
	}

	inline
	ThreadFunc::~ThreadFunc()
	{
		BlockingStop();
	}

	inline void
	ThreadFunc::Run()
	{
		myCallback();
	}

}
}
