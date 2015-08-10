#pragma once

#include "type_traits.h"
#include "__addressof.h"

namespace Yupei
{
	namespace Internal
	{
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

	}
	

	template<class Ptr>
	struct pointer_traits
	{
		using pointer = Ptr;
		using element_type = typename Internal::get_element_type<Ptr>::type;
		using difference_type = get_difference_type_t<Ptr>;
		template<class U>
		using rebind = typename Internal::get_rebind<Ptr, U>::type;

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
}
