#include "mg/common/Assert.h"
#include "mg/common/ForwardList.h"

#include "UnitTest.h"

namespace mg {
namespace unittests {

	struct UTFLValue
	{
		UTFLValue()
			: myValue(-1)
			// Garbage pointer to ensure it is nullified when the
			// element is added to a list.
			, myNext((UTFLValue*)0x123)
		{
		}

		UTFLValue(
			int aValue)
			: myValue(aValue)
			, myNext((UTFLValue*)0x123)
		{
		}

		int myValue;
		UTFLValue* myNext;
	};

	using UTFLList = mg::common::ForwardList<UTFLValue>;

	static void
	UnitTestForwardListBasic()
	{
		// Empty list.
		{
			UTFLList list;
			const UTFLList* clist = &list;
			MG_COMMON_ASSERT(list.GetFirst() == nullptr);
			MG_COMMON_ASSERT(clist->GetFirst() == nullptr);
			MG_COMMON_ASSERT(list.GetLast() == nullptr);
			MG_COMMON_ASSERT(clist->GetLast() == nullptr);
			MG_COMMON_ASSERT(list.IsEmpty());
			list.Clear();
			list.Reverse();
		}
		// Has 1 element.
		{
			UTFLList list;
			const UTFLList* clist = &list;
			UTFLValue v(1);

			list.Append(&v);
			MG_COMMON_ASSERT(v.myNext == nullptr);
			MG_COMMON_ASSERT(list.GetFirst() == &v);
			MG_COMMON_ASSERT(clist->GetFirst() == &v);
			MG_COMMON_ASSERT(list.GetLast() == &v);
			MG_COMMON_ASSERT(clist->GetLast() == &v);
			MG_COMMON_ASSERT(!list.IsEmpty());

			list.Reverse();
			MG_COMMON_ASSERT(list.GetFirst() == &v);

			list.Clear();
			MG_COMMON_ASSERT(list.IsEmpty());
			MG_COMMON_ASSERT(list.GetFirst() == nullptr);

			list.Prepend(&v);
			MG_COMMON_ASSERT(list.GetFirst() == &v);
			MG_COMMON_ASSERT(list.PopFirst() == &v);
		}
		// Many elements.
		{
			UTFLList list;
			const UTFLList* clist = &list;
			UTFLValue v1(1);
			UTFLValue v2(2);
			// Fill with garbage to check the links are nullified.
			v1.myNext = (UTFLValue*) 0x123;
			v2.myNext = v1.myNext;

			list.Append(&v1);
			MG_COMMON_ASSERT(v1.myNext == nullptr);
			list.Append(&v2);

			MG_COMMON_ASSERT(list.GetFirst() == &v1);
			MG_COMMON_ASSERT(clist->GetFirst() == &v1);
			MG_COMMON_ASSERT(list.GetLast() == &v2);
			MG_COMMON_ASSERT(clist->GetLast() == &v2);
			MG_COMMON_ASSERT(!list.IsEmpty());
			MG_COMMON_ASSERT(v1.myNext == &v2);
			MG_COMMON_ASSERT(v2.myNext == nullptr);

			UTFLValue v0(0);
			list.Prepend(&v0);
			MG_COMMON_ASSERT(list.GetFirst() == &v0);
			MG_COMMON_ASSERT(list.GetLast() == &v2);
			MG_COMMON_ASSERT(v0.myNext == &v1);
			MG_COMMON_ASSERT(v1.myNext == &v2);
			MG_COMMON_ASSERT(v2.myNext == nullptr);

			list.Reverse();
			MG_COMMON_ASSERT(list.GetFirst() == &v2);
			MG_COMMON_ASSERT(list.GetLast() == &v0);
			MG_COMMON_ASSERT(v2.myNext == &v1);
			MG_COMMON_ASSERT(v1.myNext == &v0);
			MG_COMMON_ASSERT(v0.myNext == nullptr);

			list.Clear();
			MG_COMMON_ASSERT(list.IsEmpty());
		}
		// Construct from a plain empty list.
		{
			UTFLList list(nullptr, nullptr);
			UTFLValue v1(1);
			MG_COMMON_ASSERT(list.IsEmpty());
			list.Append(&v1);
			MG_COMMON_ASSERT(list.GetFirst() == &v1);
			MG_COMMON_ASSERT(list.GetLast() == &v1);
		}
		// Construct from a plain one-element list.
		{
			UTFLValue v1(1);
			v1.myNext = &v1;
			UTFLList list(&v1, &v1);
			MG_COMMON_ASSERT(v1.myNext == nullptr);
			MG_COMMON_ASSERT(!list.IsEmpty());
			MG_COMMON_ASSERT(list.GetFirst() == &v1);
			MG_COMMON_ASSERT(list.GetLast() == &v1);
		}
		// Construct from a plain two-element list.
		{
			UTFLValue v1(1);
			UTFLValue v2(2);
			v1.myNext = &v2;
			UTFLList list(&v1, &v2);
			MG_COMMON_ASSERT(v1.myNext == &v2);
			MG_COMMON_ASSERT(v2.myNext == nullptr);
			MG_COMMON_ASSERT(!list.IsEmpty());
			MG_COMMON_ASSERT(list.GetFirst() == &v1);
			MG_COMMON_ASSERT(list.GetLast() == &v2);
		}
		// Construct from a list object.
		{
			UTFLValue v1(1);
			UTFLValue v2(2);
			v1.myNext = &v2;
			UTFLList list1(&v1, &v2);
			UTFLList list2(mg::common::Move(list1));

			MG_COMMON_ASSERT(list1.IsEmpty());
			MG_COMMON_ASSERT(v1.myNext == &v2);
			MG_COMMON_ASSERT(v2.myNext == nullptr);
			MG_COMMON_ASSERT(!list2.IsEmpty());
			MG_COMMON_ASSERT(list2.GetFirst() == &v1);
			MG_COMMON_ASSERT(list2.GetLast() == &v2);
		}
		// Pop with tail.
		{
			UTFLList list;
			UTFLValue v1(1);
			UTFLValue v2(2);
			UTFLValue v3(3);
			list.Append(&v1);
			list.Append(&v2);
			list.Append(&v3);
			UTFLValue* tail = &v1;
			UTFLValue* head = list.PopAll(tail);
			MG_COMMON_ASSERT(list.IsEmpty());
			MG_COMMON_ASSERT(tail == &v3);
			MG_COMMON_ASSERT(head == &v1);
			MG_COMMON_ASSERT(list.GetFirst() == nullptr);
			MG_COMMON_ASSERT(list.GetLast() == nullptr);
		}
	}

