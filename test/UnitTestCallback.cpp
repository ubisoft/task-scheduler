#include "mg/common/Callback.h"
#include "mg/common/HybridArray.h"

#include "UnitTest.h"

namespace mg {
namespace unittests {

	static int
	UTCallbackSum(
		int aArg1,
		int aArg2)
	{
		return aArg1 + aArg2;
	}

	// The test value is templated so as it would be possible to
	// check, that a single callback can store a callable object
	// of any type, even switch between them at runtime, and will
	// properly call the old type destructor when if reset to a
	// new type.
	//
	template<int ID>
	struct UTCallbackValue
	{
		UTCallbackValue(
			int aValue)
			: myValue(aValue)
		{
			++ourConstrCount;
		}

		UTCallbackValue()
		{
			++ourConstrCount;
		}

		~UTCallbackValue()
		{
			myValue = -1;
			++ourDestrCount;
		}

		UTCallbackValue(
			const UTCallbackValue& aSrc)
		{
			myValue = aSrc.myValue;
			++ourCopyConstrCount;
		}

		UTCallbackValue(
			UTCallbackValue&& aSrc)
		{
			myValue = aSrc.myValue;
			++ourMoveConstrCount;
		}

		UTCallbackValue&
		operator=(
			const UTCallbackValue& aSrc)
		{
			myValue = aSrc.myValue;
			++ourCopyAssignCount;
			return *this;
		}

		UTCallbackValue&
		operator=(
			UTCallbackValue&& aSrc)
		{
			myValue = aSrc.myValue;
			++ourMoveAssignCount;
			return *this;
		}

		int
		operator()(
			int aArg1,
			int aArg2)
		{
			return aArg1 + aArg2 + myValue;
		}

		// A plain method, nothing important.
		int
		Sum(
			int aArg1,
			int aArg2)
		{
			return aArg1 + aArg2 + myValue;
		}

		// Method with 'const' modifier to test Callback being
		// able to call such methods.
		int
		SumConst(
			int aArg1,
			int aArg2) const
		{
			return aArg1 + aArg2 + myValue;
		}

		// Method taking a value by copy to see how argument
		// perfect forwarding works.
		int
		SumCopy(
			UTCallbackValue aArg)
		{
			return myValue + aArg.myValue;
		}

		// Method taking an rvalue reference to see if no copying
		// occurs during perfect forwarding anywhere.
		int
		SumMove(
			UTCallbackValue&& aArg)
		{
			return myValue + aArg.myValue;
		}

		// Method taking a reference by copy to see if argument
		// forwarding compiles with references.
		int
		SumRef(
			const UTCallbackValue& aArg)
		{
			return myValue + aArg.myValue;
		}

		// All the same methods but with 'const', since Callback
		// stores mutable and const a bit differently.

		int
		SumCopyConst(
			UTCallbackValue aArg) const
		{
			return myValue + aArg.myValue;
		}

		int
		SumMoveConst(
			UTCallbackValue&& aArg) const
		{
			return myValue + aArg.myValue;
		}

		int
		SumRefConst(
			const UTCallbackValue& aArg) const
		{
			return myValue + aArg.myValue;
		}

		// Methods to see how returned value forwarding works.

		UTCallbackValue
		Copy()
		{
			return UTCallbackValue(myValue);
		}

		UTCallbackValue
		CopyConst() const
		{
			return UTCallbackValue(myValue);
		}

		UTCallbackValue
		CreateSum(
			int aArg)
		{
			return UTCallbackValue(aArg + myValue);
		}

		UTCallbackValue
		CreateSumConst(
			int aArg) const
		{
			return UTCallbackValue(aArg + myValue);
		}

		// Methods to see how returned reference forwarding works.

		UTCallbackValue&
		GetRef()
		{
			return *this;
		}

		const UTCallbackValue&
		GetRefConst() const
		{
			return *this;
		}

		// Plain methods to check Callback object transitions.

		int
		Get()
		{
			return myValue;
		}

		int
		GetConst() const
		{
			return myValue;
		}

		int
		GetPlus()
		{
			return myValue + 1;
		}

		int
		GetPlusConst() const
		{
			return myValue + 1;
		}

		// Statistics to validate copies, moves, etc.

		static void
		UseCopyConstrCount(
			int aToUse)
		{
			ourCopyConstrCount -= aToUse;
			MG_COMMON_ASSERT(ourCopyConstrCount >= 0);
		}

		static void
		UseCopyAssignCount(
			int aToUse)
		{
			ourCopyAssignCount -= aToUse;
			MG_COMMON_ASSERT(ourCopyAssignCount >= 0);
		}

		static void
		UseMoveConstrCount(
			int aToUse)
		{
			ourMoveConstrCount -= aToUse;
			MG_COMMON_ASSERT(ourMoveConstrCount >= 0);
		}

		static void
		UseMoveAssignCount(
			int aToUse)
		{
			ourMoveAssignCount -= aToUse;
			MG_COMMON_ASSERT(ourMoveAssignCount >= 0);
		}

		static void
		UseConstrCount(
			int aToUse)
		{
			ourConstrCount -= aToUse;
			MG_COMMON_ASSERT(ourConstrCount >= 0);
		}

		static void
		UseDestrCount(
			int aToUse)
		{
			ourDestrCount -= aToUse;
			MG_COMMON_ASSERT(ourDestrCount >= 0);
		}

		static void
		ResetCounters()
		{
			ourCopyConstrCount = 0;
			ourCopyAssignCount = 0;
			ourMoveConstrCount = 0;
			ourMoveAssignCount = 0;
			ourConstrCount = 0;
			ourDestrCount = 0;
		}

		static void
		CheckCounters()
		{
			MG_COMMON_ASSERT(
				ourCopyConstrCount == 0 && ourCopyAssignCount == 0 &&
				ourMoveConstrCount == 0 && ourMoveAssignCount == 0 &&
				ourConstrCount == 0 && ourDestrCount == 0
			);
		}

		int myValue;

		static int ourCopyConstrCount;
		static int ourCopyAssignCount;
		static int ourMoveConstrCount;
		static int ourMoveAssignCount;
		static int ourConstrCount;
		static int ourDestrCount;
	};

	template<int ID>
	int UTCallbackValue<ID>::ourCopyConstrCount = 0;

	template<int ID>
	int UTCallbackValue<ID>::ourCopyAssignCount = 0;

	template<int ID>
	int UTCallbackValue<ID>::ourMoveConstrCount = 0;

	template<int ID>
	int UTCallbackValue<ID>::ourMoveAssignCount = 0;

	template<int ID>
	int UTCallbackValue<ID>::ourConstrCount = 0;

	template<int ID>
	int UTCallbackValue<ID>::ourDestrCount = 0;

	using UTCallbackValue1 = UTCallbackValue<1>;
	using UTCallbackValue2 = UTCallbackValue<2>;

