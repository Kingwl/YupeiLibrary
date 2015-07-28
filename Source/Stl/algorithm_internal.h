#pragma once

#include "compare_funtor.h"
#include <initializer_list>

namespace Yupei
{
	template<typename IteratorType, typename FunctorType>
	IteratorType max_element(IteratorType first,
		IteratorType last, FunctorType functor)
	{
		auto res = first;
		if (first != last)
		{
			for (++first;first != last;++first)
			{
				if (functor(*res, first))
					res = first;
			}
		}
		return res;
	}

	template<typename IteratorType>
	IteratorType max_element(IteratorType first,
		IteratorType last)
	{
		return Yupei::max_element(first, last, greater<>{});
	}
	// 25.4.7, minimum and maximum:
	template<class T> 
	constexpr const T& min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}
	template<class T, class Compare>
	constexpr const T& min(const T& a, const T& b, Compare comp)
	{
		return comp(a, b) ? a : b;
	}
	template<class T>
	T min(std::initializer_list<T> t)
	{
		return *(Yupei::)
	}
}
