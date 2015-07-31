#pragma once


#include "type_traits.h"
#include "utility_internal.h"

namespace Yupei
{
	template<typename Type>
	void swap(Type& lhs, Type& rhs) noexcept(
		is_nothrow_constructible<Type>::value&&
		is_nothrow_move_assignable<Type>::value)
	{
		Type temp = Yupei::move(lhs);
		lhs = Yupei::move(rhs);
		rhs = Yupei::move(temp);
	}

	template<class ForwardIterator1, class ForwardIterator2>
	void iter_swap(ForwardIterator1 a, ForwardIterator2 b) noexcept(noexcept(swap(
		*Yupei::declval<ForwardIterator1>(), 
		*Yupei::declval<ForwardIterator2>())))
	{
		swap(*a, *b);
	}

	template<class ForwardIterator1, class ForwardIterator2>
	ForwardIterator2
		swap_ranges(ForwardIterator1 first1, ForwardIterator1 last1,
			ForwardIterator2 first2) noexcept(noexcept(Yupei::iter_swap(
				Yupei::declval<ForwardIterator1>(), 
				Yupei::declval<ForwardIterator2>())))
	{
		for (;first1 != last1;++first1,static_cast<void>(++first2))
		{
			Yupei::iter_swap(first1, first2);
		}
	}


	template<typename Type,
		std::size_t N>
		void swap(Type(&lhs)[N], Type(&rhs)[N]) noexcept(noexcept(swap(Yupei::declval<Type&>(), Yupei::declval<Type&>())))
	{
		Yupei::swap_ranges(lhs, lhs + N, rhs);
	}

	/*template<typename Type>
		using is_nothrow_swappable = bool_constant <
		noexcept(swap(declval<Type&>(), declval<Type&>()))>;*/

}