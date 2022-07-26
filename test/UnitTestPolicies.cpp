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

#include "mg/common/Atomic.h"
#include "mg/common/Policies.h"

#include "UnitTest.h"

#if IS_COMPILER_GCC && IS_BUILD_RELEASE
// In Release build GCC optimizations give some false-positive
// warnings.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
#endif

namespace mg {
namespace unittests {

	static void
	UnitTestPoliciesCopy()
	{
		{
			// 1 and 0 are often border cases. Check them
			// specifically.
			int a = 0;
			int b = 1;
			mg::common::ItemsCopy(&b, &a, 1);
			MG_COMMON_ASSERT(b == 0);
			MG_COMMON_ASSERT(a == 0);

			b = 1;
			mg::common::ItemsCopy(&b, &a, 0);
			MG_COMMON_ASSERT(a == 0);
			MG_COMMON_ASSERT(b == 1);

			mg::common::ItemsCopyConstruct(&b, &a, 1);
			MG_COMMON_ASSERT(b == 0);
			MG_COMMON_ASSERT(a == 0);

			b = 1;
			mg::common::ItemsCopyConstruct(&b, &a, 0);
			MG_COMMON_ASSERT(a == 0);
			MG_COMMON_ASSERT(b == 1);

			// Optimized copy for 1 element copy without loops
			// inside.
			mg::common::ItemsCopy(&b, &a);
			MG_COMMON_ASSERT(b == 0);
			MG_COMMON_ASSERT(a == 0);

			b = 1;
			mg::common::ItemsCopyConstruct(&b, &a);
			MG_COMMON_ASSERT(b == 0);
			MG_COMMON_ASSERT(a == 0);
		}
		{
			// Copy many. For trivial types is just memcpy.
			int a[3] = {0, 1, 2};
			int b[3] = {3, 4, 5};
			mg::common::ItemsCopy(b, a, 3);
			for (int i = 0; i <= 2; ++i)
			{
				MG_COMMON_ASSERT(a[i] == i);
				MG_COMMON_ASSERT(b[i] == a[i]);
			}

			// Copy-construct many. For trivial types is just
			// memcpy.
			b[0] = 3;
			b[1] = 4;
			b[2] = 5;
			mg::common::ItemsCopyConstruct(b, a, 3);
			for (int i = 0; i <= 2; ++i)
			{
				MG_COMMON_ASSERT(a[i] == i);
				MG_COMMON_ASSERT(b[i] == a[i]);
			}
		}
		// Trivial type copy. No constructors or assignment
		// operators defined, so works as memcpy.
		{
			struct T
			{
				int myValue;
			};
			T t1;
			t1.myValue = 0;
			T t2;
			t2.myValue = 1;
			// 1 and 0 are often border cases. Check them
			// specifically.
			mg::common::ItemsCopy(&t2, &t1, 1);
			MG_COMMON_ASSERT(t2.myValue == 0);
			MG_COMMON_ASSERT(t1.myValue == 0);

			t2.myValue = 1;
			mg::common::ItemsCopy(&t2, &t1, 0);
			MG_COMMON_ASSERT(t2.myValue == 1);
			MG_COMMON_ASSERT(t1.myValue == 0);

			mg::common::ItemsCopyConstruct(&t2, &t1, 1);
			MG_COMMON_ASSERT(t2.myValue == 0);
			MG_COMMON_ASSERT(t1.myValue == 0);

			t2.myValue = 1;
			mg::common::ItemsCopyConstruct(&t2, &t1, 0);
			MG_COMMON_ASSERT(t2.myValue == 1);
			MG_COMMON_ASSERT(t1.myValue == 0);

			// Optimized copy for 1 element copy without loops
			// inside.
			mg::common::ItemsCopy(&t2, &t1);
			MG_COMMON_ASSERT(t2.myValue == 0);
			MG_COMMON_ASSERT(t1.myValue == 0);

			t2.myValue = 1;
			mg::common::ItemsCopyConstruct(&t2, &t1);
			MG_COMMON_ASSERT(t2.myValue == 0);
			MG_COMMON_ASSERT(t1.myValue == 0);

			T t11[2];
			T t22[2];
			t11[0].myValue = 0;
			t11[1].myValue = 1;
			t22[0].myValue = 2;
			t22[1].myValue = 3;
			mg::common::ItemsCopy(t22, t11, 2);
			MG_COMMON_ASSERT(t11[0].myValue == 0);
			MG_COMMON_ASSERT(t11[1].myValue == 1);
			MG_COMMON_ASSERT(t22[0].myValue == 0);
			MG_COMMON_ASSERT(t22[1].myValue == 1);

			t11[0].myValue = 0;
			t11[1].myValue = 1;
			t22[0].myValue = 2;
			t22[1].myValue = 3;
			mg::common::ItemsCopyConstruct(t22, t11, 2);
			MG_COMMON_ASSERT(t11[0].myValue == 0);
			MG_COMMON_ASSERT(t11[1].myValue == 1);
			MG_COMMON_ASSERT(t22[0].myValue == 0);
			MG_COMMON_ASSERT(t22[1].myValue == 1);
		}
		// When has copy-constructor, copy-construction uses it.
		// But plain in-place assignment works as memcpy. Because
		// the assignment operator is default.
		{
			struct T
			{
				T() = default;

				T(
					const T& aObj)
				{
					myValue = aObj.myValue + 1;
				}

				int myValue;
			};
			T t1;
			t1.myValue = 0;
			T t2;
			t2.myValue = 1;
			// 1 and 0 are often border cases. Check them
			// specifically.
			mg::common::ItemsCopy(&t2, &t1, 1);
			MG_COMMON_ASSERT(t2.myValue == 0);
			MG_COMMON_ASSERT(t1.myValue == 0);

			t2.myValue = 1;
			mg::common::ItemsCopy(&t2, &t1, 0);
			MG_COMMON_ASSERT(t2.myValue == 1);
			MG_COMMON_ASSERT(t1.myValue == 0);

			// Note, copy-construct uses the constructor, not
			// memcpy, so the destination values are different.
			t2.myValue = -1;
			mg::common::ItemsCopyConstruct(&t2, &t1, 1);
			MG_COMMON_ASSERT(t2.myValue == 1);
			MG_COMMON_ASSERT(t1.myValue == 0);

			t2.myValue = -1;
			mg::common::ItemsCopyConstruct(&t2, &t1, 0);
			MG_COMMON_ASSERT(t2.myValue == -1);
			MG_COMMON_ASSERT(t1.myValue == 0);

			// Optimized copy for 1 element copy without loops
			// inside.
			mg::common::ItemsCopy(&t2, &t1);
			MG_COMMON_ASSERT(t2.myValue == 0);
			MG_COMMON_ASSERT(t1.myValue == 0);

			t2.myValue = -1;
			mg::common::ItemsCopyConstruct(&t2, &t1);
			MG_COMMON_ASSERT(t2.myValue == 1);
			MG_COMMON_ASSERT(t1.myValue == 0);
		}
		// When copy-constructor is not defined, it is default,
		// which is memcpy. But the assignment operator is not
		// default. Should be used for copying.
		{
			struct T
			{
				T() = default;

				T&
				operator=(
					const T& aObj)
				{
					myValue = aObj.myValue + 1;
					return *this;
				}

				int myValue;
			};
			T t1;
			t1.myValue = 2;
			T t2;
			t2.myValue = 0;
			// 1 and 0 are often border cases. Check them
			// specifically.
			mg::common::ItemsCopy(&t2, &t1, 1);
			MG_COMMON_ASSERT(t2.myValue == 3);
			MG_COMMON_ASSERT(t1.myValue == 2);

			t2.myValue = 0;
			mg::common::ItemsCopy(&t2, &t1, 0);
			MG_COMMON_ASSERT(t2.myValue == 0);
			MG_COMMON_ASSERT(t1.myValue == 2);

			// Note, copy-construct is just memcpy. It uses the
			// constructor, not the assignment.
			mg::common::ItemsCopyConstruct(&t2, &t1, 1);
			MG_COMMON_ASSERT(t2.myValue == 2);
			MG_COMMON_ASSERT(t1.myValue == 2);

			t2.myValue = 0;
			mg::common::ItemsCopyConstruct(&t2, &t1, 0);
			MG_COMMON_ASSERT(t2.myValue == 0);
			MG_COMMON_ASSERT(t1.myValue == 2);

			// Optimized copy for 1 element copy without loops
			// inside.
			mg::common::ItemsCopy(&t2, &t1);
			MG_COMMON_ASSERT(t2.myValue == 3);
			MG_COMMON_ASSERT(t1.myValue == 2);

			t2.myValue = 0;
			mg::common::ItemsCopyConstruct(&t2, &t1);
			MG_COMMON_ASSERT(t2.myValue == 2);
			MG_COMMON_ASSERT(t1.myValue == 2);
		}
	}