	static void
	UnitTestForwardListAppend()
	{
		// Append plain list.
		{
			UTFLList list;
			UTFLValue v1(1);
			UTFLValue v2(2);
			v1.myNext = &v2;

			list.Append(&v1, &v2);
			MG_COMMON_ASSERT(list.GetFirst() == &v1);
			MG_COMMON_ASSERT(list.GetLast() == &v2);
			MG_COMMON_ASSERT(v1.myNext == &v2);

			list.Clear();
			list.Append(&v1, &v1);
			MG_COMMON_ASSERT(list.GetFirst() == &v1);
			MG_COMMON_ASSERT(list.GetLast() == &v1);
			MG_COMMON_ASSERT(v1.myNext == nullptr);

			list.Clear();
			UTFLValue v3(3);

			v1.myNext = &v2;
			list.Append(&v1, &v2);
			list.Append(&v3, &v3);
			MG_COMMON_ASSERT(list.GetFirst() == &v1);
			MG_COMMON_ASSERT(list.GetLast() == &v3);
			MG_COMMON_ASSERT(v1.myNext == &v2);
			MG_COMMON_ASSERT(v2.myNext == &v3);
			MG_COMMON_ASSERT(v3.myNext == nullptr);

			list.Clear();
			UTFLValue v4(4);
			v1.myNext = &v2;
			v3.myNext = &v4;
			// Set to a bad value to see that it is nullified.
			v4.myNext = &v1;
			list.Append(&v1, &v2);
			list.Append(&v3, &v4);
			MG_COMMON_ASSERT(list.GetFirst() == &v1);
			MG_COMMON_ASSERT(list.GetLast() == &v4);
			MG_COMMON_ASSERT(v1.myNext == &v2);
			MG_COMMON_ASSERT(v2.myNext == &v3);
			MG_COMMON_ASSERT(v3.myNext == &v4);
			MG_COMMON_ASSERT(v4.myNext == nullptr);

			list.Append(nullptr, nullptr);
			MG_COMMON_ASSERT(list.GetFirst() == &v1);
			MG_COMMON_ASSERT(list.GetLast() == &v4);
		}
		// Append a list object.
		{
			UTFLList list1;
			UTFLList list2;
			UTFLValue v1(1);

			list1.Append(mg::common::Move(list2));
			MG_COMMON_ASSERT(list1.IsEmpty());
			MG_COMMON_ASSERT(list2.IsEmpty());

			list2.Append(&v1);
			list1.Append(mg::common::Move(list2));
			MG_COMMON_ASSERT(list1.GetFirst() == &v1);
			MG_COMMON_ASSERT(list1.GetLast() == &v1);
			MG_COMMON_ASSERT(list2.IsEmpty());

			UTFLValue v2(2);
			UTFLValue v3(2);
			list2.Append(&v2);
			list2.Append(&v3);
			list1.Append(mg::common::Move(list2));
			MG_COMMON_ASSERT(list1.GetFirst() == &v1);
			MG_COMMON_ASSERT(list1.GetLast() == &v3);
			MG_COMMON_ASSERT(list2.IsEmpty());
		}
	}