	static void
	UnitTestCallbackBasic()
	{
		// Basic test to be able to call a plain function.
		mg::common::Callback<int(int, int)> sum;
		MG_COMMON_ASSERT(sum.IsEmpty());
		sum.Set(&UTCallbackSum);
		MG_COMMON_ASSERT(!sum.IsEmpty());
		MG_COMMON_ASSERT(sum(1, 2) == 3);

		// Basic test to store a plain function by value (no &).
		sum.Set(UTCallbackSum);
		MG_COMMON_ASSERT(!sum.IsEmpty());
		MG_COMMON_ASSERT(sum(1, 2) == 3);

		// Basic test to store lambdas.
		auto lambda = [](int aArg1, int aArg2) -> int { return aArg1 + aArg2; };
		sum.Set(lambda);
		MG_COMMON_ASSERT(!sum.IsEmpty());
		MG_COMMON_ASSERT(sum(3, 4) == 7);

		// Basic test to store a mutable object method.
		UTCallbackValue1 cv(1);
		sum.Set(&cv, &UTCallbackValue1::Sum);
		MG_COMMON_ASSERT(!sum.IsEmpty());
		MG_COMMON_ASSERT(sum(2, 3) == 6);

		// Basic test to store a constant object method.
		cv.myValue = 2;
		sum.Set(&cv, &UTCallbackValue1::SumConst);
		MG_COMMON_ASSERT(!sum.IsEmpty());
		MG_COMMON_ASSERT(sum(-1, 3) == 4);
	}

	static void
	UnitTestCallbackCallableClass()
	{
		{
			UTCallbackValue1 cv(1);
			UTCallbackValue1::ResetCounters();
			UTCallbackValue2::ResetCounters();

			// A class having operator() overloaded can be stored
			// as a callback.
			mg::common::Callback<int(int, int)> sum(cv);
			MG_COMMON_ASSERT(sum(2, 3) == 6);
			// Copied into the callback object.
			UTCallbackValue1::UseCopyConstrCount(1);
			UTCallbackValue1::CheckCounters();

			sum.Set(mg::common::Move(cv));
			// Moved into the callback object.
			UTCallbackValue1::UseMoveConstrCount(1);
			// The old one is destroyed.
			UTCallbackValue1::UseDestrCount(1);
			// Saved by value. Change of the original won't affect
			// the copy.
			cv.myValue = 2;
			MG_COMMON_ASSERT(sum(3, 4) == 8);
			UTCallbackValue1::CheckCounters();

			// Test copy via Set().
			sum.Set(cv);
			UTCallbackValue1::UseDestrCount(1);
			UTCallbackValue1::UseCopyConstrCount(1);
			cv.myValue = 3;
			MG_COMMON_ASSERT(sum(5, 2) == 9);
			UTCallbackValue1::CheckCounters();

			// Can switch one callback between different callable
			// types at runtime. The previous type is destroyed
			// with its destructor, not with the new type's
			// destructor. That should check the vtables are
			// switched properly.
			UTCallbackValue2 cv2(6);
			UTCallbackValue2::UseConstrCount(1);

			sum.Set(cv2);
			UTCallbackValue1::UseDestrCount(1);
			UTCallbackValue2::UseCopyConstrCount(1);
			MG_COMMON_ASSERT(sum(1, 2) == 9);

			sum.Set(cv);
			UTCallbackValue2::UseDestrCount(1);
			UTCallbackValue1::UseCopyConstrCount(1);
			MG_COMMON_ASSERT(sum(1, 2) == 6);
			UTCallbackValue1::CheckCounters();

			// Move of the callback moves its functor too.
			mg::common::Callback<int(int, int)> sum2(mg::common::Move(sum));
			UTCallbackValue1::UseMoveConstrCount(1);
			MG_COMMON_ASSERT(sum2(1, 2) == 6);
			UTCallbackValue1::CheckCounters();

			// Callback copy will properly copy the functor.
			mg::common::Callback<int(int, int)> sum3(sum);
			UTCallbackValue1::UseCopyConstrCount(1);
			MG_COMMON_ASSERT(sum2(3, 4) == 10);
			UTCallbackValue1::CheckCounters();

			// Callback can refer to its own operator. Can't store
			// another callback by value, because won't fit into
			// its own buffer.
			sum3.Set(&sum2, &mg::common::Callback<int(int, int)>::operator());
			// Old value of sum3 is destroyed.
			UTCallbackValue1::UseDestrCount(1);
			MG_COMMON_ASSERT(sum3(1, 2) == 6);
			UTCallbackValue1::CheckCounters();

			MG_COMMON_ASSERT(!sum2.IsEmpty());
			sum2.Clear();
			MG_COMMON_ASSERT(sum2.IsEmpty());
			UTCallbackValue1::UseDestrCount(1);
		}
		UTCallbackValue1::UseDestrCount(2);
		UTCallbackValue2::UseDestrCount(1);
		UTCallbackValue1::CheckCounters();
		UTCallbackValue2::CheckCounters();
	}

	static void
	UnitTestCallbackCallableClassAssign()
	{
		{
			UTCallbackValue1 cv(1);
			UTCallbackValue1::ResetCounters();

			// Assign to an empty callback.
			mg::common::Callback<int(int, int)> sum;
			sum = cv;
			UTCallbackValue1::UseCopyConstrCount(1);
			MG_COMMON_ASSERT(sum(1, 2) == 4);

			// Assign to a non-empty callback.
			UTCallbackValue1 cv2(2);
			UTCallbackValue1::UseConstrCount(1);
			sum = cv2;
			UTCallbackValue1::UseDestrCount(1);
			UTCallbackValue1::UseCopyConstrCount(1);
			MG_COMMON_ASSERT(sum(1, 2) == 5);

			// Assign not to a callable class.
			sum = UTCallbackSum;
			UTCallbackValue1::UseDestrCount(1);
			MG_COMMON_ASSERT(sum(1, 2) == 3);
		}
		UTCallbackValue1::UseDestrCount(2);
		UTCallbackValue1::CheckCounters();
	}

	static void
	UnitTestCallbackLambda()
	{
		{
			UTCallbackValue1 v1(1);
			UTCallbackValue1::ResetCounters();

			// Check if lambda properly captures local variables.
			mg::common::Callback<int(int, int)> sum(
				[=](int aArg1, int aArg2)->int {
					return v1.myValue + aArg1 + aArg2;
				}
			);
			// There is copy-constructor + move + destructor,
			// because the lambda object is created before calling
			// Callback constructor. It is not possible to
			// construct the lambda right in-place inside
			// Callback. But this definitely should not be
			// double-copy constructor. The second one should be
			// move.
			UTCallbackValue1::UseCopyConstrCount(1);
			UTCallbackValue1::UseMoveConstrCount(1);
			UTCallbackValue1::UseDestrCount(1);
			UTCallbackValue1::CheckCounters();

			MG_COMMON_ASSERT(sum(2, 3) == 6);
			MG_COMMON_ASSERT(sum(5, 3) == 9);

			// Lambda is properly moved.
			mg::common::Callback<int(int, int)> sum2(mg::common::Move(sum));
			UTCallbackValue1::UseMoveConstrCount(1);
			MG_COMMON_ASSERT(sum2(3, 4) == 8);
			// Check both Set and constructor.
			sum2.Set(mg::common::Move(sum));
			UTCallbackValue1::UseDestrCount(1);
			UTCallbackValue1::UseMoveConstrCount(1);
			MG_COMMON_ASSERT(sum2(3, 1) == 5);
			UTCallbackValue1::CheckCounters();

			// Lambda is properly copied.
			mg::common::Callback<int(int, int)> sum3(sum);
			UTCallbackValue1::UseCopyConstrCount(1);
			MG_COMMON_ASSERT(sum3(1, 5) == 7);
			// Check both Set and constructor.
			sum3.Set(sum);
			UTCallbackValue1::UseDestrCount(1);
			UTCallbackValue1::UseCopyConstrCount(1);
			MG_COMMON_ASSERT(sum3(2, 8) == 11);
			UTCallbackValue1::CheckCounters();

			MG_COMMON_ASSERT(!sum.IsEmpty());
			sum.Clear();
			MG_COMMON_ASSERT(sum.IsEmpty());
			MG_COMMON_ASSERT(!sum2.IsEmpty());
			sum2.Clear();
			MG_COMMON_ASSERT(sum2.IsEmpty());
			MG_COMMON_ASSERT(!sum3.IsEmpty());
			sum3.Clear();
			MG_COMMON_ASSERT(sum3.IsEmpty());
			UTCallbackValue1::UseDestrCount(3);
			// Double clear does not break anything.
			sum.Clear();
			sum2.Clear();
			sum3.Clear();
			UTCallbackValue1::CheckCounters();
		}
		UTCallbackValue1::UseDestrCount(1);
		UTCallbackValue1::CheckCounters();
	}

