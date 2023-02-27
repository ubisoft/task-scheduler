#pragma once

#include "mg/common/Definitions.h"

namespace mg {
namespace common {

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
		return (int64_t) (&(((T*) (nullptr))->*aMemberDecl));
	}

	template<class T, class M>
	const T*
	ContainerOf(const M* aMember, const M T::*aMemberDecl)
	{
		return (const T*) ((int64_t) aMember - OffsetOf(aMemberDecl));
	}

	template<class T, class M>
	T*
	ContainerOf(M* aMember, const M T::*aMemberDecl)
	{
		return (T*) ((int64_t) aMember - OffsetOf(aMemberDecl));
	}

}
}