	static void
	UnitTestForwardListPrepend()
	{
		// Prepend multiple.
		{
			UTFLList list;
			UTFLValue v1(1);
			UTFLValue v2(2);

			list.Prepend(&v2);
			list.Prepend(&v1);
			MG_COMMON_ASSERT(list.GetFirst() == &v1);
			MG_COMMON_ASSERT(list.GetLast() == &v2);
			MG_COMMON_ASSERT(v1.myNext == &v2);
			MG_COMMON_ASSERT(v2.myNext == nullptr);
		}
		// Prepend a plain list.
		{
			UTFLList list;
			UTFLValue v1(1);
			UTFLValue v2(2);

			list.Prepend(nullptr, nullptr);
			MG_COMMON_ASSERT(list.IsEmpty());
			list.Append(&v1);
			MG_COMMON_ASSERT(list.PopFirst() == &v1);

			v1.myNext = &v1;
			list.Prepend(&v1, &v1);
			MG_COMMON_ASSERT(!list.IsEmpty());
			MG_COMMON_ASSERT(v1.myNext == nullptr);
			MG_COMMON_ASSERT(list.PopFirst() == &v1);
			MG_COMMON_ASSERT(list.IsEmpty());

			v1.myNext = &v2;
			list.Prepend(&v1, &v2);
			MG_COMMON_ASSERT(list.GetFirst() == &v1);
			MG_COMMON_ASSERT(list.GetLast() == &v2);
			MG_COMMON_ASSERT(list.PopFirst() == &v1);
			MG_COMMON_ASSERT(list.PopFirst() == &v2);
			MG_COMMON_ASSERT(list.GetFirst() == nullptr);
			MG_COMMON_ASSERT(list.GetLast() == nullptr);
		}
		// Prepend a list object.
		{
			UTFLList list1;
			UTFLList list2;

			list1.Prepend(mg::common::Move(list2));
			MG_COMMON_ASSERT(list1.IsEmpty());
			MG_COMMON_ASSERT(list2.IsEmpty());

			UTFLValue v1(1);
			list2.Append(&v1);
			list1.Prepend(mg::common::Move(list2));
			MG_COMMON_ASSERT(list1.GetFirst() == &v1);
			MG_COMMON_ASSERT(list1.GetLast() == &v1);
			MG_COMMON_ASSERT(list2.IsEmpty());

			UTFLValue v2(2);
			UTFLValue v3(3);
			list2.Append(&v2);
			list2.Append(&v3);
			list1.Prepend(mg::common::Move(list2));
			MG_COMMON_ASSERT(list2.IsEmpty());
			MG_COMMON_ASSERT(list1.GetFirst() == &v2);
			MG_COMMON_ASSERT(list1.GetLast() == &v1);
			MG_COMMON_ASSERT(v2.myNext == &v3);
			MG_COMMON_ASSERT(v3.myNext == &v1);
			MG_COMMON_ASSERT(v1.myNext == nullptr);
		}
	}