	static void
	UnitTestPoliciesMove()
	{
		{
			// 1 and 0 are often border cases. Check them
			// specifically.
			int a = 0;
			int b = 1;
			mg::common::ItemsMove(&b, &a, 1);
			MG_COMMON_ASSERT(b == 0);
			MG_COMMON_ASSERT(a == 0);

			b = 1;
			mg::common::ItemsMove(&b, &a, 0);
			MG_COMMON_ASSERT(b == 1);
			MG_COMMON_ASSERT(a == 0);

			mg::common::ItemsMoveConstruct(&b, &a, 1);
			MG_COMMON_ASSERT(b == 0);
			MG_COMMON_ASSERT(a == 0);

			b = 1;
			mg::common::ItemsMoveConstruct(&b, &a, 0);
			MG_COMMON_ASSERT(a == 0);
			MG_COMMON_ASSERT(b == 1);

			// Optimized move for 1 element without loops inside.
			mg::common::ItemsMove(&b, &a);
			MG_COMMON_ASSERT(b == 0);
			MG_COMMON_ASSERT(a == 0);

			b = 1;
			mg::common::ItemsMoveConstruct(&b, &a);
			MG_COMMON_ASSERT(b == 0);
			MG_COMMON_ASSERT(a == 0);
		}
		// Move many. For trivial types is just memcpy.
		{
			int a[3] = {0, 1, 2};
			int b[3] = {3, 4, 5};
			mg::common::ItemsMove(b, a, 3);
			for (int i = 0; i <= 2; ++i)
			{
				MG_COMMON_ASSERT(a[i] == i);
				MG_COMMON_ASSERT(b[i] == a[i]);
			}
		}
		{
			// Move back, from higher address to lower address.
			int a[4] = {0, 1, 2, 3};
			mg::common::ItemsMoveBack(&a[1], &a[2], 2);
			MG_COMMON_ASSERT(a[0] == 0 && a[1] == 2 && a[2] == 3 && a[3] == 3);

			// Move 0 should work, because that is intuitive, and
			// consistent with memcpy.
			a[1] = 1;
			a[2] = 2;
			a[3] = 3;
			mg::common::ItemsMoveBack(&a[1], &a[2], 0);
			MG_COMMON_ASSERT(a[0] == 0 && a[1] == 1 && a[2] == 2 && a[3] == 3);

			// Move forward, from lower address to higher address.
			mg::common::ItemsMoveForward(&a[2], &a[1], 2);
			MG_COMMON_ASSERT(a[0] == 0 && a[1] == 1 && a[2] == 1 && a[3] == 2);

			// Move 0 should work, because that is intuitive, and
			// consistent with memcpy.
			a[2] = 2;
			a[3] = 3;
			mg::common::ItemsMoveForward(&a[2], &a[1], 0);
			MG_COMMON_ASSERT(a[0] == 0 && a[1] == 1 && a[2] == 2 && a[3] == 3);
		}
		// Move construct many. For trivial types is just memcpy.
		{
			int a[3] = {0, 1, 2};
			int b[3] = {3, 4, 5};
			mg::common::ItemsMoveConstruct(b, a, 3);
			for (int i = 0; i <= 2; ++i)
			{
				MG_COMMON_ASSERT(a[i] == i);
				MG_COMMON_ASSERT(b[i] == a[i]);
			}
		}
		// Move non-trivial type without move-assignment and
		// without a proper move-constructor.
		{
			struct T
			{
				T() = default;

				T(
					const T& aObj)
				{
					myValue = aObj.myValue + 1;
				}

				int myValue;
			};
			T t1[3];
			t1[0].myValue = 0;
			t1[1].myValue = 1;
			t1[2].myValue = 2;
			T t2[3];
			t2[0].myValue = 3;
			t2[1].myValue = 4;
			t2[2].myValue = 5;

			// 1 and 0 are often border cases. Check them
			// specifically.
			mg::common::ItemsMove(t2, t1, 1);
			MG_COMMON_ASSERT(t1[0].myValue == 0);
			MG_COMMON_ASSERT(t2[0].myValue == 0);

			t2[0].myValue = 3;
			mg::common::ItemsMove(t2, t1, 0);
			MG_COMMON_ASSERT(t1[0].myValue == 0);
			MG_COMMON_ASSERT(t2[0].myValue == 3);

			// Note, copy-constructor is used when no
			// move-constructor.
			mg::common::ItemsMoveConstruct(t2, t1, 1);
			MG_COMMON_ASSERT(t1[0].myValue == 0);
			MG_COMMON_ASSERT(t2[0].myValue == 1);

			t2[0].myValue = 3;
			mg::common::ItemsMoveConstruct(t2, t1, 0);
			MG_COMMON_ASSERT(t1[0].myValue == 0);
			MG_COMMON_ASSERT(t2[0].myValue == 3);

			// Optimized move for 1 element without loops inside.
			mg::common::ItemsMove(t2, t1);
			MG_COMMON_ASSERT(t1[0].myValue == 0);
			MG_COMMON_ASSERT(t2[0].myValue == 0);

			t2[0].myValue = 3;
			mg::common::ItemsMoveConstruct(t2, t1);
			MG_COMMON_ASSERT(t1[0].myValue == 0);
			MG_COMMON_ASSERT(t2[0].myValue == 1);

			// Move many.
			t2[0].myValue = 3;
			mg::common::ItemsMove(t2, t1, 3);
			for (int i = 0; i <= 2; ++i)
			{
				MG_COMMON_ASSERT(t1[i].myValue == i);
				MG_COMMON_ASSERT(t2[i].myValue == t1[i].myValue);
			}

			t2[0].myValue = 3;
			t2[1].myValue = 4;
			t2[2].myValue = 5;
			mg::common::ItemsMoveConstruct(t2, t1, 3);
			for (int i = 0; i <= 2; ++i)
			{
				MG_COMMON_ASSERT(t1[i].myValue == i);
				MG_COMMON_ASSERT(t2[i].myValue == t1[i].myValue + 1);
			}
		}
		// Move non-trivial type with move-assignment, but no
		// proper move-constructor.
		{
			struct T
			{
				T() = default;

				T&
				operator=(
					const T& aObj)
				{
					myValue = aObj.myValue * 10;
					return *this;
				}

				T&
				operator=(
					T&& aObj)
				{
					myValue = aObj.myValue * 20;
					aObj.myValue = 0;
					return *this;
				}

				int myValue;
			};
			T t1[3];
			t1[0].myValue = 0;
			t1[1].myValue = 1;
			t1[2].myValue = 2;
			T t2[3];
			t2[0].myValue = 3;
			t2[1].myValue = 4;
			t2[2].myValue = 5;

			// 1 and 0 are often border cases. Check them
			// specifically.
			mg::common::ItemsMove(&t2[1], &t1[1], 1);
			MG_COMMON_ASSERT(t2[1].myValue == 20);
			MG_COMMON_ASSERT(t1[1].myValue == 0);

			t1[1].myValue = 1;
			t2[1].myValue = 4;
			mg::common::ItemsMove(&t2[1], &t1[1], 0);
			MG_COMMON_ASSERT(t2[1].myValue == 4);
			MG_COMMON_ASSERT(t1[1].myValue == 1);

			// Optimized move for 1 element without loops inside.
			t2[1].myValue = 4;
			mg::common::ItemsMove(&t2[1], &t1[1]);
			MG_COMMON_ASSERT(t2[1].myValue == 20);
			MG_COMMON_ASSERT(t1[1].myValue == 0);

			// Move many.
			t1[1].myValue = 1;
			t2[1].myValue = 4;
			mg::common::ItemsMove(t2, t1, 3);
			for (int i = 0; i <= 2; ++i)
				MG_COMMON_ASSERT(t1[i].myValue == 0);
			MG_COMMON_ASSERT(t2[0].myValue == 0);
			MG_COMMON_ASSERT(t2[1].myValue == 20);
			MG_COMMON_ASSERT(t2[2].myValue == 40);
		}
		// Move non-trivial type with move-assignment forward and
		// back.
		{
			struct T
			{
				T() = default;

				T&
				operator=(
					T&& aObj)
				{
					myValue = aObj.myValue * 10;
					aObj.myValue = 0;
					return *this;
				}

				int myValue;
			};
			T t[4];
			t[0].myValue = 0;
			t[1].myValue = 1;
			t[2].myValue = 2;
			t[3].myValue = 3;
			mg::common::ItemsMoveBack(&t[1], &t[2], 2);
			MG_COMMON_ASSERT(
				t[0].myValue == 0 && t[1].myValue == 20 && t[2].myValue == 30 &&
				t[3].myValue == 0
			);

			t[1].myValue = 1;
			t[2].myValue = 2;
			t[3].myValue = 3;
			mg::common::ItemsMoveForward(&t[2], &t[1], 2);
			MG_COMMON_ASSERT(
				t[0].myValue == 0 && t[1].myValue == 0 && t[2].myValue == 10 &&
				t[3].myValue == 20
			);

			// Move 0 should work in directional move of a
			// non-trivial type.
			t[0].myValue = 0;
			t[1].myValue = 1;
			t[2].myValue = 2;
			t[3].myValue = 3;
			mg::common::ItemsMoveBack(&t[1], &t[2], 0);
			MG_COMMON_ASSERT(
				t[0].myValue == 0 && t[1].myValue == 1 && t[2].myValue == 2 &&
				t[3].myValue == 3
			);

			mg::common::ItemsMoveForward(&t[1], &t[2], 0);
			MG_COMMON_ASSERT(
				t[0].myValue == 0 && t[1].myValue == 1 && t[2].myValue == 2 &&
				t[3].myValue == 3
			);
		}
		// Move construction for a non-trivial type having a
		// proper move constructor.
		{
			struct T
			{
				T() = default;

				T(
					T&& aObj)
				{
					myValue = aObj.myValue * 10;
					aObj.myValue = 0;
				}

				int myValue;
			};
			T t1[2];
			t1[0].myValue = 1;
			t1[1].myValue = 2;
			T t2[2];
			t2[0].myValue = 3;
			t2[1].myValue = 4;
			mg::common::ItemsMoveConstruct(t2, t1, 2);
			MG_COMMON_ASSERT(
				t1[0].myValue == 0 && t1[1].myValue == 0 &&
				t2[0].myValue == 10 && t2[1].myValue == 20
			);

			t1[0].myValue = 1;
			t2[0].myValue = 3;
			mg::common::ItemsMoveConstruct(t2, t1);
			MG_COMMON_ASSERT(t1[0].myValue == 0 && t2[0].myValue == 10);

			// Move 0 should work to be consistent with memcpy.
			t1[0].myValue = 1;
			t1[1].myValue = 2;
			t2[0].myValue = 3;
			t2[1].myValue = 4;
			mg::common::ItemsMoveConstruct(t2, t1, 0);
			MG_COMMON_ASSERT(
				t1[0].myValue == 1 && t1[1].myValue == 2 &&
				t2[0].myValue == 3 && t2[1].myValue == 4
			);
		}
	}

