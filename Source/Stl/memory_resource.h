#pragma once

namespace Yupei
{
	namespace Experimental
	{
		namespace Pmr
		{
			class memory_resource;

			bool operator==(const memory_resource& a,
				const memory_resource& b) noexcept;
			bool operator!=(const memory_resource& a,
				const memory_resource& b) noexcept;

			template <class Tp> class polymorphic_allocator;

			template <class T1, class T2>
			bool operator==(const polymorphic_allocator<T1>& a,
				const polymorphic_allocator<T2>& b) noexcept;
			template <class T1, class T2>
			bool operator!=(const polymorphic_allocator<T1>& a,
				const polymorphic_allocator<T2>& b) noexcept;

			// The name resource_adaptor_imp is for exposition only.
			template <class Allocator> class resource_adaptor_imp;

			template <class Allocator>
			using resource_adaptor = resource_adaptor_imp<
				allocator_traits<Allocator>::rebind_alloc<char >> ;

			// Global memory resources
			memory_resource* new_delete_resource() noexcept;
			memory_resource* null_memory_resource() noexcept;

			// The default memory resource
			memory_resource* set_default_resource(memory_resource* r) noexcept;
			memory_resource* get_default_resource() noexcept;

			// Standard memory resources
			struct pool_options;
			class synchronized_pool_resource;
			class unsynchronized_pool_resource;
			class monotonic_buffer_resource;

		}
	}
}