	static void
	UnitTestForwardListInsert()
	{
		// Insert into empty.
		{
			UTFLList list;
			UTFLValue v1(1);

			list.Insert(nullptr, &v1);
			MG_COMMON_ASSERT(list.GetFirst() == &v1);
			MG_COMMON_ASSERT(list.GetLast() == &v1);
			MG_COMMON_ASSERT(v1.myNext == nullptr);
		}
		// Insert before first.
		{
			UTFLList list;
			UTFLValue v1(1);
			UTFLValue v2(2);
			list.Insert(nullptr, &v2);
			list.Insert(nullptr, &v1);
			MG_COMMON_ASSERT(list.GetFirst() == &v1);
			MG_COMMON_ASSERT(list.GetLast() == &v2);
			MG_COMMON_ASSERT(v2.myNext == nullptr);
		}
		// Insert in the middle first.
		{
			UTFLList list;
			UTFLValue v1(1);
			UTFLValue v2(2);
			UTFLValue v3(3);
			list.Append(&v1);
			list.Append(&v3);
			list.Insert(&v1, &v2);
			MG_COMMON_ASSERT(list.GetFirst() == &v1);
			MG_COMMON_ASSERT(list.GetLast() == &v3);
			MG_COMMON_ASSERT(v1.myNext == &v2);
			MG_COMMON_ASSERT(v2.myNext == &v3);
			MG_COMMON_ASSERT(v3.myNext == nullptr);
		}
		// Insert in the end.
		{
			UTFLList list;
			UTFLValue v1(1);
			UTFLValue v2(2);
			UTFLValue v3(3);
			list.Append(&v1);
			list.Append(&v2);
			list.Insert(&v2, &v3);
			MG_COMMON_ASSERT(list.GetFirst() == &v1);
			MG_COMMON_ASSERT(list.GetLast() == &v3);
			MG_COMMON_ASSERT(v1.myNext == &v2);
			MG_COMMON_ASSERT(v2.myNext == &v3);
			MG_COMMON_ASSERT(v3.myNext == nullptr);
		}
	}

	static void
	UnitTestForwardListAssign()
	{
		// Assign from a list object.
		{
			UTFLList list1;
			UTFLList list2;
			UTFLValue v1(1);

			list1 = mg::common::Move(list2);
			MG_COMMON_ASSERT(list1.IsEmpty());
			MG_COMMON_ASSERT(list2.IsEmpty());

			list1.Append(&v1);
			list1 = mg::common::Move(list2);
			MG_COMMON_ASSERT(list1.IsEmpty());
			MG_COMMON_ASSERT(list2.IsEmpty());

			UTFLValue v2(2);
			list1.Append(&v1);
			list2.Append(&v2);
			list1 = mg::common::Move(list2);
			MG_COMMON_ASSERT(list1.GetFirst() == &v2);
			MG_COMMON_ASSERT(list1.GetLast() == &v2);
			MG_COMMON_ASSERT(list2.IsEmpty());

			UTFLValue v3(3);
			list1.Clear();
			list1.Append(&v1);
			list2.Append(&v2);
			list2.Append(&v3);
			list1 = mg::common::Move(list2);
			MG_COMMON_ASSERT(list2.IsEmpty());
			MG_COMMON_ASSERT(list1.GetFirst() == &v2);
			MG_COMMON_ASSERT(list1.GetLast() == &v3);
			MG_COMMON_ASSERT(v2.myNext == &v3);
			MG_COMMON_ASSERT(v3.myNext == nullptr);

			list1.Clear();
			list1.Append(&v1);
			list1.Append(&v2);
			list2.Append(&v3);
			list1 = mg::common::Move(list2);
			MG_COMMON_ASSERT(list2.IsEmpty());
			MG_COMMON_ASSERT(list1.GetFirst() == &v3);
			MG_COMMON_ASSERT(list1.GetLast() == &v3);
			MG_COMMON_ASSERT(v3.myNext == nullptr);
		}
	}

	struct UTFLValue2
	{
		UTFLValue2()
			: myValue(-1)
			, myNext2((UTFLValue2*)0x123)
		{
		}

