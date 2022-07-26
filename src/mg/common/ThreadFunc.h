// Copyright (C) 2022 Ubisoft Entertainment
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 2 of
// the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this program.  If not, see
// <https://www.gnu.org/licenses/>.

#pragma once

#include "mg/common/Callback.h"
#include "mg/common/Thread.h"

namespace mg {
namespace common {

	using ThreadCallback = mg::common::Callback<void()>;

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
