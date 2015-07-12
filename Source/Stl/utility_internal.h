#pragma once

#include "traits_internal.h"
#include "container_traits.h"

namespace Yupei
{
	template<typename T>
	inline constexpr T&& forward(remove_reference_t<T>& value)
	{
		//lvalue
		return static_cast<T&&>(value);
	}

	template<typename T>
	inline constexpr T&& forward(remove_reference_t<T>&& value)
	{
		//rvalue
		return static_cast<T&&>(value);
	}

	template<typename T>
	constexpr remove_reference_t<T>&& rvalue_cast(T&& value)
	{
		return static_cast<remove_reference_t<T>&&>(value);
	}

	template<typename T>
	constexpr remove_reference_t<T>&& move(T&& value)
	{
		return static_cast<remove_reference_t<T>&&>(value);
	}

	template<typename Type>
	add_rvalue_reference_t<Type> declval() noexcept;

	//The erased_type struct is an empty struct that serves as a placeholder for a type T 
	//in situations where the actual type T is determined at runtime.For example, 
	//the nested type, allocator_type, is an alias for erased_type in classes that use type -
	//erased allocators(see[type.erased.allocator]).
	inline namespace Experimental
	{
		struct erased_type {};
	}
	// get or not series

	template<typename Type,
		typename = void>
	struct get_pointer
	{
		using type = container_value_type<Type>*;
	};

	template<typename Type>
	struct get_pointer<Type,
		void_t<container_pointer<Type>>
	>
	{
		using type = container_pointer<Type>;
	};

	template<typename Type>
	using get_pointer_t = typename get_pointer<Type>::type;

	template<typename Type,
		typename = void>
	struct get_reference
	{
		using type = container_value_type<Type>&;
	};

	template<typename Type>
	struct get_reference<Type,
		void_t<container_reference<Type>>
	>
	{
		using type = container_reference<Type>;
	};

	template<typename Type>
	using get_reference_t = typename get_reference<Type>::type;

	template<typename Type,
		typename = void>
	struct get_const_reference
	{
		using type = const container_value_type<Type>&;
	};

	template<typename Type>
	struct get_const_reference<Type,
		void_t<container_reference<Type>>
	>
	{
		using type = container_const_reference<Type>;
	};

	template<typename Type>
	using get_const_reference_t = typename get_const_reference<Type>::type;

	template<typename Type,
		typename = void>
	struct get_difference_type
	{
		using type = std::ptrdiff_t;
	};

	template<typename Type>
	struct get_difference_type<Type,
		void_t<container_difference_type<Type>>
	>
	{
		using type = container_difference_type<Type>;
	};

	template<typename Type>
	using get_difference_type_t = typename get_difference_type<Type>::type;


	
	template<typename ValueType>
	struct container_utility
	{
		using value_type = ValueType;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
	};
	
}