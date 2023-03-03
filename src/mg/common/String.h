#pragma once

#include "mg/common/Assert.h"
#include "mg/common/StringFunctions.h"
#include "mg/common/Util.h"

namespace mg {
namespace common {

	// IString is an interface, it can't have an instance. Only
	// children. However compiler still adds padding for members
	// of this class. It has one pointer (8 byte), one uint32_t
	// (4 bytes). Therefore its alignment is 8 and total member
	// size 12. Compiler adds 4 byte padding in the end to make
	// the size aligned by 8.
	// But it is not needed - the only possible child class,
	// HybridString, stores an array of chars whose alignment
	// is 1. So in total:
	//
	// - IString can't be created, and of course can't be stored
	//   in an array (so end padding is not needed);
	//
	// - Its only child class does not need alignment at all.
	//
	// With the padding of IString, HybridString becomes at least
	// 4 bytes bigger. To make it packed better, IString gets 4
	// byte alignment, so now it does not have unused memory in
	// the end. HybridString explicitly gets 8 byte alignment, so
	// any instance of IString becomes implicitly aligned by 8
	// bytes, without excess 4 bytes.
	#pragma pack(push, 4)

	class IString
	{
	protected:
		// The constructor is protected, because the class is not
		// supposed to be used on its own - it should be
		// inherited.
		IString(
			char* aData,
			uint32_t aSize);

		// Destructor is not virtual to avoid vtable creation. The
		// string can't be deleted by the base pointer - it would
		// be undefined behaviour. Hence it is protected.
		// In case it will be ever needed, that can be allowed by
		// overloading new/delete to use malloc/free, because they
		// have well defined behaviour, and don't depend on class
		// hierarchies.
		~IString();

	public:
		// Access ////////////////////////////////////////////////

		char* GetBuffer();

		const char* GetBuffer() const;

		uint32_t GetBufferSize() const;

		uint32_t GetLength() const;

		operator const char* () const;

		char& operator[](
			int aIndex);

		char& operator[](
			unsigned aIndex);

		const char& operator[](
			int aIndex) const;

		const char& operator[](
			unsigned int aIndex) const;

		// Checks ////////////////////////////////////////////////

		bool IsEmpty() const;

		bool IsDynamic() const;

		// Modification //////////////////////////////////////////

		void Clear();

		void Reserve(
			uint32_t aLength);

		void SetLength(
			uint32_t aLength);

		void Set(
			const char* aString);

		void Set(
			const char* aString,
			uint32_t aLength);

		void Append(
			const char* aString);

		MG_STRFORMAT_PRINTF(2, 3)
		IString& AppendFormat(
			const char* aFormatString,
			...);

		MG_STRFORMAT_PRINTF(2, 0)
		IString& AppendVFormat(
			const char* aFormatString,
			va_list aParams);

		MG_STRFORMAT_PRINTF(2, 0)
		IString& VFormat(
			const char* aFormatString,
			va_list aParams);

		MG_STRFORMAT_PRINTF(2, 3)
		IString& Format(
			const char* aFormatString,
			...);

		IString& operator=(
			const char* aString);

		IString& operator=(
			const IString& aString);

		IString& operator+=(
			const char* aString);

	private:
		void PrivRealloc(
			uint32_t aNewSize);

		// IString does not have virtual methods, and therefore
		// does not have vtable - its layout is strictly defined.
		// Also it assumes it is inherited by a non-virtual class
		// with defined layout. Child class should have a static
		// buffer as a first member, so IString could get it by
		// looking at the end address of self.
		// It is valid to assume that the child class' members
		// will follow IString in memory, because
		//
		//   - No vtable;
		//
		//   - Pointer at the child class should be safe to cast
		//     to IString, which means IString members will go
		//     first, for sure.

		char* PrivGetBufferStatic();

		const char* PrivGetBufferStatic() const;

