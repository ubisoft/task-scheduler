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

#include "Bench.h"

#include "mg/common/ConditionVariable.h"
#include "mg/common/Mutex.h"

namespace mg {
namespace bench {

	class Signal
	{
	public:
		Signal();

		~Signal();

		bool Send();

		void ReceiveBlocking();

	private:
		mg::common::Mutex myLock;
		mg::common::ConditionVariable myCond;
		bool myValue;
	};

	Signal::Signal()
		: myValue(false)
	{
	}

	Signal::~Signal()
	{
		// Repeat the original signal implementation.
		myLock.Lock();
		myLock.Unlock();
	}

	bool
	Signal::Send()
	{
		myLock.Lock();
		bool rc = !myValue;
		if (rc)
		{
			myValue = true;
			myCond.Signal();
		}
		myLock.Unlock();
		return rc;
	}

	void
	Signal::ReceiveBlocking()
	{
		myLock.Lock();
		while (!myValue)
			myCond.Wait(myLock);
		myValue = false;
		myLock.Unlock();
	}

}
}

#include "BenchSignalTemplate.hpp"