	static void
	UnitTestPoliciesConstruction()
	{
		// Construct many.
		{
			int a[3] = {0, 1, 2};
			mg::common::ItemsConstruct(a, 3);
			for (int i = 0; i <= 2; ++i)
				MG_COMMON_ASSERT(a[i] == i);
		}
		{
			// 1 and 0 are often border cases. Check them
			// specifically.
			int a = 0;
			mg::common::ItemsConstruct(&a, 1);
			MG_COMMON_ASSERT(a == 0);

			mg::common::ItemsConstruct(&a, 0);
			MG_COMMON_ASSERT(a == 0);

			// Optimized 1-case without loops inside.
			mg::common::ItemsConstruct(&a);
			MG_COMMON_ASSERT(a == 0);
		}
		// Construct trivial type is nop for structs as well.
		{
			struct T
			{
				int myValue;
			};
			T t[3];
			t[0].myValue = 0;
			t[1].myValue = 1;
			t[2].myValue = 2;
			mg::common::ItemsConstruct(t, 3);
			for (int i = 0; i <= 2; ++i)
				MG_COMMON_ASSERT(t[i].myValue == i);

			// 0 construction should work to be consistent with
			// other Items functions.
			mg::common::ItemsConstruct(t, 0);
			for (int i = 0; i <= 2; ++i)
				MG_COMMON_ASSERT(t[i].myValue == i);
		}
		// Non-trivial type uses constructor properly.
		{
			struct T
			{
				T()
				{
					myValue = 15;
				}

				int myValue;
			};
			T t[3];
			t[0].myValue = 0;
			t[1].myValue = 1;
			t[2].myValue = 2;
			mg::common::ItemsConstruct(t, 3);
			for (int i = 0; i <= 2; ++i)
				MG_COMMON_ASSERT(t[i].myValue == 15);

			// Optimized 1-case without loops inside.
			t[0].myValue = 0;
			mg::common::ItemsConstruct(t);
			MG_COMMON_ASSERT(t[0].myValue == 15);

			// 0 construction should work to be consistent with
			// other Items functions.
			t[0].myValue = 0;
			t[1].myValue = 1;
			t[2].myValue = 2;
			mg::common::ItemsConstruct(t, 0);
			for (int i = 0; i <= 2; ++i)
				MG_COMMON_ASSERT(t[i].myValue == i);
		}
		// Ensure construction calls all the hierarchy of
		// constructors.
		{
			struct Parent
			{
				Parent()
				{
					myValue = 1;
				}

				virtual ~Parent() = default;

				int myValue;
			};

			struct Child
				: public Parent
			{
				int mySecondValue;
			};

			Child c[2];
			c[0].myValue = 0;
			c[0].mySecondValue = 0;
			c[1].myValue = 0;
			c[1].mySecondValue = 0;
			mg::common::ItemsConstruct(c, 2);
			MG_COMMON_ASSERT(c[0].myValue == 1);
			MG_COMMON_ASSERT(c[0].mySecondValue == 0);
			MG_COMMON_ASSERT(c[1].myValue == 1);
			MG_COMMON_ASSERT(c[1].mySecondValue == 0);
		}
	}