	static void
	UnitTestCallbackLambdaArgumentForward()
	{
		{
			UTCallbackValue1 v1(1);
			UTCallbackValue1::ResetCounters();

			mg::common::Callback<int(UTCallbackValue1)> getCopy;
			mg::common::Callback<int(UTCallbackValue1&&)> getMove;
			mg::common::Callback<int(const UTCallbackValue1&)> getRef;

			// Arguments should be perfectly forwarded, without a
			// single copy done (unless they are passed by value).
			getRef.Set(
				[](const UTCallbackValue1& aArg)->int {
					return aArg.myValue;
				}
			);
			UTCallbackValue1::CheckCounters();

			MG_COMMON_ASSERT(getRef(v1) == 1);
			v1.myValue = 2;
			MG_COMMON_ASSERT(getRef(v1) == 2);
			UTCallbackValue1::CheckCounters();

			getMove.Set(
				[](UTCallbackValue1&& aArg)->int {
					return aArg.myValue;
				}
			);
			UTCallbackValue1::CheckCounters();

			v1.myValue = 3;
			MG_COMMON_ASSERT(getMove(mg::common::Move(v1)) == 3);
			UTCallbackValue1::CheckCounters();

			MG_COMMON_ASSERT(getMove(UTCallbackValue1(4)) == 4);
			UTCallbackValue1::UseConstrCount(1);
			UTCallbackValue1::UseDestrCount(1);
			UTCallbackValue1::CheckCounters();

			// Arguments passed by value also should be forwarded,
			// without intermediate copies inside the Callback
			// object. Only one copy is made as required by the
			// signature taking the object by value. Everything
			// else shall use moves.
			getCopy.Set(
				[](UTCallbackValue1 aArg)->int {
					return aArg.myValue;
				}
			);
			MG_COMMON_ASSERT(getCopy(v1) == 3);
			// One copy as required by the signature.
			UTCallbackValue1::UseCopyConstrCount(1);
			// One move, because the copy is created by the
			// Callback object having the same signature as the
			// lambda. The copy is then moved to the lambda.
			UTCallbackValue1::UseMoveConstrCount(1);
			UTCallbackValue1::UseDestrCount(2);
			UTCallbackValue1::CheckCounters();
		}
		UTCallbackValue1::UseDestrCount(1);
		UTCallbackValue1::CheckCounters();
	}

	static void
	UnitTestCallbackMethodArgumentForward()
	{
		{
			UTCallbackValue1 v1(1);
			UTCallbackValue1 v2(2);
			UTCallbackValue1::ResetCounters();

			// Arguments should be perfectly forwarded, without a
			// single copy done (unless they are passed by value).
			mg::common::Callback<int(UTCallbackValue1)> sumCopy;
			mg::common::Callback<int(UTCallbackValue1&&)> sumMove;
			mg::common::Callback<int(const UTCallbackValue1&)> sumRef;
			UTCallbackValue1::CheckCounters();

			sumCopy.Set(&v1, &UTCallbackValue1::SumCopy);
			MG_COMMON_ASSERT(sumCopy(v2) == 3);
			// One copy as required by the signature.
			UTCallbackValue1::UseCopyConstrCount(1);
			// One move, because the copy is created by the
			// Callback object having the same signature as the
			// method. The copy is then moved to the method.
			UTCallbackValue1::UseMoveConstrCount(1);
			UTCallbackValue1::UseDestrCount(2);
			UTCallbackValue1::CheckCounters();

			// Const method invoker has a bit different
			// implementation. Check it also does the perfect
			// forwarding.
			sumCopy.Set((const UTCallbackValue1*)&v1, &UTCallbackValue1::SumCopyConst);
			v2.myValue = 3;
			MG_COMMON_ASSERT(sumCopy(v2) == 4);
			UTCallbackValue1::UseCopyConstrCount(1);
			UTCallbackValue1::UseMoveConstrCount(1);
			UTCallbackValue1::UseDestrCount(2);
			UTCallbackValue1::CheckCounters();

			sumMove.Set(&v1, &UTCallbackValue1::SumMove);
			v2.myValue = 4;
			MG_COMMON_ASSERT(sumMove(mg::common::Move(v2)) == 5);
			UTCallbackValue1::CheckCounters();
			MG_COMMON_ASSERT(sumMove(UTCallbackValue1(5)) == 6);
			UTCallbackValue1::UseConstrCount(1);
			UTCallbackValue1::UseDestrCount(1);
			UTCallbackValue1::CheckCounters();

			sumMove.Set((const UTCallbackValue1*)&v1, &UTCallbackValue1::SumMoveConst);
			v2.myValue = 6;
			MG_COMMON_ASSERT(sumMove(mg::common::Move(v2)) == 7);
			UTCallbackValue1::CheckCounters();
			MG_COMMON_ASSERT(sumMove(UTCallbackValue1(7)) == 8);
			UTCallbackValue1::UseConstrCount(1);
			UTCallbackValue1::UseDestrCount(1);
			UTCallbackValue1::CheckCounters();

			sumRef.Set(&v1, &UTCallbackValue1::SumRef);
			v2.myValue = 8;
			MG_COMMON_ASSERT(sumRef(v2) == 9);
			UTCallbackValue1::CheckCounters();
			MG_COMMON_ASSERT(sumRef(UTCallbackValue1(9)) == 10);
			UTCallbackValue1::UseConstrCount(1);
			UTCallbackValue1::UseDestrCount(1);
			UTCallbackValue1::CheckCounters();

			sumRef.Set(&v1, &UTCallbackValue1::SumRefConst);
			v2.myValue = 10;
			MG_COMMON_ASSERT(sumRef(v2) == 11);
			UTCallbackValue1::CheckCounters();
			MG_COMMON_ASSERT(sumRef(UTCallbackValue1(11)) == 12);
			UTCallbackValue1::UseConstrCount(1);
			UTCallbackValue1::UseDestrCount(1);
			UTCallbackValue1::CheckCounters();
		}
		UTCallbackValue1::UseDestrCount(2);
		UTCallbackValue1::CheckCounters();
	}

