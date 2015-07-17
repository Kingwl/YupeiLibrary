#include "memory_resource.h"


namespace Yupei
{
	inline namespace Experimental
	{
		namespace Pmr
		{

			//Returns:
			/*A pointer to a static - duration object of a type derived from memory_resource that
			can serve as a resource for allocating memory using ::operator new and ::operator delete.The same value is
			returned every time this function is called.For return value p and memory resource r, p->is_equal(r)
			returns &r == p.*/
			memory_resource * new_delete_resource() noexcept
			{
				static Internal::new_delete_resource new_del_res{};
				return &new_del_res;
			}

			/*Returns:
			A pointer to a static - duration object of a type derived from memory_resource for which allocate()
			always throws bad_alloc and for which deallocate() has no effect.The same value is returned
			every time this function is called.For return value p and memory resource r, p->is_equal(r)
			returns &r == p.*/
			memory_resource * null_memory_resource() noexcept
			{
				static Internal::null_memory_resource null_mem_res{};
				return &null_mem_res;
			}

			static Yupei::Experimental::Pmr::memory_resource* defaultResouce = new_delete_resource();

			memory_resource * set_default_resource(memory_resource * r) noexcept
			{
				auto ret = defaultResouce;
				defaultResouce = r;
				return ret;
			}

			memory_resource * get_default_resource() noexcept
			{
				return defaultResouce;
			}
		}
	}

	


	

}