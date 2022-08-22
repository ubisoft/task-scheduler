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

#include "mg/common/ForwardList.h"
#include "mg/common/Mutex.h"

namespace mg {
namespace bench {

	struct BenchValue
	{
		BenchValue();

		BenchValue* myNext;
	};

	// Trivial queue takes a mutex lock on each operation. The simplest possible
	// implementation and the most typical one. And most of the time it is enough.
	//
	class MultiProducerQueue
	{
	public:
		bool Push(
			BenchValue* aValue);

		BenchValue* PopAll(
			BenchValue*& aOutTail);

	private:
		mg::common::Mutex myLock;
		mg::common::ForwardList<BenchValue> myValues;
	};

	inline
	BenchValue::BenchValue()
		: myNext(nullptr)
	{
	}

	inline bool
	MultiProducerQueue::Push(
		BenchValue* aValue)
	{
		myLock.Lock();
		bool res = myValues.IsEmpty();
		myValues.Append(aValue);
		myLock.Unlock();
		return res;
	}

	inline BenchValue*
	MultiProducerQueue::PopAll(
		BenchValue*& aOutTail)
	{
		myLock.Lock();
		BenchValue* head = myValues.PopAll(aOutTail);
		myLock.Unlock();
		return head;
	}

}
}

#include "BenchMultiProducerQueueTemplate.hpp"