	static void
	UnitTestCallbackReturnForward()
	{
		UTCallbackValue1::ResetCounters();
		{
			mg::common::Callback<UTCallbackValue1(int)> create(
				[](int aArg)->UTCallbackValue1 {
					return UTCallbackValue1(aArg);
				}
			);

			UTCallbackValue1 v1(create(1));
			MG_COMMON_ASSERT(v1.myValue == 1);
			// Value is created inside the lambda.
			UTCallbackValue1::UseConstrCount(1);
#if IS_COMPILER_MSVC && !IS_CPP_AT_LEAST_17
			// On non-clang it is returned to the value stored in
			// the test via move, and the original is destroyed.
			// Clang/MSVC19 manages to generate more efficient code
			// without moves and destructions.
			UTCallbackValue1::UseMoveConstrCount(1);
			UTCallbackValue1::UseDestrCount(1);
#endif
			UTCallbackValue1::CheckCounters();

			mg::common::Callback<UTCallbackValue1&()> ref(
				[&]()->UTCallbackValue1& {
					return v1.GetRef();
				}
			);
			MG_COMMON_ASSERT(ref().myValue == 1);
			v1.myValue = 2;
			MG_COMMON_ASSERT(ref().myValue == 2);

			mg::common::Callback<const UTCallbackValue1&()> refConst(
				[&]()->const UTCallbackValue1& {
					return v1.GetRefConst();
				}
			);
			MG_COMMON_ASSERT(refConst().myValue == 2);
			v1.myValue = 3;
			MG_COMMON_ASSERT(refConst().myValue == 3);
			UTCallbackValue1::CheckCounters();

			{
				create.Set(&v1, &UTCallbackValue1::CreateSum);
				v1.myValue = 2;
				UTCallbackValue1 tmp(create(2));
				MG_COMMON_ASSERT(tmp.myValue == 4);
				UTCallbackValue1::UseConstrCount(1);
#if IS_COMPILER_MSVC && !IS_CPP_AT_LEAST_17
				// Clang/MSVC19 manages to generate more efficient code
				// without moves and destructions. There is also
				// no a sane explanation, why the hell the VS
				// compiler would need to make 2 copies, and for
				// a lambda doing exactly the same - just 1 copy.
				UTCallbackValue1::UseMoveConstrCount(2);
				UTCallbackValue1::UseDestrCount(2);
#endif
			}
			UTCallbackValue1::UseDestrCount(1);
			UTCallbackValue1::CheckCounters();

			{
				create.Set(&v1, &UTCallbackValue1::CreateSumConst);
				v1.myValue = 3;
				UTCallbackValue1 tmp(create(4));
				MG_COMMON_ASSERT(tmp.myValue == 7);
				UTCallbackValue1::UseConstrCount(1);
#if IS_COMPILER_MSVC && !IS_CPP_AT_LEAST_17
				// Clang/MSVC19 manages to generate more efficient code
				// without moves and destructions.
				UTCallbackValue1::UseMoveConstrCount(2);
				UTCallbackValue1::UseDestrCount(2);
#endif
			}
			UTCallbackValue1::UseDestrCount(1);
			UTCallbackValue1::CheckCounters();

			ref.Set(&v1, &UTCallbackValue1::GetRef);
			v1.myValue = 1;
			MG_COMMON_ASSERT(ref().myValue == 1);
			v1.myValue = 2;
			MG_COMMON_ASSERT(ref().myValue == 2);

			refConst.Set(&v1, &UTCallbackValue1::GetRefConst);
			v1.myValue = 3;
			MG_COMMON_ASSERT(ref().myValue == 3);
			v1.myValue = 4;
			MG_COMMON_ASSERT(ref().myValue == 4);

			// Return an rvalue and keep it by a reference to
			// check dangling references.
			mg::common::Callback<UTCallbackValue1()> copy(
				[&]()->UTCallbackValue1 {
					return UTCallbackValue1(v1.myValue);
				}
			);
			{
				v1.myValue = 5;
				UTCallbackValue1&& tmp = copy();
				MG_COMMON_ASSERT(tmp.myValue == 5);
				UTCallbackValue1::UseConstrCount(1);
#if IS_COMPILER_MSVC && !IS_CPP_AT_LEAST_17
				// Clang/MSVC19 manages to generate more efficient code
				// without moves and destructions.
				UTCallbackValue1::UseMoveConstrCount(1);
				UTCallbackValue1::UseDestrCount(1);
#endif
				UTCallbackValue1::CheckCounters();
			}
			UTCallbackValue1::UseDestrCount(1);
			UTCallbackValue1::CheckCounters();

			copy.Set(&v1, &UTCallbackValue1::Copy);
			{
				v1.myValue = 6;
				UTCallbackValue1&& tmp = copy();
				MG_COMMON_ASSERT(tmp.myValue == 6);
				UTCallbackValue1::UseConstrCount(1);
#if IS_COMPILER_MSVC && !IS_CPP_AT_LEAST_17
				// Clang/MSVC19 manages to generate more efficient code
				// without moves and destructions.
				UTCallbackValue1::UseMoveConstrCount(2);
				UTCallbackValue1::UseDestrCount(2);
#endif
				UTCallbackValue1::CheckCounters();
			}
			UTCallbackValue1::UseDestrCount(1);
			UTCallbackValue1::CheckCounters();

			copy.Set(&v1, &UTCallbackValue1::CopyConst);
			{
				v1.myValue = 7;
				UTCallbackValue1&& tmp = copy();
				MG_COMMON_ASSERT(tmp.myValue == 7);
				UTCallbackValue1::UseConstrCount(1);
#if IS_COMPILER_MSVC && !IS_CPP_AT_LEAST_17
				// Clang/MSVC19 manages to generate more efficient code
				// without moves and destructions.
				UTCallbackValue1::UseMoveConstrCount(2);
				UTCallbackValue1::UseDestrCount(2);
#endif
				UTCallbackValue1::CheckCounters();
			}
			UTCallbackValue1::UseDestrCount(1);
			UTCallbackValue1::CheckCounters();
		}
		UTCallbackValue1::UseDestrCount(1);
		UTCallbackValue1::CheckCounters();
	}

	struct UTCalllbackVirtValueBase
	{
		virtual ~UTCalllbackVirtValueBase() = default;

		virtual int Sum(
			int aArg) = 0;

		int myValue;
	};

	struct UTCalllbackVirtValueDerived
		: public UTCalllbackVirtValueBase
	{
		int
		Sum(
			int aArg) override
		{
			return myValue + aArg;
		}
	};

	static void
	UnitTestCallbackVirtualMethod()
	{
		UTCalllbackVirtValueDerived d;
		d.myValue = 1;
		mg::common::Callback<int(int)> sum(&d, &UTCalllbackVirtValueDerived::Sum);
		MG_COMMON_ASSERT(sum(2) == 3);

		UTCalllbackVirtValueBase* b = &d;
		sum.Set(b, &UTCalllbackVirtValueBase::Sum);
		MG_COMMON_ASSERT(sum(3) == 4);
	}