	static void
	UnitTestPoliciesDestruction()
	{
		// Destroy many.
		{
			int a[3] = {0, 1, 2};
			mg::common::ItemsDestroy(a, 3);
			for (int i = 0; i <= 2; ++i)
				MG_COMMON_ASSERT(a[i] == i);
		}
		{
			// 1 and 0 are often border cases. Check them
			// specifically.
			int a = 0;
			mg::common::ItemsDestroy(&a, 1);
			MG_COMMON_ASSERT(a == 0);

			mg::common::ItemsDestroy(&a, 0);
			MG_COMMON_ASSERT(a == 0);

			// Optimized destruction for 1 element without loops
			// inside.
			mg::common::ItemsDestroy(&a);
			MG_COMMON_ASSERT(a == 0);
		}
		// Destroy trivial type without any methods. Should be
		// nop.
		{
			struct T
			{
				int myValue;
			};
			T t[3];
			t[0].myValue = 0;
			t[1].myValue = 1;
			t[2].myValue = 2;
			mg::common::ItemsDestroy(t, 3);
			for (int i = 0; i <= 2; ++i)
				MG_COMMON_ASSERT(t[i].myValue == i);

			// Destroy zero should work to be consistent with
			// other Items functions.
			mg::common::ItemsDestroy(t, 0);
			for (int i = 0; i <= 2; ++i)
				MG_COMMON_ASSERT(t[i].myValue == i);

			// Optimized 1 element destruction without loops
			// inside.
			t[0].myValue = 0;
			mg::common::ItemsDestroy(t);
			MG_COMMON_ASSERT(t[0].myValue == 0);
		}
		// Destroy a non-trivial type with a proper destructor.
		{
			struct T
			{
				~T()
				{
					myValue = -1;
#if IS_COMPILER_GCC && IS_BUILD_RELEASE
					// Otherwise GCC optimizes the destructor out
					// seeing that the values are re-assigned
					// later anyway.
					mg::common::CompilerBarrier();
#endif
				}

				int myValue;
			};
			T t[3];
			t[0].myValue = 0;
			t[1].myValue = 1;
			t[2].myValue = 2;
			mg::common::ItemsDestroy(t, 3);
			for (int i = 0; i <= 2; ++i)
				MG_COMMON_ASSERT(t[i].myValue == -1);

			// Destroy zero should work to be consistent with
			// other Items functions.
			t[0].myValue = 0;
			t[1].myValue = 1;
			t[2].myValue = 2;
			mg::common::ItemsDestroy(t, 0);
			for (int i = 0; i <= 2; ++i)
				MG_COMMON_ASSERT(t[i].myValue == i);

			// Optimized 1 element destruction without loops
			// inside.
			t[0].myValue = 0;
			mg::common::ItemsDestroy(t);
			MG_COMMON_ASSERT(t[0].myValue == -1);
		}
		// Ensure virtual destructors are called properly.
		{
			struct Parent
			{
				virtual
				~Parent()
				{
					myValue = -1;
#if IS_COMPILER_GCC && IS_BUILD_RELEASE
					mg::common::CompilerBarrier();
#endif
				}

				int myValue;
			};

			struct Child
				: public Parent
			{
				int mySecondValue;
			};

			Child c[2];
			c[0].myValue = 0;
			c[0].mySecondValue = 0;
			c[1].myValue = 0;
			c[1].mySecondValue = 0;
			mg::common::ItemsDestroy(c, 2);
			MG_COMMON_ASSERT(c[0].myValue == -1);
			MG_COMMON_ASSERT(c[0].mySecondValue == 0);
			MG_COMMON_ASSERT(c[1].myValue == -1);
			MG_COMMON_ASSERT(c[1].mySecondValue == 0);
		}
	}

