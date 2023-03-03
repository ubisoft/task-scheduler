#pragma once

#include "mg/common/Types.h"

namespace mg {
namespace common {

	// Simplified implementation of std::declval. See the official
	// documentation for details.
	// In short, declval helps to get a value of type T inside
	// templates, but never actually create the value. All declval
	// expressions, being inside a template, are evaluated by the
	// compiler, but never result into binary code, nor anything
	// else runtime specific. An attempt to use declval in runtime
	// code will result into undefined reference, since the
	// 'function' is not defined anywhere.
	template<typename T>
	T&& Declval();

	// The same as std::enable_if. See the official documentation
	// for details.
	// In short, helps to delete template specifications which
	// do not conform to the given condition.
	template<bool Condition, typename T = void>
	struct EnableIf
	{
	};

	template<typename T>
	struct EnableIf<true, T>
	{
		using Type = T;
	};

	// Evaluates a enum value to true if the given types T1 and
	// T2 are comparable. Has no std:: alternative. Usage example:
	// inside EnableIf() to disable a template function which
	// wants to use T1 == T2 inside.
	template<typename T1, typename T2, typename T3 = bool>
	struct AreComparable
	{
		enum { myValue = false };
	};

	template<typename T1, typename T2>
	struct AreComparable<T1, T2, decltype(Declval<T1>() == Declval<T2>())>
	{
		enum { myValue = true };
	};

	template<typename T>
	struct IsPointer
	{
		enum { myValue = false };
	};

	template<typename T>
	struct IsPointer<T*>
	{
		enum { myValue = true };
	};

	// Trivial type does not have non-default constructors,
	// destructors, assignment operators. The same applies to all
	// members of the type if it is a class or struct,
	// recursively.
	template<typename T>
	struct IsTrivial
	{
		enum { myValue = __is_trivial(T) };
	};

	// Trivial destructor is the one generated automatically and
	// not doing anything. That means all members of the type, if
	// this is a struct or a class, also should have trivial
	// destructors.
	template<typename T>
	struct HasTrivialDestructor
	{
		enum { myValue = __has_trivial_destructor(T) };
	};

	// Trivial constructor is the one generated automatically and
	// not doing anything. That means all members of the type, if
	// this is a struct or a class, also should have trivial
	// constructors.
	template<typename T>
	struct HasTrivialConstructor
	{
		enum { myValue = __has_trivial_constructor(T) };
	};

	template <typename T>
	struct RemoveReference
	{
		typedef T Type;
	};

	template <typename T>
	struct RemoveReference<T&>
	{
		typedef T Type;
	};

	template <typename T>
	struct RemoveReference<T&&>
	{
		typedef T Type;
	};

	template<typename T>
	struct RemoveConstAndReference
	{
		typedef T Type;
	};

	template<typename T>
	struct RemoveConstAndReference<T&>
	{
		typedef T Type;
	};

	template<typename T>
	struct RemoveConstAndReference<T&&>
	{
		typedef T Type;
	};

	template<typename T>
	struct RemoveConstAndReference<const T>
	{
		typedef T Type;
	};

	template<typename T>
	struct RemoveConstAndReference<const T&>
	{
		typedef T Type;
	};

	template<typename T>
	struct RemoveConstAndReference<const T&&>
	{
		typedef T Type;
	};

	template<typename T>
	typename RemoveReference<T>::Type&&
	Move(
		T&& aValue)
	{
		using ReturnType = typename RemoveReference<T>::Type&&;
		return static_cast<ReturnType>(aValue);
	}

	template<typename T>
	inline T&&
	Forward(
		typename RemoveReference<T>::Type& aValue)
	{
		return static_cast<T&&>(aValue);
	}

	template<typename T>
	inline T&&
	Forward(
		typename RemoveReference<T>::Type&& aValue)
	{
		return static_cast<T&&>(aValue);
	}

	template<class L, class R, class T = void>
	struct EnableIfNotSame
	{
		typedef T Type;
	};

	template<class LR, class T>
	struct EnableIfNotSame<LR, LR, T>
	{
	};

	template<class T, class M>
	inline int64_t
	OffsetOf(const M T::*aMemberDecl)
	{
		// This is safe because there is no dereference.
		// &(ptr->member) turns into "ptr + offsetof(member)".
		//
		// The builtin offsetof() is not used, because it raises a
		// warning for any non-POD type. But in fact it is totally
		// fine unless 'virtual inheritance' is used (not the same
		// as virtual methods).
		return (int64_t)(&(((T*)(nullptr))->*aMemberDecl));
	}

	template<class T, class M>
	const T*
	ContainerOf(const M* aMember, const M T::*aMemberDecl)
	{
		return (const T*)((int64_t)aMember - OffsetOf(aMemberDecl));
	}

	template<class T, class M>
	T*
	ContainerOf(M* aMember, const M T::*aMemberDecl)
	{
		return (T*)((int64_t)aMember - OffsetOf(aMemberDecl));
	}

}
}
