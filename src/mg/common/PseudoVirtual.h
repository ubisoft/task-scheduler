#pragma once

#include "mg/common/Definitions.h"

namespace mg {
namespace common {

	// Helper to make a child class deletable by the base pointer
	// without a virtual destructor. This can help to remove
	// vtable and thus make the class 8 bytes smaller and faster.
	// It works when
	//
	//   - Destructor is the only virtual method of the class;
	//
	//   - Child class destructor is trivial. I.e. all child
	//     members are either basic types, or structs consisting
	//     of basic types without custom destructors.
	//
	// C++ standard says that 'delete' on a base class with a
	// non-virtual destructor leads to undefined behaviour. But it
	// is not so for malloc()/free() or if allocation of the
	// underlying buffer is done manually using new[]/delete[] for
	// uint8_t or char, for instance. With manual allocation it does
	// not matter whether it is a base pointer or not - it just
	// needs to be the same pointer as returned by a corresponding
	// malloc()/new[] earlier.
	//
	// This class overloads new/delete to use raw buffers, so
	// a child class can be deleted by the base pointer safely.
	struct PseudoVirtual
	{
		inline void*
		operator new(
			size_t aSize)
		{
			// Use new[]/delete[], not free/malloc. Because
			// new/delete may be redefined on an upper level for
			// memory accounting.
			return new uint8_t[aSize];
		}

		inline void*
		operator new(
			size_t aSize,
			void* aPtr)
		{
			MG_UNUSED(aSize);
			return aPtr;
		}

		inline void
		operator delete(
			void* aPtr)
		{
			delete[] (uint8_t*)aPtr;
		}

		inline void
		operator delete(
			void* aPtr,
			void* aPlace)
		{
			MG_UNUSED(aPtr);
			MG_UNUSED(aPlace);
		}
	};

}
}