	static void
	UnitTestPoliciesCompare()
	{
		MG_COMMON_ASSERT(!mg::common::ItemsAreEq(1, 2));
		MG_COMMON_ASSERT(mg::common::ItemsAreEq(1, 1));
		{
			struct NotComparable
			{
				int myValue;
			};

			struct Comparable
			{
				bool
				operator==(
					const Comparable& aObj) const
				{
					return myValue == aObj.myValue;
				}

				int myValue;
			};

			NotComparable nc1;
			NotComparable nc2;
			// It should compile, but will crash at call, so add
			// the dummy 'true' check to never call the
			// comparator.
			MG_COMMON_ASSERT(MG_TEST(true) || mg::common::ItemsAreEq(nc1, nc2));

			Comparable c1;
			c1.myValue = 0;
			Comparable c2;
			c2.myValue = 1;
			MG_COMMON_ASSERT(!mg::common::ItemsAreEq(c1, c2));
			MG_COMMON_ASSERT(mg::common::ItemsAreEq(c1, c1));
			c2.myValue = c1.myValue;
			MG_COMMON_ASSERT(mg::common::ItemsAreEq(c1, c2));
		}
	}

	static void
	UnitTestPoliciesSwap()
	{
		struct T
		{
			T()
				: myValue(0)
			{
			}

			T(
				int aValue)
				: myValue(aValue)
			{
			}

			T(
				T&& aT)
				: myValue(aT.myValue)
			{
				aT.myValue = 0;
			}

			T& operator=(
				T&& aT)
			{
				myValue = aT.myValue;
				aT.myValue = 0;
				return *this;
			}

			int myValue;
		};

		// One pair swap.
		{
			T t1(1);
			T t2(2);
			mg::common::ItemsSwap(&t1, &t2);
			MG_COMMON_ASSERT(t1.myValue == 2);
			MG_COMMON_ASSERT(t2.myValue == 1);
		}
		// Swap arrays.
		{
			const int count = 5;
			T ts1[count];
			T ts2[count];
			for (int i = 0; i < count; ++i)
			{
				ts1[i].myValue = -i;
				ts2[i].myValue = i;
			}
			mg::common::ItemsSwap(ts1, ts2, count);
			for (int i = 0; i < count; ++i)
			{
				MG_COMMON_ASSERT(ts1[i].myValue == i);
				MG_COMMON_ASSERT(ts2[i].myValue == -i);
			}
		}
		// Swap 0.
		{
			struct BadT
			{
				BadT()
				{
					myValue = 0;
				}

				BadT(
					int aValue)
					: myValue(aValue)
				{
				}

				BadT(
					BadT&&)
				{
					MG_COMMON_ASSERT(false);
				}

				BadT& operator=(
					BadT&&)
				{
					MG_COMMON_ASSERT(false);
					return *this;
				}

				int myValue;
			};
			BadT t1(1);
			BadT t2(2);
			mg::common::ItemsSwap(&t1, &t2, 0);
			MG_COMMON_ASSERT(t1.myValue == 1);
			MG_COMMON_ASSERT(t2.myValue == 2);
		}
		// The same but with simple type.
		{
			int t1 = 1;
			int t2 = 2;
			mg::common::ItemsSwap(&t1, &t2);
			MG_COMMON_ASSERT(t1 == 2);
			MG_COMMON_ASSERT(t2 == 1);
		}
		{
			const int count = 5;
			int ts1[count];
			int ts2[count];
			for (int i = 0; i < count; ++i)
			{
				ts1[i] = -i;
				ts2[i] = i;
			}
			mg::common::ItemsSwap(ts1, ts2, count);
			for (int i = 0; i < count; ++i)
			{
				MG_COMMON_ASSERT(ts1[i] == i);
				MG_COMMON_ASSERT(ts2[i] == -i);
			}
		}
		{
			int t1 = 1;
			int t2 = 2;
			mg::common::ItemsSwap(&t1, &t2, 0);
			MG_COMMON_ASSERT(t1 == 1);
			MG_COMMON_ASSERT(t2 == 2);
		}
	}

