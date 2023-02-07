#pragma once

#include "mg/common/Assert.h"
#include "mg/common/TypeTraits.h"

namespace mg {
namespace common {

	// Callback object depends only on a signature - returned type
	// and argument count and types. On the other hand, the object
	// can at runtime decide will it store a method reference, or
	// a lambda function, or a plain function pointer, or a
	// functor. Since the interface is unified, and the content
	// depends on runtime, the only way to make it work is using
	// virtual methods. So the Callback adds +1 virtual call for
	// each invocation and +1 virtual call on destruction (not
	// always).
	//
	// Moreover, the virtual methods should be switchable. So for
	// example if a callback object stores a lambda, it needs to
	// have one vtable to properly destroy it or copy. But the
	// same callback can be assigned to a class method pointer,
	// and then it will have to be able to destroy and copy it as
	// well. That can only be done in the C manner - function
	// pointers, switched together with Callback object changes.
	//
	// Callback object needs too many function pointers to
	// manipulate the callable internals, so they are wrapped into
	// a global vtable per each {signature, callable type}
	// template pair, to save memory.
	//
	template<typename Ret, typename... Args>
	struct CallbackStorageVtab
	{
		// Void pointer points at a raw memory buffer where an
		// actual callable object is saved. Its type is templated,
		// so it is passed as void* to make the vtab depend only
		// on the signature.

		void (*Destroy)(
			void* aThis);

		void (*MoveConstruct)(
			void* aThis,
			void* aOther);

		void (*CopyConstruct)(
			void* aThis,
			const void* aOther);

		// It is essential **not** to return it as a forwarding
		// reference (Ret&&). That leads to dangling references,
		// because reference lifetime is limited to the scope of
		// its creation. So when the same reference is returned
		// more than once via forwarding, it is deleted after a
		// first forward, and the next forwards will operate on
		// garbage. The only alternative is moving the returned
		// values instead of forwarding. It means the callbacks
		// should not return anything heavy - it is going to be
		// moved ~3 times inside the Callback object.
		//
		// Perfect forwarding works only on passing values into a
		// function. Not out of it.
		Ret (*Call)(
			void* aThis,
			Args&&...);

		// Empty vtable with all functions set to null. That
		// allows not to check for vtab being not null inside
		// Callback. Having vtab in Callback always not null makes
		// it a little faster to check if concrete methods are not
		// null. They can be. For example, a method pointer does
		// not need destruction, and is intended to be the most
		// common case. So it saves 1 virtual call on Callback
		// destruction if it stores a method.
		static const CallbackStorageVtab ourEmptyVtab;
	};

	template<typename Ret, typename... Args>
	struct CallbackStorage
	{
		using Vtab = CallbackStorageVtab<Ret, Args...>;

		CallbackStorage();

		~CallbackStorage();

		// Convenient wrappers around vtab so as not to touch it
		// directly when possible, and pass the raw buffer
		// properly.

		void Destroy();

		template<typename Storage, typename... ConstructArgs>
		void Create(
			ConstructArgs&&... aConstructArgs);

		void MoveConstruct(
			CallbackStorage* aOther);

		void CopyConstruct(
			const CallbackStorage* aOther);

		bool IsEmpty() const;

		Ret Call(
			Args&&...);

		const Vtab* myVtab;
		// The buffer stores an actual callable object: a lambda,
		// an object method, a functor, or a raw function pointer.
		// Having it opaque, as a byte buffer, allows to reuse
		// this memory for any kinds of callables at runtime,
		// not exposing their nature.
		// The size is limited so as not to use the heap. If a
		// callable object is bigger than 32 bytes, there is
		// likely something wrong with it.
		uint8 myBuffer[sizeof(void*) * 4];
	};

	//////////////////////////////////////////////////////////////