	static void
	UnitTestCallbackMethodCopy()
	{
		{
			UTCallbackValue1 v1(0);
			UTCallbackValue1::ResetCounters();
			mg::common::Callback<int(int, int)> sum(&v1, &UTCallbackValue1::Sum);
			MG_COMMON_ASSERT(sum(1, 2) == 3);

			// Check constructor copy works.
			{
				mg::common::Callback<int(int, int)> tmp(sum);
				MG_COMMON_ASSERT(sum(1, 2) == 3);
				v1.myValue = 1;
				MG_COMMON_ASSERT(sum(1, 2) == 4);
			}

			// Copy via Set.
			mg::common::Callback<int(int, int)> copy;
			copy.Set(sum);
			MG_COMMON_ASSERT(sum(1, 2) == 4);
			v1.myValue = 2;
			MG_COMMON_ASSERT(sum(1, 2) == 5);

			// Move constructor works.
			{
				mg::common::Callback<int(int, int)> tmp(mg::common::Move(sum));
				MG_COMMON_ASSERT(sum(1, 2) == 5);
				v1.myValue = 0;
				MG_COMMON_ASSERT(sum(1, 2) == 3);
			}

			// Move via Set.
			copy.Set(mg::common::Move(sum));
			MG_COMMON_ASSERT(sum(1, 2) == 3);
			v1.myValue = 1;
			MG_COMMON_ASSERT(sum(1, 2) == 4);

			//
			// Check all the same for const method.
			//

			sum.Set((const UTCallbackValue1*)&v1, &UTCallbackValue1::SumConst);
			v1.myValue = 0;
			MG_COMMON_ASSERT(sum(1, 2) == 3);

			// Check constructor copy works.
			{
				mg::common::Callback<int(int, int)> tmp(sum);
				MG_COMMON_ASSERT(sum(1, 2) == 3);
				v1.myValue = 1;
				MG_COMMON_ASSERT(sum(1, 2) == 4);
			}

			// Copy via Set.
			copy.Set(sum);
			MG_COMMON_ASSERT(sum(1, 2) == 4);
			v1.myValue = 2;
			MG_COMMON_ASSERT(sum(1, 2) == 5);

			// Move constructor works.
			{
				mg::common::Callback<int(int, int)> tmp(mg::common::Move(sum));
				MG_COMMON_ASSERT(sum(1, 2) == 5);
				v1.myValue = 0;
				MG_COMMON_ASSERT(sum(1, 2) == 3);
			}

			// Move via Set.
			copy.Set(mg::common::Move(sum));
			MG_COMMON_ASSERT(sum(1, 2) == 3);
			v1.myValue = 1;
			MG_COMMON_ASSERT(sum(1, 2) == 4);
		}
		UTCallbackValue1::UseDestrCount(1);
		UTCallbackValue1::CheckCounters();
	}

	static void
	UnitTestCallbackLambdaTransitions()
	{
		// Check all transitions from Lambda to anything else via
		// constructor and Set.
		//
		// Construct Lambda -> Set Method
		// Construct Lambda -> Method Const
		// Construct Lambda -> Lambda
		// Construct empty  -> Set Lambda -> Set Method
		// Construct empty  -> Set Lambda -> Set Method Const
		// Construct empty  -> Set Lambda -> Set Lambda
		{
			UTCallbackValue1 v1(1);
			UTCallbackValue1::ResetCounters();

			// Construct Lambda, Set Method.
			{
				mg::common::Callback<int()> get([=]() -> int { return v1.myValue; });
				UTCallbackValue1::UseCopyConstrCount(1);
				UTCallbackValue1::UseMoveConstrCount(1);
				UTCallbackValue1::UseDestrCount(1);
				// Ensure that the value is copied. So its change
				// here will **not** affect the callback.
				v1.myValue = 0;
				MG_COMMON_ASSERT(get() == 1);

				get.Set(&v1, &UTCallbackValue1::Get);
				UTCallbackValue1::UseDestrCount(1);
				// Ensure that the value is stored by pointer. So
				// its change here will affect the callback.
				v1.myValue = 2;
				MG_COMMON_ASSERT(get() == 2);
			}
			UTCallbackValue1::CheckCounters();

			// Construct empty, Set Lambda, Set Method.
			{
				mg::common::Callback<int()> get;
				get.Set([=]() -> int { return v1.myValue; });
				UTCallbackValue1::UseCopyConstrCount(1);
				UTCallbackValue1::UseMoveConstrCount(1);
				UTCallbackValue1::UseDestrCount(1);
				// Ensure that the value is copied. So its change
				// here will **not** affect the callback.
				v1.myValue = 3;
				MG_COMMON_ASSERT(get() == 2);

				get.Set(&v1, &UTCallbackValue1::Get);
				UTCallbackValue1::UseDestrCount(1);
				// Ensure that the value is stored by pointer. So
				// its change here will affect the callback.
				v1.myValue = 2;
				MG_COMMON_ASSERT(get() == 2);
			}
			UTCallbackValue1::CheckCounters();

			// Construct Lambda, Set Method const.
			{
				mg::common::Callback<int()> get([=]() -> int { return v1.myValue; });
				UTCallbackValue1::ResetCounters();

				get.Set((const UTCallbackValue1*)&v1, &UTCallbackValue1::GetConst);
				UTCallbackValue1::UseDestrCount(1);
				// Ensure that the value is stored by pointer. So
				// its change here will affect the callback.
				v1.myValue = 3;
				MG_COMMON_ASSERT(get() == 3);
			}
			UTCallbackValue1::CheckCounters();

			// Construct empty, Set Lambda, Set Method Const.
			{
				mg::common::Callback<int()> get;
				get.Set([=]() -> int { return v1.myValue; });
				UTCallbackValue1::ResetCounters();

				get.Set((const UTCallbackValue1*)&v1, &UTCallbackValue1::GetConst);
				UTCallbackValue1::UseDestrCount(1);
				// Ensure that the value is stored by pointer. So
				// its change here will affect the callback.
				v1.myValue = 2;
				MG_COMMON_ASSERT(get() == 2);
			}
			UTCallbackValue1::CheckCounters();

			// Construct Lambda, Set Lambda.
			{
				mg::common::Callback<int()> get([=]() -> int { return v1.myValue; });
				UTCallbackValue1::ResetCounters();

				get.Set([=]() -> int { return v1.myValue + 1; });
				UTCallbackValue1::UseMoveConstrCount(1);
				UTCallbackValue1::UseCopyConstrCount(1);
				UTCallbackValue1::UseDestrCount(2);
				// Ensure that the value is copied. So its change
				// here will **not** affect the callback.
				v1.myValue = 4;
				MG_COMMON_ASSERT(get() == 3);
			}
			UTCallbackValue1::UseDestrCount(1);
			UTCallbackValue1::CheckCounters();

			// Construct empty, Set Lambda, Set Lambda.
			{
				mg::common::Callback<int()> get;
				get.Set([=]() -> int { return v1.myValue; });
				UTCallbackValue1::ResetCounters();

				get.Set([=]() -> int { return v1.myValue + 1; });
				UTCallbackValue1::UseMoveConstrCount(1);
				UTCallbackValue1::UseCopyConstrCount(1);
				UTCallbackValue1::UseDestrCount(2);
				// Ensure that the value is copied. So its change
				// here will **not** affect the callback.
				v1.myValue = 6;
				MG_COMMON_ASSERT(get() == 5);
			}
			UTCallbackValue1::UseDestrCount(1);
			UTCallbackValue1::CheckCounters();
		}
		UTCallbackValue1::UseDestrCount(1);
		UTCallbackValue1::CheckCounters();
	}

