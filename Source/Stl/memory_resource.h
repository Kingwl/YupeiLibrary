#pragma once

#include <cstddef>

namespace Yupei
{
	namespace Experimental
	{
		namespace Pmr
		{
			class memory_resource {
				// For exposition only
				static constexpr size_t max_align = alignof(std::max_align_t);

			public:
				virtual ~memory_resource() {}

				void* allocate(size_t bytes, size_t alignment = max_align)
				{
					return do_allocate(bytes, alignment);
				}
				void deallocate(void* p, size_t bytes,
					size_t alignment = max_align)
				{
					return do_deallocate(p, bytes, alignment);
				}

				bool is_equal(const memory_resource& other) const noexcept
				{
					return do_is_equal(other);
				}

			protected:
				virtual void* do_allocate(size_t bytes, size_t alignment) = 0;
				virtual void do_deallocate(void* p, size_t bytes,
					size_t alignment) = 0;

			//Returns:

			//	A derived class shall implement this function to return true if memory allocated from this can be deallocated from other and vice - versa; 
			//	otherwise it shall return false.[Note:The most - derived type of other might not match the type of this.
			//	For a derived class, D, a typical implementation of this function will compute dynamic_cast<const D*>(&other) 
			//	and go no further(i.e., return false) if it returns nullptr.¡ª end note]
				virtual bool do_is_equal(const memory_resource& other) const noexcept = 0;
			};

			bool operator==(const memory_resource& a,
				const memory_resource& b) noexcept;
			bool operator!=(const memory_resource& a,
				const memory_resource& b) noexcept;

			template <class Tp> class polymorphic_allocator;

			template <class T1, class T2>
			bool operator==(const polymorphic_allocator<T1>& a,
				const polymorphic_allocator<T2>& b) noexcept
			{
				return &a == &b || a.is_equal(b);
			}
			template <class T1, class T2>
			bool operator!=(const polymorphic_allocator<T1>& a,
				const polymorphic_allocator<T2>& b) noexcept
			{
				return !(a == b);
			}

			//// The name resource_adaptor_imp is for exposition only.
			//template <class Allocator> class resource_adaptor_imp;

			//template <class Allocator>
			//using resource_adaptor = resource_adaptor_imp<
			//	allocator_traits<Allocator>::rebind_alloc<char >> ;

			// Global memory resources
			memory_resource* new_delete_resource() noexcept;
			memory_resource* null_memory_resource() noexcept;

			// The default memory resource
			memory_resource* set_default_resource(memory_resource* r) noexcept;
			memory_resource* get_default_resource() noexcept;

			// Standard memory resources
			struct pool_options {
				size_t max_blocks_per_chunk = 4096;
				size_t largest_required_pool_block = 0;
			};

			class synchronized_pool_resource : public memory_resource {
			public:
				synchronized_pool_resource(const pool_options& opts, memory_resource* upstream);

				synchronized_pool_resource()
					: synchronized_pool_resource(pool_options(), get_default_resource()) { }
				explicit synchronized_pool_resource(memory_resource* upstream)
					: synchronized_pool_resource(pool_options(), upstream) { }
				explicit synchronized_pool_resource(const pool_options& opts)
					: synchronized_pool_resource(opts, get_default_resource()) { }

				synchronized_pool_resource(
					const synchronized_pool_resource&) = delete;
				virtual ~synchronized_pool_resource();

				synchronized_pool_resource& operator=(
					const synchronized_pool_resource&) = delete;

				void release();
				memory_resource* upstream_resource() const;
				pool_options options() const;

			protected:
				virtual void* do_allocate(size_t bytes, size_t alignment);
				virtual void do_deallocate(void* p, size_t bytes, size_t alignment);

				virtual bool do_is_equal(const memory_resource& other) const noexcept;
			};

			class unsynchronized_pool_resource : public memory_resource {
			public:
				unsynchronized_pool_resource(const pool_options& opts, memory_resource* upstream);

				unsynchronized_pool_resource()
					: unsynchronized_pool_resource(pool_options(), get_default_resource()) { }
				explicit unsynchronized_pool_resource(memory_resource* upstream)
					: unsynchronized_pool_resource(pool_options(), upstream) { }
				explicit unsynchronized_pool_resource(const pool_options& opts)
					: unsynchronized_pool_resource(opts, get_default_resource()) { }

				unsynchronized_pool_resource(
					const unsynchronized_pool_resource&) = delete;
				virtual ~unsynchronized_pool_resource();

				unsynchronized_pool_resource& operator=(
					const unsynchronized_pool_resource&) = delete;

				void release();
				memory_resource* upstream_resource() const;
				pool_options options() const;

			protected:
				virtual void* do_allocate(size_t bytes, size_t alignment);
				virtual void do_deallocate(void* p, size_t bytes, size_t alignment);

				virtual bool do_is_equal(const memory_resource& other) const noexcept;
			};
			class monotonic_buffer_resource;

		}
	}
}
