#pragma once

#include <cstdint>
#include "traits_internal.h"

namespace Yupei
{

	using gcd_type = std::uintmax_t;

	template<typename T>
	constexpr inline T StaticGetGcd(T v1, T v2)
	{
		return v2 == 0 ? v1 : StaticGetGcd(v2, v1 % v2);
	}

	template<typename T>
	constexpr inline T StaticGetLcm(T v1, T v2)
	{
		return v1 * v2 / StaticGetGcd(v1, v2);
	}

	template<typename T1,
		typename T2>
	constexpr inline T1 StaticPow(T1 num, T2 exp)
	{
		return exp == 0 ? 1 : num * StaticPow(num,--exp);
	}
}