	// Implementation of a non-const object method invocation.
	// Note, the method pointer can't be saved into the storage's
	// buffer as is, because it may be not just a pointer like for
	// a plain function. It has a special structure, and can't be
	// just cast from a raw buffer safely. For example, this may
	// not compile depending on compiler:
	//
	//     uint8 myBuffer[sizeof(void*) * 4];
	//     Method* meth = (Method*)myBuffer;
	//
	// This is why the mutable method storage wrapper exists. So
	// all the work with the method pointer is done directly
	// without casts.
	//
	template<typename Object, typename Ret, typename... Args>
	struct CallbackStorageMethodMutable
	{
		using Signature = Ret(Object::*)(Args...);
		using Vtab = CallbackStorageVtab<Ret, Args...>;

		CallbackStorageMethodMutable(
			Object* aObject,
			Signature aMethod);

		Object* myObject;
		Signature myMethod;

		static const Vtab ourVtab;
	};

	//////////////////////////////////////////////////////////////

	// All the same as mutable method storage, but with 'const' in
	// the method's signature.
	//
	template<typename Object, typename Ret, typename... Args>
	struct CallbackStorageMethodConst
	{
		using Signature = Ret(Object::*)(Args...) const;
		using Vtab = CallbackStorageVtab<Ret, Args...>;

		CallbackStorageMethodConst(
			const Object* aObject,
			Signature aMethod);

		const Object* myObject;
		Signature myMethod;

		static const Vtab ourVtab;
	};

	//////////////////////////////////////////////////////////////

	// Functor is anything callable - a raw function pointer, a
	// lambda, a class having operator() overloaded. Some of these
	// need proper destructors. In case of lambda - have banned
	// copy constructors and no type defined except via a
	// template. All that makes necessary to wrap everything
	// callable into a struct like this one. To operate on the
	// functor object directly.
	//
	template<typename Func, typename Ret, typename... Args>
	struct CallbackStorageFunctor
	{
		using Vtab = CallbackStorageVtab<Ret, Args...>;

		CallbackStorageFunctor(
			const Func& aFunctor);

		CallbackStorageFunctor(
			Func&& aFunctor);

		CallbackStorageFunctor(
			const CallbackStorageFunctor& aFunctor);

		CallbackStorageFunctor(
			CallbackStorageFunctor&& aFunctor);

		Func myFunctor;

		static const Vtab ourVtab;
	};

	//////////////////////////////////////////////////////////////

	template<typename Signature>
	class Callback;

	template<typename Ret, typename... Args>
	class Callback<Ret(Args...)>
	{
	private:
		using Storage = CallbackStorage<Ret, Args...>;
		using Signature = Ret(*)(Storage* aStorage, Args&&...);

		template<typename T>
		using EnableIfNotCallback = typename EnableIfNotSame<
			typename RemoveConstAndReference<T>::Type, Callback
		>::Type;

		template<typename T>
		using EnableIfNotPlain = typename EnableIfNotSame<
			typename RemoveConstAndReference<T>::Type, Ret(*)(Args...)
		>::Type;

		template<
			typename T,
			typename T2 = EnableIfNotCallback<T>,
			typename T3 = EnableIfNotPlain<T>
		>
		// Use any of T2 or T3 as a result. Otherwise clang simply
		// skips them both and SFINAE does not work.
		using EnableIfNotSpecial = T3;

	public:
		Callback();

		// Use initializer list MyFunc({arg1, arg2}) C++ feature
		// to create the callback implicitly, when you don't want
		// to write MyFunc(mg::common::Callback(arg1, arg2)) each
		// time. Just wrap the callback args into {}.
		template<typename Object>
		Callback(
			Object* aObject,
			Ret(Object::*aMethod)(Args...));

		template<typename Object>
		Callback(
			const Object* aObject,
			Ret(Object::*aMethod)(Args...) const);

		Callback(
			const Callback& aOther);

		Callback(
			Callback&& aOther);

