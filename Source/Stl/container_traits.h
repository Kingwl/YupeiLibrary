#pragma once

namespace Yupei
{

	template<typename Container>
	using container_value_type = typename Container::value_type;

	template<typename Container>
	using container_size_type = typename Container::size_type;

	template<typename Container>
	using container_difference_type = typename Container::difference_type;

	template<typename Container>
	using container_reference = typename Container::reference;

	template<typename Container>
	using container_const_reference = typename Container::const_reference;

	template<typename Container>
	using container_iterator = typename Container::iterator;

	template<typename Container>
	using container_const_iterator = typename Container::const_iterator;

	template<typename Container>
	using container_allocator_type = typename Container::allocator_type;

	template<typename Container>
	using container_pointer = typename Container::pointer;
}
