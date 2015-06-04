#pragma once

#include "basic.h"


namespace Yupei
{
	

	template<typename T>
	constexpr T&& forward(remove_reference_t<T>& value)
	{
		//lvalue
		return static_cast<T&&>(value);
	}

	template<typename T>
	constexpr T&& forward(remove_reference_t<T>&& value)
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

	

	
}