		// Func is a functor type - anything what can be called
		// with operator().
		//
		// Note: the method's argument has &&, but it is not an
		// rvalue. In C++ if a templated type has &&, it creates a
		// new type - 'forwarding reference'. Which can be both
		// rvalue and lvalue.
		//
		// This constructor is disabled for Callback and plain
		// function objects so as to be able to define them
		// separately. Otherwise the forwarding reference will
		// gather all the invocations.
		template<typename Func, typename = EnableIfNotSpecial<Func>>
		Callback(
			Func&& aFunc);

		// Plain functions are handled separately. Because in
		// C/C++ it is possible to pass a function both by "value"
		// and by a pointer. It is the same really, but in C++
		// templates they are different - it is not possible to
		// have a member of type 'function'. It must be
		// specifically a pointer. Hence special handling for
		// functions / function pointers.
		Callback(
			Ret(*aFunc)(Args...));

		void Clear();

		bool IsEmpty() const;

		template<typename Object>
		void Set(
			Object* aObject,
			Ret(Object::*aMethod)(Args...));

		template<typename Object>
		void Set(
			const Object* aObject,
			Ret(Object::*aMethod)(Args...) const);

		void Set(
			const Callback& aOther);

		void Set(
			Callback&& aOther);

		void Set(
			Ret(*aFunc)(Args...));

		template<typename Func, typename = EnableIfNotSpecial<Func>>
		void Set(
			Func&& aFunc);

		template<typename Func, typename = EnableIfNotSpecial<Func>>
		Callback& operator=(
			Func&& aFunc);

		Callback& operator=(
			Callback&& aOther);

		Callback& operator=(
			const Callback& aOther);

		// The call can't be const. If it stores an object and its
		// mutable method, the call would change the object.
		Ret operator()(
			Args... aArgs);

	private:
		Storage myStorage;
	};

	//////////////////////////////////////////////////////////////

	template<typename Ret, typename... Args>
	inline
	CallbackStorage<Ret, Args...>::CallbackStorage()
		: myVtab(&Vtab::ourEmptyVtab)
	{
	}

	template<typename Ret, typename... Args>
	inline
	CallbackStorage<Ret, Args...>::~CallbackStorage()
	{
		Destroy();
	}

	template<typename Ret, typename... Args>
	template<typename Storage, typename... ConstructArgs>
	void
	CallbackStorage<Ret, Args...>::Create(
		ConstructArgs&&... aConstructArgs)
	{
		// If this is a lambda, try to use a smaller bounded value
		// count. The current size is big enough for any sane
		// case.
		static_assert(sizeof(Storage) <= sizeof(myBuffer), "Too big object");
		// The special storage can't have an alignment bigger than
		// the base storage and its buffer member - 8 bytes. This
		// can be fixed, but no necessity in it for now.
		static_assert(alignof(Storage) <= 8, "Alignment can't be satisfied");
		MG_DEV_ASSERT(IsEmpty());
		myVtab = &Storage::ourVtab;
		new (myBuffer) Storage(Forward<ConstructArgs>(aConstructArgs)...);
		MG_DEV_ASSERT(!IsEmpty());
	}

	template<typename Ret, typename... Args>
	void
	CallbackStorage<Ret, Args...>::Destroy()
	{
		if (myVtab->Destroy != nullptr)
			myVtab->Destroy(myBuffer);
		myVtab = &Vtab::ourEmptyVtab;
	}

	template<typename Ret, typename... Args>
	void
	CallbackStorage<Ret, Args...>::MoveConstruct(
		CallbackStorage* aOther)
	{
		MG_DEV_ASSERT(IsEmpty());
		myVtab = aOther->myVtab;
		if (myVtab->MoveConstruct != nullptr)
			myVtab->MoveConstruct(myBuffer, aOther->myBuffer);
	}

