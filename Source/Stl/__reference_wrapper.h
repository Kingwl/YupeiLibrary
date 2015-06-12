#pragma once

#include "type_traits.h"
#include "__addressof.h"

namespace Yupei
{
	namespace Internal
	{
		template<typename Type,
			typename = void>
		struct result_type_traits {};

		template<typename Type>
		struct result_type_traits<Type, void_t<typename Type::result_type >>
		{
			using result_type = typename Type::result_type;
		};

		template<typename Type,
			typename = void>
		struct arguement_type_traits : result_type_traits<Type> {};

		template<typename Type>
		struct arguement_type_traits<Type, void_t<typename Type::arguement_type >>
			: result_type_traits<Type>
		{
			using arguement_type = typename Type::arguement_type;
		};

		template<typename Type,
			typename = void>
		struct binary_args_traits : arguement_type_traits<Type> {};

		template<typename Type>
		struct binary_args_traits<Type, void_t<typename Type::first_argument_type,
			typename Type::second_argument_type >>
			:arguement_type_traits<Type>
		{
			using first_argument_type = typename Type::first_argument_type;
		using second_argument_type = typename Type::second_argument_type;
		};

		template<typename Type>
		struct weak_types_traits
		{
			using type = conditional_t <
				is_function<remove_pointer_t<Type>>::value,
				Internal::IsFunctionHelper<remove_pointer_t<Type>>,
				conditional_t <
				is_member_function_pointer<remove_cv_t<Type>>::value,
				Internal::IsMemberFunctionHelper<remove_cv_t<Type>>,
				binary_args_traits<Type>
				>> ;
		};
	};

	//template <class T> class reference_wrapper {
	//public:
	//	// types
	//	typedef T type;
	//	typedef see below result_type; // not always defined
	//	typedef see below argument_type; // not always defined
	//	typedef see below first_argument_type; // not always defined
	//	typedef see below second_argument_type; // not always defined
	//											// construct/copy/destroy
	//	reference_wrapper(T&) noexcept;
	//	reference_wrapper(T&&) = delete; // do not bind to temporary objects
	//	reference_wrapper(const reference_wrapper& x) noexcept;
	//	// assignment
	//	reference_wrapper& operator=(const reference_wrapper& x) noexcept;
	//	// access
	//	operator T& () const noexcept;
	//	T& get() const noexcept;
	//	// invocation
	//	template <class... ArgTypes>
	//	result_of_t<T&(ArgTypes&&...)>
	//		operator() (ArgTypes&&...) const;
	//};

	//If a call wrapper(20.9.1) has a weak result type the type of its member type result_type is based on the
	//	type T of the wrapper¨s target object(20.9.1) :
	/*(3.1) ！ if T is a pointer to function type, result_type shall be a synonym for the return type of T;
	(3.2) ！ if T is a pointer to member function, result_type shall be a synonym for the return type of T;
	(3.3) ！ if T is a class type and the qualified - id T::result_type is valid and denotes a type(14.8.2), then
	result_type shall be a synonym for T::result_type;
	(3.4) ！ otherwise result_type shall not be defined.*/

	template <typename T> class reference_wrapper : public Internal::weak_types_traits<T>::type
	{
	public:
		// types
		reference_wrapper(T& arg) noexcept
			: ptr(Yupei::addressof(arg))
		{

		}
		reference_wrapper(T&&) = delete; // do not bind to temporary objects
		reference_wrapper(const reference_wrapper& x) noexcept = default;
		// assignment
		reference_wrapper& operator=(const reference_wrapper& x) noexcept = default;
		// access
		operator T& () const noexcept
		{
			return *ptr;
		}
		T& get() const noexcept
		{
			return *ptr;
		}
		// invocation
		template <class... ArgTypes>
		result_of_t<T&(ArgTypes&&...)>
			operator() (ArgTypes&&...args) const
		{
			return Yupei::invoke(get(), Yupei::forward<ArgTypes>(args)...);
		}
	private:
		T* ptr;
	};
}