	protected:
		char* myData;
		uint32_t mySize;
		// In case it will be ever needed to inherit a new class
		// from IString, with no static buffer, or with a static
		// buffer as not a first member, it is possible to add a
		// new member 'const uint32_t myStaticDataOffset;', which
		// would be initialized in the constructor, and would be
		// a byte offset from IString's address to the static
		// buffer location, or 0 when no static buffer. Another
		// option - add a 1 byte flag 'bool myIsDynamic;'.
		// But both solutions would add 4 bytes.
	};
	#pragma pack(pop)

	// 8 bytes - sizeof(myData).
	// 4 bytes - sizeof(mySize).
	static_assert(sizeof(IString) == 12, "IString should not have padding nor vtable");

	//////////////////////////////////////////////////////////////

	template <int S>
	class alignas(alignof(void*)) HybridString final
		: public IString
	{
	public:
		HybridString();

		HybridString(
			const HybridString& aString);

		HybridString(
			const char* aString);

		HybridString& operator=(
			const char* aString);

		HybridString& operator=(
			const HybridString& aString);

	private:
		// Keep at the bottom so as the highly used fields above
		// are in one cache line.
		char myStaticData[S];
	};

	// HybridString should be 8 byte aligned - its base, IString,
	// has a pointer (which needs 8 byte alignment).
	static_assert(alignof(HybridString<1>) == 8, "HybridString should have alignment");

	//////////////////////////////////////////////////////////////

	inline
	IString::IString(
		char* aData,
		uint32_t aSize)
		: myData(aData)
		, mySize(aSize)
	{
		MG_DEV_ASSERT(aSize > 0);
		myData[0] = 0;
	}

	inline
	IString::~IString()
	{
		if (IsDynamic())
			delete[] myData;
	}

	inline char*
	IString::GetBuffer()
	{
		return myData;
	}

	inline const char*
	IString::GetBuffer() const
	{
		return myData;
	}

	inline uint32_t
	IString::GetBufferSize() const
	{
		return mySize;
	}

	inline uint32_t
	IString::GetLength() const
	{
		return mg::common::Strlen(myData);
	}

	inline
	IString::operator const char* () const
	{
		return myData;
	}

	inline bool
	IString::IsEmpty() const
	{
		return *myData == 0;
	}

	inline bool
	IString::IsDynamic() const
	{
		return myData != PrivGetBufferStatic();
	}

	inline void
	IString::Clear()
	{
		*myData = 0;
	}

	inline void
	IString::Reserve(
		uint32_t aLength)
	{
		uint32_t sizeWithTermination = aLength + 1;
		if (mySize < sizeWithTermination)
			PrivRealloc(sizeWithTermination);
	}

	inline void
	IString::SetLength(
		uint32_t aLength)
	{
		Reserve(aLength);
		myData[aLength] = 0;
	}

	MG_STRFORMAT_PRINTF(2, 3)
	inline IString&
	IString::AppendFormat(
		const char* aFormatString,
		...)
	{
		va_list va;
		va_start(va, aFormatString);
		AppendVFormat(aFormatString, va);
		va_end(va);
		return *this;
	}

	MG_STRFORMAT_PRINTF(2, 0)
	inline IString&
	IString::VFormat(
		const char* aFormatString,
		va_list aParams)
	{
		Clear();
		return AppendVFormat(aFormatString, aParams);
	}

	MG_STRFORMAT_PRINTF(2, 3)
	inline IString&
	IString::Format(
		const char* aFormatString,
		...)
	{
		va_list va;
		va_start(va, aFormatString);
		VFormat(aFormatString, va);
		va_end(va);
		return *this;
	}

	inline IString&
	IString::operator=(
		const char* aString)
	{
		Set(aString);
		return *this;
	}

	inline IString&
	IString::operator=(
		const IString& aString)
	{
		Set(aString.GetBuffer());
		return *this;
	}

	inline IString&
	IString::operator+=(
		const char* aString)
	{
		Append(aString);
		return *this;
	}

	inline char*
	IString::PrivGetBufferStatic()
	{
		return (char*) (this + 1);
	}