	template<typename Ret, typename... Args>
	void
	CallbackStorage<Ret, Args...>::CopyConstruct(
		const CallbackStorage* aOther)
	{
		MG_DEV_ASSERT(IsEmpty());
		myVtab = aOther->myVtab;
		if (myVtab->CopyConstruct != nullptr)
			myVtab->CopyConstruct(myBuffer, aOther->myBuffer);
	}

	template<typename Ret, typename... Args>
	inline bool
	CallbackStorage<Ret, Args...>::IsEmpty() const
	{
		return myVtab == &Vtab::ourEmptyVtab;
	}

	template<typename Ret, typename... Args>
	Ret
	CallbackStorage<Ret, Args...>::Call(
		Args&&... aArgs)
	{
		MG_DEV_ASSERT(myVtab->Call != nullptr);
		return myVtab->Call(myBuffer, Forward<Args>(aArgs)...);
	}

	//////////////////////////////////////////////////////////////

	template<typename Object, typename Ret, typename... Args>
	inline
	CallbackStorageMethodMutable<Object, Ret, Args...>::CallbackStorageMethodMutable(
		Object* aObject,
		Signature aMethod)
		: myObject(aObject)
		, myMethod(aMethod)
	{
	}

	template<typename Object, typename Ret, typename... Args>
	Ret
	CallbackStorageCallMethodMutable(
		void* aStorage,
		Args&&... aArgs)
	{
		using Storage = CallbackStorageMethodMutable<Object, Ret, Args...>;
		Storage* storage = (Storage*) aStorage;
		Object* object = storage->myObject;
		const typename Storage::Signature method = storage->myMethod;

		return (object->*method)(Forward<Args>(aArgs)...);
	}

	//////////////////////////////////////////////////////////////

	template<typename Object, typename Ret, typename... Args>
	inline
	CallbackStorageMethodConst<Object, Ret, Args...>::CallbackStorageMethodConst(
		const Object* aObject,
		Signature aMethod)
		: myObject(aObject)
		, myMethod(aMethod)
	{
	}

	template<typename Object, typename Ret, typename... Args>
	Ret
	CallbackStorageCallMethodConst(
		void* aStorage,
		Args&&... aArgs)
	{
		using Storage = CallbackStorageMethodConst<Object, Ret, Args...>;
		Storage* storage = (Storage*) aStorage;
		const Object* object = storage->myObject;
		const typename Storage::Signature method = storage->myMethod;

		return (object->*method)(Forward<Args>(aArgs)...);
	}

	//////////////////////////////////////////////////////////////

	template<typename Func, typename Ret, typename... Args>
	inline
	CallbackStorageFunctor<Func, Ret, Args...>::CallbackStorageFunctor(
		const Func& aFunctor)
		: myFunctor(aFunctor)
	{
	}

	template<typename Func, typename Ret, typename... Args>
	inline
	CallbackStorageFunctor<Func, Ret, Args...>::CallbackStorageFunctor(
		Func&& aFunctor)
		: myFunctor(Forward<Func>(aFunctor))
	{
	}

	template<typename Func, typename Ret, typename... Args>
	inline
	CallbackStorageFunctor<Func, Ret, Args...>::CallbackStorageFunctor(
		const CallbackStorageFunctor& aFunctor)
		: myFunctor(aFunctor.myFunctor)
	{
	}

	template<typename Func, typename Ret, typename... Args>
	inline
	CallbackStorageFunctor<Func, Ret, Args...>::CallbackStorageFunctor(
		CallbackStorageFunctor&& aFunctor)
		: myFunctor(Forward<Func>(aFunctor.myFunctor))
	{
	}

	template<typename Func, typename Ret, typename... Args>
	Ret
	CallbackStorageCallFunctor(
		void* aStorage,
		Args&&... aArgs)
	{
		using Storage = CallbackStorageFunctor<Func, Ret, Args...>;
		Storage* storage = (Storage*) aStorage;

		return storage->myFunctor(Forward<Args>(aArgs)...);
	}

