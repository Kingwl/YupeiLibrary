#pragma once

#include "type_traits.h"

namespace Yupei
{
	namespace Internal
	{
		template<typename Type>
		Type* address_of_impl(Type& arg, true_type)
		{
			return arg;
		}

		template<typename Type>
		Type* address_of_impl(Type& arg, false_type)
		{
			return reinterpret_cast<Type*>(
				&const_cast<char&>(
					reinterpret_cast<const volatile char&>(arg)));

		}
	}

	template<typename Type>
	Type* addressof(Type& arg)
	{
		return Internal::address_of_impl(arg, is_function<remove_pointer_t<Type>>{});
	}

}
