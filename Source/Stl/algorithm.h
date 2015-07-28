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

	
}
