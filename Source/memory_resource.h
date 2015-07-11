#pragma once

namespace Yupei
{
	namespace Experimental
	{
		namespace Pmr
		{
			class memory_resource;

			bool operator == (const memory_resource& a,
				const memory_resource& b) noexcept;
			bool operator != (const memory_resource& a,
				const memory_resource& b) noexcept;

			template<class Tp>
			class polymorphic_allocator;

			template<class T1,class T2>
			bool operator == (const polymorphic_allocator<T1>& a,
				const polymorphic_allocator<T2>& b) noexcept;

			template<class T1, class T2>
			bool operator != (const polymorphic_allocator<T1>& a,
				const polymorphic_allocator<T2>& b) noexcept;
		}
	}
}
