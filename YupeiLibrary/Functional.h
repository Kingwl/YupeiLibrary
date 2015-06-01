#pragma once

#undef min
#undef max

namespace Yupei
{
	template<typename Type>
	const Type& min(const Type& t1, const Type& t2)
	{
		return t1 < t2 ? t1 : t2;
	}

	template<typename Type>
	const Type& max(const Type& t1, const Type& t2)
	{
		return t1 > t2 ? t1 : t2;
	}

	template<typename Type>
	const Type& min(std::initializer_list<Type> _list)
	{

	}
}