	static void
	UnitTestCallbackLambdaAssign()
	{
		// Assign to an empty callback.
		mg::common::Callback<int(int, int)> sum;
		sum = [=](int aA, int aB) -> int { return aA + aB + 1; };
		MG_COMMON_ASSERT(sum(1, 2) == 4);

		// Assign to a non-empty callback.
		sum = [=](int aA, int aB) -> int { return aA + aB + 2; };
		MG_COMMON_ASSERT(sum(1, 2) == 5);

		// Assign not to a lambda.
		sum = UTCallbackSum;
		MG_COMMON_ASSERT(sum(1, 2) == 3);
	}

	static void
	UnitTestCallbackMethodTransitions()
	{
		// Check all transitions from Method to anything else via
		// constructor and Set.
		//
		// Construct Method -> Set Method
		// Construct empty  -> Set Method -> Set Method
		// Construct Method -> Method Const
		// Construct empty  -> Set Method -> Set Method Const
		// Construct Method -> Lambda
		// Construct empty  -> Set Method -> Set Lambda
		{
			UTCallbackValue1 v1(1);
			UTCallbackValue1::ResetCounters();

			// Construct Method, Set Method.
			{
				// Ensure that the value is stored by pointer. So
				// its change here will affect the callback.

				mg::common::Callback<int()> get(&v1, &UTCallbackValue1::Get);
				v1.myValue = 0;
				MG_COMMON_ASSERT(get() == 0);

				get.Set(&v1, &UTCallbackValue1::GetPlus);
				v1.myValue = 2;
				MG_COMMON_ASSERT(get() == 3);
			}
			UTCallbackValue1::CheckCounters();

			// Construct empty, Set Method, Set Method.
			{
				// Ensure that the value is stored by pointer. So
				// its change here will affect the callback.

				mg::common::Callback<int()> get;
				get.Set(&v1, &UTCallbackValue1::Get);
				v1.myValue = 0;
				MG_COMMON_ASSERT(get() == 0);

				get.Set(&v1, &UTCallbackValue1::GetPlus);
				v1.myValue = 3;
				MG_COMMON_ASSERT(get() == 4);
			}
			UTCallbackValue1::CheckCounters();

			// Construct Method, Set Method Const.
			{
				// Ensure that the value is stored by pointer. So
				// its change here will affect the callback.

				mg::common::Callback<int()> get(&v1, &UTCallbackValue1::Get);
				v1.myValue = 0;
				MG_COMMON_ASSERT(get() == 0);

				get.Set(&v1, &UTCallbackValue1::GetPlusConst);
				v1.myValue = 4;
				MG_COMMON_ASSERT(get() == 5);
			}
			UTCallbackValue1::CheckCounters();

			// Construct empty, Set Method, Set Method Const.
			{
				// Ensure that the value is stored by pointer. So
				// its change here will affect the callback.

				mg::common::Callback<int()> get;
				get.Set(&v1, &UTCallbackValue1::Get);
				v1.myValue = 0;
				MG_COMMON_ASSERT(get() == 0);

				get.Set(&v1, &UTCallbackValue1::GetPlusConst);
				v1.myValue = 5;
				MG_COMMON_ASSERT(get() == 6);
			}
			UTCallbackValue1::CheckCounters();

			// Construct Method, Set Lambda.
			{
				mg::common::Callback<int()> get(&v1, &UTCallbackValue1::Get);
				// Ensure that the value is stored by pointer. So
				// its change here will affect the callback.
				v1.myValue = 0;
				MG_COMMON_ASSERT(get() == 0);
				v1.myValue = 5;

				get.Set([=]() -> int { return v1.myValue + 1; });
				UTCallbackValue1::UseMoveConstrCount(1);
				UTCallbackValue1::UseCopyConstrCount(1);
				UTCallbackValue1::UseDestrCount(1);
				// Ensure that the value is copied. So its change
				// here will **not** affect the callback.
				v1.myValue = 7;
				MG_COMMON_ASSERT(get() == 6);
			}
			UTCallbackValue1::UseDestrCount(1);
			UTCallbackValue1::CheckCounters();

			// Construct empty, Set Method, Set Lambda.
			{
				mg::common::Callback<int()> get;
				get.Set(&v1, &UTCallbackValue1::Get);
				// Ensure that the value is stored by pointer. So
				// its change here will affect the callback.
				v1.myValue = 6;
				MG_COMMON_ASSERT(get() == 6);
				v1.myValue = 7;

				get.Set([=]() -> int { return v1.myValue + 1; });
				UTCallbackValue1::UseMoveConstrCount(1);
				UTCallbackValue1::UseCopyConstrCount(1);
				UTCallbackValue1::UseDestrCount(1);
				// Ensure that the value is copied. So its change
				// here will **not** affect the callback.
				v1.myValue = 9;
				MG_COMMON_ASSERT(get() == 8);
			}
			UTCallbackValue1::UseDestrCount(1);
			UTCallbackValue1::CheckCounters();
		}
		UTCallbackValue1::UseDestrCount(1);
		UTCallbackValue1::CheckCounters();
	}

