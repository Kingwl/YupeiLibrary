#pragma once

namespace Yupei
{
	template<typename T, T V>
	struct integral_constant
	{
		static constexpr T value = V;
		using value_type = T;
		using type = integral_constant<T, V>;

		constexpr operator value_type () const noexcept
		{
			return value;
		}

		constexpr value_type operator ()() const noexcept
		{
			return value;
		}
	};

	//C++ 17 bool_constant
	template<bool B>
	using bool_constant = integral_constant<bool, B>;

	using true_type = bool_constant<true>;
	using false_type = bool_constant<false>;

	namespace Internal
	{
		struct WrapInt
		{
			constexpr WrapInt(int) {}
		};

	}
}