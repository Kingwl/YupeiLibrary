#pragma once

#include "basic.h"
#include "type_traits.h"

namespace Yupei
{
	struct allocator_arg_t {};
	constexpr allocator_arg_t allocator_arg{};

	namespace Internal
	{
		template<typename Type,
			typename Alloc,typename = void>
		struct UsesAllocatorHelper : false_type
		{

		};

		template<typename Type,
			typename Alloc
		>
		struct UsesAllocatorHelper<Type,Alloc,Yupei::void_t<typename Type::allocator_type>>
			: bool_constant<
			is_convertible<typename Type::allocator_type,Alloc>::value>
		{

		};
	}

	template<typename Type,
		typename Alloc>
	struct uses_allocator : Internal::UsesAllocatorHelper<Type, Alloc>
	{

	};

	/*(1.1) ！ if uses_allocator<T, Alloc>::value is false and is_constructible<T, V1, V2, ..., VN>::value
		is true, then obj is initialized as obj(v1, v2, ..., vN);
	(1.2) ！ otherwise, if uses_allocator<T, Alloc>::value is true and is_constructible<T, allocator_ -
		arg_t, Alloc, V1, V2, ..., VN>::value is true, then obj is initialized as obj(allocator_arg,
			alloc, v1, v2, ..., vN);
	(1.3) ！ otherwise, if uses_allocator<T, Alloc>::value is true and is_constructible<T, V1, V2, ...,
		VN, Alloc>::value is true, then obj is initialized as obj(v1, v2, ..., vN, alloc);
	(1.4) ！ otherwise, the request for uses - allocator construction is ill - formed.[Note:An error will result if
		uses_allocator<T, Alloc>::value is true but the specific constructor does not take an allocator.
		This definition prevents a silent failure to pass the allocator to an element.！end note]*/


	template<typename Type,typename Alloc,typename... Args>
	struct can_be_constructed_by_allocator
	{
		static constexpr bool is_uses_allocator = uses_allocator<Type, Alloc>::value;
		static constexpr bool is_con1 = is_constructible<Type, allocator_arg_t, Args...>::value;
		static constexpr bool is_con2 = is_constructible<Type, Args..., Alloc>::value;
		static_assert(!is_uses_allocator ||
			(is_uses_allocator && (is_con1 || is_con2)), "prevents a silent failure to pass the allocator to an element.");
		using type = integral_constant< int,(is_uses_allocator ? (is_con1 ? 1 : 2) : 0)>;
	};

	template<typename Type, typename Alloc, typename... Args>
	using can_be_constructed_by_allocator_t = typename can_be_constructed_by_allocator<Type, Alloc, Args...>::type;
}