	inline const char*
	IString::PrivGetBufferStatic() const
	{
		return (const char*) (this + 1);
	}

	//////////////////////////////////////////////////////////////

	template <int S>
	inline
	HybridString<S>::HybridString()
		: IString(myStaticData, S)
	{
		static_assert(offsetof(HybridString, myStaticData) == sizeof(IString),
			"Static data should follow IString");
	}

	template <int S>
	inline
	HybridString<S>::HybridString(
		const HybridString& aString)
		: IString(myStaticData, S)
	{
		*this = aString.GetBuffer();
	}

	template <int S>
	inline
	HybridString<S>::HybridString(
		const char* aString)
		: IString(myStaticData, S)
	{
		*this = aString;
	}

	template <int S>
	inline HybridString<S>&
	HybridString<S>::operator=(
		const char* aString)
	{
		Set(aString);
		return *this;
	}

	template <int S>
	inline HybridString<S>&
	HybridString<S>::operator=(
		const HybridString& aString)
	{
		if (this != &aString)
			Set(aString.GetBuffer());

		return *this;
	}

	//////////////////////////////////////////////////////////////

	static inline bool
	operator==(
		const IString& aString,
		const IString& aString2)
	{
		return mg::common::Strcmp(aString, aString2) == 0;
	}

	static inline bool
	operator==(
		const IString& aString,
		const char* aString2)
	{
		return mg::common::Strcmp(aString, aString2) == 0;
	}

	static inline bool
	operator==(
		const char* aString,
		const IString& aString2)
	{
		return mg::common::Strcmp(aString, aString2) == 0;
	}

	static inline bool
	operator!=(
		const IString& aString,
		const IString& aString2)
	{
		return mg::common::Strcmp(aString, aString2) == 0;
	}

	static inline bool
	operator!=(
		const IString& aString,
		const char* aString2)
	{
		return mg::common::Strcmp(aString, aString2) == 0;
	}

	static inline bool
	operator!=(
		const char* aString,
		const IString& aString2)
	{
		return mg::common::Strcmp(aString, aString2) == 0;
	}

	static inline bool
	operator<(
		const IString& aString,
		const IString& aString2)
	{
		return mg::common::Strcmp(aString, aString2) == 0;
	}

	static inline bool
	operator<(
		const IString& aString,
		const char* aString2)
	{
		return mg::common::Strcmp(aString, aString2) == 0;
	}

	static inline bool
	operator<(
		const char* aString,
		const IString& aString2)
	{
		return mg::common::Strcmp(aString, aString2) == 0;
	}

	static inline bool
	operator>(
		const IString& aString,
		const IString& aString2)
	{
		return mg::common::Strcmp(aString, aString2) == 0;
	}

	static inline bool
	operator>(
		const IString& aString,
		const char* aString2)
	{
		return mg::common::Strcmp(aString, aString2) == 0;
	}

	static inline bool
	operator>(
		const char* aString,
		const IString& aString2)
	{
		return mg::common::Strcmp(aString, aString2) == 0;
	}

	static inline bool
	operator<=(
		const IString& aString,
		const IString& aString2)
	{
		return mg::common::Strcmp(aString, aString2) == 0;
	}

	static inline bool
	operator<=(
		const IString& aString,
		const char* aString2)
	{
		return mg::common::Strcmp(aString, aString2) == 0;
	}

	static inline bool
	operator<=(
		const char* aString,
		const IString& aString2)
	{
		return mg::common::Strcmp(aString, aString2) == 0;
	}

	static inline bool
	operator>=(
		const IString& aString,
		const IString& aString2)
	{
		return mg::common::Strcmp(aString, aString2) == 0;
	}

	static inline bool
	operator>=(
		const IString& aString,
		const char* aString2)
	{
		return mg::common::Strcmp(aString, aString2) == 0;
	}

	static inline bool
	operator>=(
		const char* aString,
		const IString& aString2)
	{
		return mg::common::Strcmp(aString, aString2) == 0;
	}

}
}
