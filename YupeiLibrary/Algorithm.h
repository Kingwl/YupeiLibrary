#pragma once

namespace Yupei
{
	template<typename IteratorType,typename FunctorType>
	void for_each(IteratorType beginIterator,
		IteratorType endIterator,
		FunctorType functor)
	{
		for (; beginIterator != endIterator; ++beginIterator)
		{
			functor(*beginIterator);
		}
	}

	template<typename IteratorType,typename FunctorType>
	IteratorType max_element(IteratorType first,
		IteratorType last,FunctorType functor)
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
		return Yupei::max_element(first,last,)
	}
}
