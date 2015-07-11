#pragma once

#include "utility.h"

namespace Yupei
{
	template<typename AllocatorType>
	struct allocator_traits
	{
		using allocator_type = AllocatorType;
		using value_type = typename allocator_type::value_type;
		//using reference = Extension::DefaultOrOtherType<allocator_type, reference, value_type&>;
	};

	
	template<typename PointerType,typename DeleterType>
	class unique_ptr;
}