	//////////////////////////////////////////////////////////////

	template<typename Object>
	void
	CallbackStorageDestroy(
		void* aThis)
	{
		((Object*) aThis)->~Object();
	}

	template<typename Object>
	void
	CallbackStorageMoveConstruct(
		void* aThis,
		void* aOther)
	{
		Object* dst = (Object*) aThis;
		Object* src = (Object*) aOther;
		new (dst) Object(Move(*src));
	}

	template<typename Object>
	void
	CallbackStorageCopyConstruct(
		void* aThis,
		const void* aOther)
	{
		Object* dst = (Object*) aThis;
		const Object* src = (const Object*) aOther;
		new (dst) Object(*src);
	}

	template<typename Object, typename Ret, typename... Args>
	const CallbackStorageVtab<Ret, Args...>
	CallbackStorageMethodMutable<Object, Ret, Args...>::ourVtab = {
		nullptr,
		CallbackStorageMoveConstruct<CallbackStorageMethodMutable>,
		CallbackStorageCopyConstruct<CallbackStorageMethodMutable>,
		CallbackStorageCallMethodMutable<Object, Ret, Args...>,
	};

	template<typename Object, typename Ret, typename... Args>
	const CallbackStorageVtab<Ret, Args...>
	CallbackStorageMethodConst<Object, Ret, Args...>::ourVtab = {
		nullptr,
		CallbackStorageMoveConstruct<CallbackStorageMethodConst>,
		CallbackStorageCopyConstruct<CallbackStorageMethodConst>,
		CallbackStorageCallMethodConst<Object, Ret, Args...>,
	};

	template<typename Func, typename Ret, typename... Args>
	const CallbackStorageVtab<Ret, Args...>
	CallbackStorageFunctor<Func, Ret, Args...>::ourVtab = {
		CallbackStorageDestroy<CallbackStorageFunctor>,
		CallbackStorageMoveConstruct<CallbackStorageFunctor>,
		CallbackStorageCopyConstruct<CallbackStorageFunctor>,
		CallbackStorageCallFunctor<Func, Ret, Args...>,
	};

	template<typename Ret, typename... Args>
	const CallbackStorageVtab<Ret, Args...>
	CallbackStorageVtab<Ret, Args...>::ourEmptyVtab = {
		nullptr,
		nullptr,
		nullptr,
		nullptr,
	};

	//////////////////////////////////////////////////////////////

	template<typename Ret, typename... Args>
	inline
	Callback<Ret(Args...)>::Callback()
	{
	}

	template<typename Ret, typename... Args>
	template<typename Object>
	inline
	Callback<Ret(Args...)>::Callback(
		Object* aObject,
		Ret(Object::*aMethod)(Args...))
	{
		using Storage = CallbackStorageMethodMutable<Object, Ret, Args...>;
		myStorage.template Create<Storage>(aObject, aMethod);
	}

	template<typename Ret, typename... Args>
	template<typename Object>
	inline
	Callback<Ret(Args...)>::Callback(
		const Object* aObject,
		Ret(Object::*aMethod)(Args...) const)
	{
		using Storage = CallbackStorageMethodConst<Object, Ret, Args...>;
		myStorage.template Create<Storage>(aObject, aMethod);
	}

	template<typename Ret, typename... Args>
	inline
	Callback<Ret(Args...)>::Callback(
		const Callback& aOther)
	{
		myStorage.CopyConstruct(&aOther.myStorage);
	}

	template<typename Ret, typename... Args>
	inline
	Callback<Ret(Args...)>::Callback(
		Callback&& aOther)
	{
		myStorage.MoveConstruct(&aOther.myStorage);
	}

	template<typename Ret, typename... Args>
	template<typename Func, typename>
	inline
	Callback<Ret(Args...)>::Callback(
		Func&& aFunc)
	{
		using FuncOrig = typename RemoveConstAndReference<Func>::Type;
		using Storage = CallbackStorageFunctor<FuncOrig, Ret, Args...>;
		myStorage.template Create<Storage>(Forward<Func>(aFunc));
	}