	static void
	UnitTestCallbackMethodConstTransitions()
	{
		// Check all transitions from a constant Method to
		// anything else via constructor and Set.
		//
		// Construct Method Const -> Set Method
		// Construct empty  -> Set Method Const -> Set Method
		// Construct Method Const -> Method Const
		// Construct empty  -> Set Method Const -> Set Method Const
		// Construct Method Const -> Lambda
		// Construct empty  -> Set Method Const -> Set Lambda
		{
			UTCallbackValue1 v1(1);
			UTCallbackValue1::ResetCounters();

			// Construct Method Const, Set Method.
			{
				// Ensure that the value is stored by pointer. So
				// its change here will affect the callback.

				mg::common::Callback<int()> get(&v1, &UTCallbackValue1::GetConst);
				v1.myValue = 0;
				MG_COMMON_ASSERT(get() == 0);

				get.Set(&v1, &UTCallbackValue1::GetPlus);
				v1.myValue = 2;
				MG_COMMON_ASSERT(get() == 3);
			}
			UTCallbackValue1::CheckCounters();

			// Construct empty, Set Method Const, Set Method.
			{
				// Ensure that the value is stored by pointer. So
				// its change here will affect the callback.

				mg::common::Callback<int()> get;
				get.Set(&v1, &UTCallbackValue1::GetConst);
				v1.myValue = 0;
				MG_COMMON_ASSERT(get() == 0);

				get.Set(&v1, &UTCallbackValue1::GetPlus);
				v1.myValue = 3;
				MG_COMMON_ASSERT(get() == 4);
			}
			UTCallbackValue1::CheckCounters();

			// Construct Method Const, Set Method Const.
			{
				// Ensure that the value is stored by pointer. So
				// its change here will affect the callback.

				mg::common::Callback<int()> get(&v1, &UTCallbackValue1::GetConst);
				v1.myValue = 0;
				MG_COMMON_ASSERT(get() == 0);

				get.Set(&v1, &UTCallbackValue1::GetPlusConst);
				v1.myValue = 4;
				MG_COMMON_ASSERT(get() == 5);
			}
			UTCallbackValue1::CheckCounters();

			// Construct empty, Set Method Const, Set Method Const.
			{
				// Ensure that the value is stored by pointer. So
				// its change here will affect the callback.

				mg::common::Callback<int()> get;
				get.Set(&v1, &UTCallbackValue1::GetConst);
				v1.myValue = 0;
				MG_COMMON_ASSERT(get() == 0);

				get.Set(&v1, &UTCallbackValue1::GetPlusConst);
				v1.myValue = 5;
				MG_COMMON_ASSERT(get() == 6);
			}
			UTCallbackValue1::CheckCounters();

			// Construct Method Const, Set Lambda.
			{
				mg::common::Callback<int()> get(&v1, &UTCallbackValue1::GetConst);
				// Ensure that the value is stored by pointer. So
				// its change here will affect the callback.
				v1.myValue = 0;
				MG_COMMON_ASSERT(get() == 0);
				v1.myValue = 5;

				get.Set([=]() -> int { return v1.myValue + 1; });
				UTCallbackValue1::UseMoveConstrCount(1);
				UTCallbackValue1::UseCopyConstrCount(1);
				UTCallbackValue1::UseDestrCount(1);
				// Ensure that the value is copied. So its change
				// here will **not** affect the callback.
				v1.myValue = 7;
				MG_COMMON_ASSERT(get() == 6);
			}
			UTCallbackValue1::UseDestrCount(1);
			UTCallbackValue1::CheckCounters();

			// Construct empty, Set Method Const, Set Lambda.
			{
				mg::common::Callback<int()> get;
				get.Set(&v1, &UTCallbackValue1::GetConst);
				// Ensure that the value is stored by pointer. So
				// its change here will affect the callback.
				v1.myValue = 0;
				MG_COMMON_ASSERT(get() == 0);
				v1.myValue = 7;

				get.Set([=]() -> int { return v1.myValue + 1; });
				UTCallbackValue1::UseMoveConstrCount(1);
				UTCallbackValue1::UseCopyConstrCount(1);
				UTCallbackValue1::UseDestrCount(1);
				// Ensure that the value is copied. So its change
				// here will **not** affect the callback.
				v1.myValue = 9;
				MG_COMMON_ASSERT(get() == 8);
			}
			UTCallbackValue1::UseDestrCount(1);
			UTCallbackValue1::CheckCounters();
		}
		UTCallbackValue1::UseDestrCount(1);
		UTCallbackValue1::CheckCounters();
	}

	static void
	UnitTestCallbackImplicit()
	{
		UTCallbackValue1 val(1);
		auto lamCopy = [](mg::common::Callback<int(int, int)> aCb) -> int {
			return aCb(2, 3);
		};
		auto lamConstRef = [](const mg::common::Callback<int(int, int)>& aCb) -> bool {
			return aCb.IsEmpty();
		};
		auto lamMove = [](mg::common::Callback<int(int, int)>&& aCb) -> int {
			return aCb(3, 4);
		};

		// Mutable methods implicit callback.
		MG_COMMON_ASSERT(lamCopy({&val, &UTCallbackValue1::Sum}) == 6);
		MG_COMMON_ASSERT(!lamConstRef({&val, &UTCallbackValue1::Sum}));
		MG_COMMON_ASSERT(lamMove({&val, &UTCallbackValue1::Sum}) == 8);

		// Const methods implicit callback.
		MG_COMMON_ASSERT(lamCopy({&val, &UTCallbackValue1::SumConst}) == 6);
		MG_COMMON_ASSERT(!lamConstRef({&val, &UTCallbackValue1::SumConst}));
		MG_COMMON_ASSERT(lamMove({&val, &UTCallbackValue1::SumConst}) == 8);

		// Functions implicit callback.
		MG_COMMON_ASSERT(lamCopy(&UTCallbackSum) == 5);
		MG_COMMON_ASSERT(!lamConstRef(&UTCallbackSum));
		MG_COMMON_ASSERT(lamMove(&UTCallbackSum) == 7);

		// Functor implicit callback.
		UTCallbackValue1::ResetCounters();

		MG_COMMON_ASSERT(lamCopy(val) == 6);
		UTCallbackValue1::UseCopyConstrCount(1);
		UTCallbackValue1::UseDestrCount(1);

		MG_COMMON_ASSERT(!lamConstRef(val));
		UTCallbackValue1::UseCopyConstrCount(1);
		UTCallbackValue1::UseDestrCount(1);

		MG_COMMON_ASSERT(lamMove(val) == 8);
		UTCallbackValue1::UseCopyConstrCount(1);
		UTCallbackValue1::UseDestrCount(1);

		UTCallbackValue1::CheckCounters();

		// See if forwarding works - it does not. But it is fine.
		// aCb object must be created since it has a name - it is
		// an lvalue then. The complier can't skip its creation.
		mg::common::Callback<int(int, int)>* cbp = nullptr;
		auto lamNewMove = [&](mg::common::Callback<int(int, int)>&& aCb) -> void {
			cbp = new mg::common::Callback<int(int, int)>(mg::common::Move(aCb));
		};
		lamNewMove(val);
		MG_COMMON_ASSERT((*cbp)(1, 2) == 4);
		UTCallbackValue1::UseCopyConstrCount(1);
		UTCallbackValue1::UseMoveConstrCount(1);
		UTCallbackValue1::UseDestrCount(1);
		delete cbp;
		UTCallbackValue1::UseDestrCount(1);
		UTCallbackValue1::CheckCounters();
	}

	static void
	UnitTestCallbackPlainFunction()
	{
		// Set by value.
		{
			mg::common::Callback<int(int, int)> sum;
			sum.Set(UTCallbackSum);
			MG_COMMON_ASSERT(!sum.IsEmpty());
			MG_COMMON_ASSERT(sum(1, 2) == 3);
		}
		// Set by pointer.
		{
			mg::common::Callback<int(int, int)> sum;
			sum.Set(&UTCallbackSum);
			MG_COMMON_ASSERT(!sum.IsEmpty());
			MG_COMMON_ASSERT(sum(1, 2) == 3);
		}
		// Construct by value.
		{
			mg::common::Callback<int(int, int)> sum(UTCallbackSum);
			MG_COMMON_ASSERT(!sum.IsEmpty());
			MG_COMMON_ASSERT(sum(1, 2) == 3);
		}
		// Construct by pointer.
		{
			mg::common::Callback<int(int, int)> sum(&UTCallbackSum);
			MG_COMMON_ASSERT(!sum.IsEmpty());
			MG_COMMON_ASSERT(sum(1, 2) == 3);
		}
		// Assign by value.
		{
			mg::common::Callback<int(int, int)> sum;
			sum = UTCallbackSum;
			MG_COMMON_ASSERT(!sum.IsEmpty());
			MG_COMMON_ASSERT(sum(1, 2) == 3);
		}
		// Assign by pointer.
		{
			mg::common::Callback<int(int, int)> sum;
			sum = &UTCallbackSum;
			MG_COMMON_ASSERT(!sum.IsEmpty());
			MG_COMMON_ASSERT(sum(1, 2) == 3);
		}
	}