	static void
	UnitTestPoliciesDelete()
	{
		{
			static int destCount = 0;
			static int delCount = 0;
			struct T
			{
				~T()
				{
					++destCount;
				}

				void
				operator delete(
					void *)
				{
					++delCount;
				}

				int myValue = 0;
			};
			T* t = new T();
			mg::common::ItemsDelete(&t);
			MG_COMMON_ASSERT(delCount == 1);
			delCount = 0;
			MG_COMMON_ASSERT(destCount == 1);
			destCount = 0;

			T* arr[3] = {new T(), nullptr, new T()};
			mg::common::ItemsDelete(arr, 3);
			MG_COMMON_ASSERT(delCount == 2);
			delCount = 0;
			MG_COMMON_ASSERT(destCount == 2);
			destCount = 0;
		}
		{
			int* i = new int;
			mg::common::ItemsDelete(&i);

			int* arr[3] = {nullptr, new int(1), new int(2)};
			mg::common::ItemsDelete(arr, 3);
		}
	}

	void
	UnitTestPolicies()
	{
		TestSuiteGuard suite("Policies");

		UnitTestPoliciesCopy();
		UnitTestPoliciesMove();
		UnitTestPoliciesConstruction();
		UnitTestPoliciesDestruction();
		UnitTestPoliciesCompare();
		UnitTestPoliciesSwap();
		UnitTestPoliciesDelete();
	}

}
}

#if IS_COMPILER_GCC
#pragma GCC diagnostic pop
#endif