	template<typename Ret, typename... Args>
	inline
	Callback<Ret(Args...)>::Callback(
		Ret(*aFunc)(Args...))
	{
		using Storage = CallbackStorageFunctor<Ret(*)(Args...), Ret, Args...>;
		myStorage.template Create<Storage>(aFunc);
	}

	template<typename Ret, typename... Args>
	inline void
	Callback<Ret(Args...)>::Clear()
	{
		myStorage.Destroy();
	}

	template<typename Ret, typename... Args>
	inline bool
	Callback<Ret(Args...)>::IsEmpty() const
	{
		return myStorage.IsEmpty();
	}

	template<typename Ret, typename... Args>
	template<typename Object>
	inline void
	Callback<Ret(Args...)>::Set(
		Object* aObject,
		Ret(Object::*aMethod)(Args...))
	{
		using Storage = CallbackStorageMethodMutable<Object, Ret, Args...>;
		// Some types does not support copying or moving into an
		// existing object (lambdas). To make them work the old
		// object should be destroyed.
		myStorage.Destroy();
		myStorage.template Create<Storage>(aObject, aMethod);
	}

	template<typename Ret, typename... Args>
	template<typename Object>
	inline void
	Callback<Ret(Args...)>::Set(
		const Object* aObject,
		Ret(Object::*aMethod)(Args...) const)
	{
		using Storage = CallbackStorageMethodConst<Object, Ret, Args...>;
		myStorage.Destroy();
		myStorage.template Create<Storage>(aObject, aMethod);
	}

	template<typename Ret, typename... Args>
	inline void
	Callback<Ret(Args...)>::Set(
		const Callback& aOther)
	{
		myStorage.Destroy();
		myStorage.CopyConstruct(&aOther.myStorage);
	}

	template<typename Ret, typename... Args>
	inline void
	Callback<Ret(Args...)>::Set(
		Callback&& aOther)
	{
		myStorage.Destroy();
		myStorage.MoveConstruct(&aOther.myStorage);
	}

	template<typename Ret, typename... Args>
	void
	Callback<Ret(Args...)>::Set(
		Ret(*Func)(Args...))
	{
		using Storage = CallbackStorageFunctor<Ret(*)(Args...), Ret, Args...>;
		myStorage.Destroy();
		myStorage.template Create<Storage>(Func);
	}

	template<typename Ret, typename... Args>
	template<typename Func, typename>
	inline void
	Callback<Ret(Args...)>::Set(
		Func&& aFunc)
	{
		using FuncOrig = typename RemoveConstAndReference<Func>::Type;
		using Storage = CallbackStorageFunctor<FuncOrig, Ret, Args...>;
		myStorage.Destroy();
		myStorage.template Create<Storage>(Forward<Func>(aFunc));
	}

	template<typename Ret, typename... Args>
	template<typename Func, typename>
	inline Callback<Ret(Args...)>&
	Callback<Ret(Args...)>::operator=(
		Func&& aFunc)
	{
		Set(mg::common::Forward<Func>(aFunc));
		return *this;
	}

	template<typename Ret, typename... Args>
	Callback<Ret(Args...)>&
	Callback<Ret(Args...)>::operator=(
		Callback&& aOther)
	{
		Set(mg::common::Move(aOther));
		return *this;
	}

	template<typename Ret, typename... Args>
	Callback<Ret(Args...)>&
	Callback<Ret(Args...)>::operator=(
		const Callback& aOther)
	{
		Set(aOther);
		return *this;
	}

	template<typename Ret, typename... Args>
	Ret
	Callback<Ret(Args...)>::operator()(
		Args... aArgs)
	{
		return myStorage.Call(Forward<Args>(aArgs)...);
	}

}
}
