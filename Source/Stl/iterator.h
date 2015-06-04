#pragma once

#include "basic.h"

namespace Yupei
{
	template<typename CategoryType,
		typename ValueType,
		typename DistanceType = ptrdiff_t,
		typename PointerType = ValueType*,
		typename ReferenceType = ValueType&>
	struct iterator
	{
		using value_type = ValueType;
		using difference_type = DistanceType;
		using pointer = PointerType;
		using reference = ReferenceType;
		using iterator_category = CategoryType;
	};

	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag :public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};

	template<typename IteratorType>
	struct iterator_traits
	{
		using value_type = typename IteratorType::value_type;
		using difference_type = typename IteratorType::difference_type;
		using pointer = typename IteratorType::pointer;
		using reference = typename IteratorType::reference;
		using iterator_category = typename IteratorType::iterator_category;
	};

	template<typename T>
	struct iterator_traits<T*>
	{
		using value_type = T;
		using difference_type = ptrdiff_t;
		using pointer = T*;
		using reference = T&;
		using iterator_category = random_access_iterator_tag;
	};

}