		UTFLValue2(
			int aValue)
			: myValue(aValue)
			, myNext2((UTFLValue2*)0x123)
		{
		}

		int myValue;
		UTFLValue2* myNext2;
	};

	using UTFLList2 = mg::common::ForwardList<UTFLValue2, &UTFLValue2::myNext2>;

	static void
	UnitTestForwardListDifferentLink()
	{
		UTFLList2 list2;
		UTFLValue2 v1(1);
		UTFLValue2 v2(2);
		list2.Append(&v1);
		list2.Append(&v2);
		MG_COMMON_ASSERT(list2.GetFirst() == &v1);
		MG_COMMON_ASSERT(list2.GetLast() == &v2);
		MG_COMMON_ASSERT(v1.myNext2 == &v2);
		MG_COMMON_ASSERT(v2.myNext2 == nullptr);
	}

	static void
	UnitTestForwardListIterator()
	{
		UTFLList list;
		for (UTFLValue* val : list)
		{
			MG_UNUSED(val);
			MG_COMMON_ASSERT(false);
		}

		UTFLValue v1(1);
		list.Append(&v1);
		int count = 0;
		for (UTFLValue* val : list)
		{
			++count;
			MG_COMMON_ASSERT(val->myValue == count);
		}
		MG_COMMON_ASSERT(count == 1);

		count = 0;
		for (UTFLValue* val : list)
		{
			MG_COMMON_ASSERT(val == list.PopFirst());
			++count;
			MG_COMMON_ASSERT(val->myValue == count);
		}
		MG_COMMON_ASSERT(count == 1);

		UTFLValue v2(2);
		list.Append(&v1);
		list.Append(&v2);
		count = 0;
		for (UTFLValue* val : list)
		{
			++count;
			MG_COMMON_ASSERT(val->myValue == count);
		}
		MG_COMMON_ASSERT(count == 2);

		count = 0;
		for (UTFLValue* val : list)
		{
			MG_COMMON_ASSERT(val == list.PopFirst());
			++count;
			MG_COMMON_ASSERT(val->myValue == count);
		}
		MG_COMMON_ASSERT(count == 2);

		UTFLValue v3(3);
		list.Append(&v1);
		list.Append(&v2);
		list.Append(&v3);
		count = 0;
		for (UTFLValue* val : list)
		{
			++count;
			MG_COMMON_ASSERT(val->myValue == count);
		}
		MG_COMMON_ASSERT(count == 3);

		count = 0;
		for (UTFLValue* val : list)
		{
			MG_COMMON_ASSERT(val == list.PopFirst());
			++count;
			MG_COMMON_ASSERT(val->myValue == count);
		}
		MG_COMMON_ASSERT(count == 3);
	}

	static void
	UnitTestForwardListConstIterator()
	{
		UTFLList list;
		const UTFLList& clist = list;
		for (const UTFLValue* val : clist)
		{
			MG_UNUSED(val);
			MG_COMMON_ASSERT(false);
		}

		UTFLValue v1(1);
		list.Append(&v1);
		int count = 0;
		for (const UTFLValue* val : clist)
		{
			++count;
			MG_COMMON_ASSERT(val->myValue == count);
		}
		MG_COMMON_ASSERT(count == 1);

		UTFLValue v2(2);
		list.Append(&v1);
		list.Append(&v2);
		count = 0;
		for (const UTFLValue* val : clist)
		{
			++count;
			MG_COMMON_ASSERT(val->myValue == count);
		}
		MG_COMMON_ASSERT(count == 2);

		UTFLValue v3(3);
		list.Append(&v1);
		list.Append(&v2);
		list.Append(&v3);
		count = 0;
		for (const UTFLValue* val : clist)
		{
			++count;
			MG_COMMON_ASSERT(val->myValue == count);
		}
		MG_COMMON_ASSERT(count == 3);
	}

	void
	UnitTestForwardList()
	{
		TestSuiteGuard suite("ForwardList");

		UnitTestForwardListBasic();
		UnitTestForwardListAppend();
		UnitTestForwardListPrepend();
		UnitTestForwardListInsert();
		UnitTestForwardListDifferentLink();
		UnitTestForwardListAssign();
		UnitTestForwardListIterator();
		UnitTestForwardListConstIterator();
	}

}
}
