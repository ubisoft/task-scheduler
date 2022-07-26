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

#include "mg/common/HybridArray.h"
#include "mg/common/MultiProducerQueueIntrusive.h"
#include "mg/common/Random.h"
#include "mg/common/ThreadFunc.h"

#include "UnitTest.h"

namespace mg {
namespace unittests {

	static void
	UnitTestMPQIBasic()
	{
		{
			struct Entry
			{
				int myValue;
				Entry* myNext;
			};

			Entry e1;
			e1.myValue = 1;
			Entry e2;
			e2.myValue = 2;
			Entry e3;
			e3.myValue = 3;

			mg::common::MultiProducerQueueIntrusive<Entry> queue;
			Entry* tail;
			MG_COMMON_ASSERT(queue.IsEmpty());
			MG_COMMON_ASSERT(queue.PopAll(tail) == nullptr && tail == nullptr);
			Entry* garbage = (Entry*) &queue;
			e1.myNext = garbage;
			e2.myNext = garbage;
			e3.myNext = garbage;

			MG_COMMON_ASSERT(queue.Push(&e1));
			MG_COMMON_ASSERT(!queue.IsEmpty());
			Entry* res = queue.PopAll(tail);
			MG_COMMON_ASSERT(queue.IsEmpty());
			MG_COMMON_ASSERT(res == &e1);
			MG_COMMON_ASSERT(res->myNext == nullptr);
			MG_COMMON_ASSERT(tail == &e1);
			MG_COMMON_ASSERT(queue.PopAll(tail) == nullptr && tail == nullptr);
			e1.myNext = garbage;

			MG_COMMON_ASSERT(queue.Push(&e1));
			MG_COMMON_ASSERT(!queue.IsEmpty());
			MG_COMMON_ASSERT(!queue.Push(&e2));
			res = queue.PopAll(tail);
			MG_COMMON_ASSERT(queue.IsEmpty());
			MG_COMMON_ASSERT(res == &e1);
			MG_COMMON_ASSERT(e1.myNext == &e2);
			MG_COMMON_ASSERT(e2.myNext == nullptr);
			MG_COMMON_ASSERT(tail == &e2);
			MG_COMMON_ASSERT(queue.PopAll(tail) == nullptr && tail == nullptr);
			e1.myNext = garbage;
			e2.myNext = garbage;

			MG_COMMON_ASSERT(queue.Push(&e1));
			MG_COMMON_ASSERT(!queue.IsEmpty());
			MG_COMMON_ASSERT(!queue.Push(&e2));
			MG_COMMON_ASSERT(!queue.Push(&e3));
			res = queue.PopAll(tail);
			MG_COMMON_ASSERT(queue.IsEmpty());
			MG_COMMON_ASSERT(res == &e1);
			MG_COMMON_ASSERT(e1.myNext == &e2);
			MG_COMMON_ASSERT(e2.myNext == &e3);
			MG_COMMON_ASSERT(e3.myNext == nullptr);
			MG_COMMON_ASSERT(tail == &e3);
			MG_COMMON_ASSERT(queue.PopAll(tail) == nullptr && tail == nullptr);

			// Push empty reversed.
			MG_COMMON_ASSERT(queue.PushManyFastReversed(nullptr));
			MG_COMMON_ASSERT(queue.IsEmpty());

			// Push one reversed.
			e1.myNext = nullptr;
			MG_COMMON_ASSERT(queue.PushManyFastReversed(&e1));
			MG_COMMON_ASSERT(!queue.IsEmpty());
			res = queue.PopAll();
			MG_COMMON_ASSERT(res == &e1 && res->myNext == nullptr);
			MG_COMMON_ASSERT(queue.IsEmpty());

			// Push 2 reversed.
			e2.myNext = &e1;
			e1.myNext = nullptr;
			MG_COMMON_ASSERT(queue.PushManyFastReversed(&e2));
			MG_COMMON_ASSERT(!queue.IsEmpty());
			res = queue.PopAll();
			MG_COMMON_ASSERT(res == &e1);
			MG_COMMON_ASSERT(e1.myNext == &e2);
			MG_COMMON_ASSERT(e2.myNext == nullptr);

			// Push 3 reversed.
			e3.myNext = &e2;
			e2.myNext = &e1;
			e1.myNext = nullptr;
			MG_COMMON_ASSERT(queue.PushManyFastReversed(&e3));
			MG_COMMON_ASSERT(!queue.IsEmpty());
			res = queue.PopAll();
			MG_COMMON_ASSERT(res == &e1);
			MG_COMMON_ASSERT(e1.myNext == &e2);
			MG_COMMON_ASSERT(e2.myNext == &e3);
			MG_COMMON_ASSERT(e3.myNext == nullptr);

			// Push 4 reversed.
			Entry e4;
			e4.myValue = 4;
			e4.myNext = &e3;
			e3.myNext = &e2;
			e2.myNext = &e1;
			e1.myNext = nullptr;
			MG_COMMON_ASSERT(queue.PushManyFastReversed(&e4));
			MG_COMMON_ASSERT(!queue.IsEmpty());
			res = queue.PopAll();
			MG_COMMON_ASSERT(res == &e1);
			MG_COMMON_ASSERT(e1.myNext == &e2);
			MG_COMMON_ASSERT(e2.myNext == &e3);
			MG_COMMON_ASSERT(e3.myNext == &e4);
			MG_COMMON_ASSERT(e4.myNext == nullptr);

			// Push empty reversed range.
			MG_COMMON_ASSERT(queue.PushManyFastReversed(nullptr, nullptr));
			MG_COMMON_ASSERT(queue.IsEmpty());

			// Push one item in a reversed range.
			e1.myNext = nullptr;
			MG_COMMON_ASSERT(queue.PushManyFastReversed(&e1, &e1));
			MG_COMMON_ASSERT(!queue.IsEmpty());
			res = queue.PopAll();
			MG_COMMON_ASSERT(res == &e1 && res->myNext == nullptr);
			MG_COMMON_ASSERT(queue.IsEmpty());

			// Push 2 in a reversed range.
			e2.myNext = &e1;
			e1.myNext = nullptr;
			MG_COMMON_ASSERT(queue.PushManyFastReversed(&e2, &e1));
			MG_COMMON_ASSERT(!queue.IsEmpty());
			res = queue.PopAll();
			MG_COMMON_ASSERT(res == &e1);
			MG_COMMON_ASSERT(e1.myNext == &e2);
			MG_COMMON_ASSERT(e2.myNext == nullptr);

			// Make reversed range with non empty last next.
			e4.myNext = &e3;
			e3.myNext = &e2;
			e2.myNext = &e1;
			e1.myNext = nullptr;
			// Should cut e4.
			MG_COMMON_ASSERT(queue.PushManyFastReversed(&e4, &e2));
			MG_COMMON_ASSERT(!queue.IsEmpty());
			res = queue.PopAll();
			MG_COMMON_ASSERT(res == &e2);
			MG_COMMON_ASSERT(e2.myNext == &e3);
			MG_COMMON_ASSERT(e3.myNext == &e4);
			MG_COMMON_ASSERT(e4.myNext == nullptr);

			// Push many empty.
			MG_COMMON_ASSERT(queue.PushMany(nullptr));
			MG_COMMON_ASSERT(queue.IsEmpty());

			// Push 1 as many.
			e1.myNext = nullptr;
			MG_COMMON_ASSERT(queue.PushMany(&e1));
			MG_COMMON_ASSERT(!queue.IsEmpty());
			res = queue.PopAll();
			MG_COMMON_ASSERT(res == &e1 && res->myNext == nullptr);
			MG_COMMON_ASSERT(queue.IsEmpty());

			// Push 2 as many.
			e1.myNext = &e2;
			e2.myNext = nullptr;
			MG_COMMON_ASSERT(queue.PushMany(&e1));
			MG_COMMON_ASSERT(!queue.IsEmpty());
			res = queue.PopAll();
			MG_COMMON_ASSERT(res == &e1);
			MG_COMMON_ASSERT(e1.myNext == &e2);
			MG_COMMON_ASSERT(e2.myNext == nullptr);

			// Push 3 as many.
			e1.myNext = &e2;
			e2.myNext = &e3;
			e3.myNext = nullptr;
			MG_COMMON_ASSERT(queue.PushMany(&e1));
			MG_COMMON_ASSERT(!queue.IsEmpty());
			res = queue.PopAll();
			MG_COMMON_ASSERT(res == &e1);
			MG_COMMON_ASSERT(e1.myNext == &e2);
			MG_COMMON_ASSERT(e2.myNext == &e3);
			MG_COMMON_ASSERT(e3.myNext == nullptr);

			// Push 4 as many.
			e1.myNext = &e2;
			e2.myNext = &e3;
			e3.myNext = &e4;
			e4.myNext = nullptr;
			MG_COMMON_ASSERT(queue.PushMany(&e1));
			MG_COMMON_ASSERT(!queue.IsEmpty());
			res = queue.PopAll();
			MG_COMMON_ASSERT(res == &e1);
			MG_COMMON_ASSERT(e1.myNext == &e2);
			MG_COMMON_ASSERT(e2.myNext == &e3);
			MG_COMMON_ASSERT(e3.myNext == &e4);
			MG_COMMON_ASSERT(e4.myNext == nullptr);
		}
		{
			// Try non-default link name.
			struct Entry
			{
				int myValue;
				Entry* myNextInQueue;
			};

			Entry e1;
			e1.myValue = 1;
			Entry e2;
			e2.myValue = 2;

			mg::common::MultiProducerQueueIntrusive<Entry, &Entry::myNextInQueue> queue;
			Entry* tail;
			MG_COMMON_ASSERT(queue.IsEmpty());
			MG_COMMON_ASSERT(queue.PopAll(tail) == nullptr && tail == nullptr);
			Entry* garbage = (Entry*) &queue;
			e1.myNextInQueue = garbage;
			e2.myNextInQueue = garbage;

			MG_COMMON_ASSERT(queue.Push(&e1));
			MG_COMMON_ASSERT(!queue.IsEmpty());
			MG_COMMON_ASSERT(!queue.Push(&e2));
			Entry* res = queue.PopAll(tail);
			MG_COMMON_ASSERT(queue.IsEmpty());
			MG_COMMON_ASSERT(res == &e1);
			MG_COMMON_ASSERT(e1.myNextInQueue == &e2);
			MG_COMMON_ASSERT(e2.myNextInQueue == nullptr);
			MG_COMMON_ASSERT(tail == &e2);
			MG_COMMON_ASSERT(queue.PopAll(tail) == nullptr && tail == nullptr);

			e1.myNextInQueue = &e2;
			e2.myNextInQueue = nullptr;
			MG_COMMON_ASSERT(queue.PushMany(&e1));
			MG_COMMON_ASSERT(!queue.IsEmpty());
			res = queue.PopAll(tail);
			MG_COMMON_ASSERT(queue.IsEmpty());
			MG_COMMON_ASSERT(res == &e1);
			MG_COMMON_ASSERT(e1.myNextInQueue == &e2);
			MG_COMMON_ASSERT(e2.myNextInQueue == nullptr);
		}
	}