	static void
	UnitTestCallbackAssignCopy()
	{
		// Empty = empty.
		{
			mg::common::Callback<int(int, int)> sum1;
			mg::common::Callback<int(int, int)> sum2;
			sum1 = sum2;
			MG_COMMON_ASSERT(sum1.IsEmpty());
			MG_COMMON_ASSERT(sum2.IsEmpty());
		}
		// Empty = non-empty.
		{
			mg::common::Callback<int(int, int)> sum1;
			mg::common::Callback<int(int, int)> sum2(UTCallbackValue1(1));
			UTCallbackValue1::ResetCounters();
			sum1 = sum2;
			MG_COMMON_ASSERT(!sum1.IsEmpty());
			MG_COMMON_ASSERT(!sum2.IsEmpty());
			UTCallbackValue1::UseCopyConstrCount(1);
			MG_COMMON_ASSERT(sum1(1, 2) == 4);
			MG_COMMON_ASSERT(sum2(1, 2) == 4);
		}
		UTCallbackValue1::UseDestrCount(2);
		UTCallbackValue1::CheckCounters();
		// Non-empty = empty.
		{
			mg::common::Callback<int(int, int)> sum1(UTCallbackValue1(1));
			mg::common::Callback<int(int, int)> sum2;
			UTCallbackValue1::ResetCounters();
			sum1 = sum2;
			MG_COMMON_ASSERT(sum1.IsEmpty());
			MG_COMMON_ASSERT(sum2.IsEmpty());
			UTCallbackValue1::UseDestrCount(1);
		}
		UTCallbackValue1::CheckCounters();
		// Non-empty = non-empty.
		{
			mg::common::Callback<int(int, int)> sum1(UTCallbackValue1(1));
			mg::common::Callback<int(int, int)> sum2(UTCallbackValue2(2));
			UTCallbackValue1::ResetCounters();
			UTCallbackValue2::ResetCounters();
			sum1 = sum2;
			MG_COMMON_ASSERT(sum1(1, 2) == 5);
			MG_COMMON_ASSERT(sum2(1, 2) == 5);
			UTCallbackValue1::UseDestrCount(1);
			UTCallbackValue2::UseCopyConstrCount(1);
		}
		UTCallbackValue2::UseDestrCount(2);
		UTCallbackValue1::CheckCounters();
		UTCallbackValue2::CheckCounters();
	}

	static void
	UnitTestCallbackAssignMove()
	{
		// Empty = empty.
		{
			mg::common::Callback<int(int, int)> sum1;
			mg::common::Callback<int(int, int)> sum2;
			sum1 = mg::common::Move(sum2);
			MG_COMMON_ASSERT(sum1.IsEmpty());
			MG_COMMON_ASSERT(sum2.IsEmpty());
		}
		// Empty = non-empty.
		{
			mg::common::Callback<int(int, int)> sum1;
			mg::common::Callback<int(int, int)> sum2(UTCallbackValue1(1));
			UTCallbackValue1::ResetCounters();
			sum1 = mg::common::Move(sum2);
			MG_COMMON_ASSERT(sum1(1, 2) == 4);
			MG_COMMON_ASSERT(sum2(1, 2) == 4);
			UTCallbackValue1::UseMoveConstrCount(1);
		}
		UTCallbackValue1::UseDestrCount(2);
		UTCallbackValue1::CheckCounters();
		// Non-empty = empty.
		{
			mg::common::Callback<int(int, int)> sum1(UTCallbackValue1(1));
			mg::common::Callback<int(int, int)> sum2;
			UTCallbackValue1::ResetCounters();
			sum1 = mg::common::Move(sum2);
			MG_COMMON_ASSERT(sum1.IsEmpty());
			MG_COMMON_ASSERT(sum2.IsEmpty());
			UTCallbackValue1::UseDestrCount(1);
		}
		UTCallbackValue1::CheckCounters();
		// Non-empty = non-empty.
		{
			mg::common::Callback<int(int, int)> sum1(UTCallbackValue1(1));
			mg::common::Callback<int(int, int)> sum2(UTCallbackValue2(2));
			UTCallbackValue1::ResetCounters();
			UTCallbackValue2::ResetCounters();
			sum1 = mg::common::Move(sum2);
			MG_COMMON_ASSERT(sum1(1, 2) == 5);
			MG_COMMON_ASSERT(sum2(1, 2) == 5);
			UTCallbackValue1::UseDestrCount(1);
			UTCallbackValue2::UseMoveConstrCount(1);
		}
		UTCallbackValue2::UseDestrCount(2);
		UTCallbackValue1::CheckCounters();
		UTCallbackValue2::CheckCounters();
	}

	static void
	UnitTestCallbackArray()
	{
		// Should be able to store the callbacks in an array like
		// std::function allows.
		{
			mg::common::HybridArray<mg::common::Callback<int(int, int)>, 4> arr;
			UTCallbackValue1 cv1(1);
			UTCallbackValue2 cv2(2);
			UTCallbackValue1::ResetCounters();
			UTCallbackValue2::ResetCounters();

			arr.Add(UTCallbackSum);
			arr.Add(&UTCallbackSum);

			arr.Add(UTCallbackValue2(3));
			// Implicitly created callback.
			UTCallbackValue2::UseConstrCount(1);
			UTCallbackValue2::UseMoveConstrCount(1);
			UTCallbackValue2::UseDestrCount(1);
			// Move into the array.
			UTCallbackValue2::UseMoveConstrCount(1);
			// The temporary value from which the implicit
			// callback was created.
			UTCallbackValue2::UseDestrCount(1);

			arr.Add({&cv1, &UTCallbackValue1::Sum});

			arr.Add({&cv2, &UTCallbackValue2::SumConst});
			// Realloc has happened.
			UTCallbackValue2::UseMoveConstrCount(1);
			UTCallbackValue2::UseDestrCount(1);

			MG_COMMON_ASSERT(arr[0](1, 2) == 3);
			MG_COMMON_ASSERT(arr[1](1, 2) == 3);
			MG_COMMON_ASSERT(arr[2](1, 2) == 6);
			MG_COMMON_ASSERT(arr[3](1, 2) == 4);
			MG_COMMON_ASSERT(arr[4](1, 2) == 5);
		}
		UTCallbackValue1::UseDestrCount(1);
		UTCallbackValue2::UseDestrCount(2);
		UTCallbackValue1::CheckCounters();
		UTCallbackValue2::CheckCounters();
	}

	void
	UnitTestCallback()
	{
		TestSuiteGuard suite("Callback");

		UnitTestCallbackBasic();
		UnitTestCallbackCallableClass();
		UnitTestCallbackCallableClassAssign();
		UnitTestCallbackLambda();
		UnitTestCallbackLambdaArgumentForward();
		UnitTestCallbackLambdaTransitions();
		UnitTestCallbackLambdaAssign();
		UnitTestCallbackMethodArgumentForward();
		UnitTestCallbackMethodCopy();
		UnitTestCallbackMethodTransitions();
		UnitTestCallbackMethodConstTransitions();
		UnitTestCallbackReturnForward();
		UnitTestCallbackVirtualMethod();
		UnitTestCallbackImplicit();
		UnitTestCallbackPlainFunction();
		UnitTestCallbackAssignCopy();
		UnitTestCallbackAssignMove();
		UnitTestCallbackArray();
	}

}
}
