#pragma once

#include "utility.h"

namespace Yupei
{
	template<typename Type,
		typename UType,
		typename = void>
	struct has_rebind : false_type
	{
		//no rebind...
	};

	template<typename Type, typename UType>
	struct has_rebind<Type,
		UType,
		void_t<typename Type::template rebind<UType>>
	> : true_type
	{

	};

	template<typename Type,
		typename UType,
		bool HasRebind = has_rebind<Type, UType>::value>
	struct get_rebind
	{
		//no rebind...
	};

	template<typename Type, typename UType>
	struct get_rebind
		<
		Type,
		UType,
		true
		>
	{
		using type = typename Type::template rebind<UType>;
	};

	template<template<typename, typename...> typename Type,
		typename T1,
		typename... Args,
		typename UType>
	struct get_rebind<
		Type<T1, Args...>, UType,
		false>
	{
		using type = Type<UType, Args...>;
	};

	template<typename Type,
		typename = void>
	struct has_element_type : false_type
	{

	};

	template<typename Type>
	struct has_element_type<Type, void_t<typename Type::element_type>> : true_type
	{
		
	};

	template<typename Type,
		bool HasRebind = has_element_type<Type>::value>
	struct get_element_type
	{
		//no rebind...
		static_assert(has_element_type<Type>::value, "no element_type!");
	};

	template<typename Type>
	struct get_element_type
		<
		Type,
		true
		>
	{
		using type = typename Type::element_type;
	};

	template
		<
		template<typename, typename...> typename Type,
		typename T1,
		typename... Args
		>
	struct get_element_type<
		Type<T1, Args...>,
		false>
	{
		using type = T1;
	};
	
	
	template<class Ptr>
	struct pointer_traits
	{
		using pointer = Ptr;
		using element_type = typename get_element_type<Ptr>::type;
		using difference_type = get_difference_type_t<Ptr>;
		template<class U>
		using rebind = typename get_rebind<Ptr, U>::type;

		static pointer pointer_to(conditional_t<is_void<element_type>::value,
			void*,
			element_type&> r)
		{
			return pointer::pointer_to(r);
		}
	};

	template<class T>
	struct pointer_traits<T*>
	{
		using pointer = T*;
		using element_type = T;
		using difference_type = std::ptrdiff_t;
		template<class U>
		using rebind = U*;

		static pointer pointer_to(conditional_t<is_void<element_type>::value,
			void*,
			element_type&> r) noexcept
		{
			return Yupei::addressof(r);
		}
	};



	//minimal allocator_traits support
	template<typename AllocatorType>
	struct allocator_traits
	{
		using allocator_type = AllocatorType;
		using value_type = typename allocator_type::value_type;
		using reference = get_reference_t<AllocatorType>;
		//using pointer

	};

	
	template<typename PointerType,typename DeleterType>
	class unique_ptr;
}