	static void
	UnitTestMPQIStress()
	{
		struct Entry
		{
			Entry(
				uint32 aThreadId,
				uint32 aId)
				: myThreadId(aThreadId)
				, myId(aId)
				, myNext(nullptr)
			{
			}

			uint32 myThreadId;
			uint32 myId;
			Entry* myNext;
		};

		const uint32 itemCount = 100000;
		const uint32 threadCount = 10;

		mg::common::MultiProducerQueueIntrusive<Entry> queue;
		mg::common::Array<Entry*> data;
		data.Reserve(threadCount * itemCount);

		mg::common::HybridArray<mg::common::ThreadFunc*, threadCount> threads;
		int32 readyCount = 0;
		for (uint32 ti = 0; ti < threadCount; ++ti)
		{
			threads.Add(new mg::common::ThreadFunc([&]() {

				const uint32 packMaxSize = 5;

				uint32 threadId = mg::common::AtomicIncrement(&readyCount) - 1;
				while ((uint32)mg::common::AtomicLoad(&readyCount) != threadCount)
					mg::common::Sleep(1);

				uint32 i = 0;
				while (i < itemCount)
				{
					uint32 packSize = mg::common::RandomUInt32() % packMaxSize + 1;
					if (i + packSize > itemCount)
						packSize = itemCount - i;

					Entry* head = new Entry(threadId, i++);
					Entry* pos = head;
					for (uint32 j = 1; j < packSize; ++j)
					{
						pos->myNext = new Entry(threadId, i++);
						pos = pos->myNext;
					}
					if (packSize == 1)
						queue.Push(head);
					else
						queue.PushMany(head);
				}
			}));
			threads.Last()->Start();
		}

		bool done = false;
		uint64 yield = 0;
		while (!done)
		{
			done = true;
			for (mg::common::ThreadFunc* f : threads)
				done &= !f->IsRunning();
			Entry* head = queue.PopAll();
			while (head != nullptr)
			{
				data.Add(head);
				head = head->myNext;
			}
			if (++yield % 1000 == 0)
				mg::common::Sleep(1);
		}
		MG_COMMON_ASSERT(data.Count() == itemCount * threadCount);

		threads.DeleteAll();
		mg::common::HybridArray<uint32, threadCount> counters;
		for (uint32 i = 0; i < threadCount; ++i)
			counters.Add(0);
		for (Entry* e : data)
			MG_COMMON_ASSERT(e->myId == counters[e->myThreadId]++);
		for (uint32 count : counters)
			MG_COMMON_ASSERT(count == itemCount);
	}

	void
	UnitTestMultiProducerQueue()
	{
		TestSuiteGuard suite("MultiProducerQueue");

		UnitTestMPQIBasic();
		UnitTestMPQIStress();
	}

}
}
