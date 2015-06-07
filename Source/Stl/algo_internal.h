#pragma once

#include <initializer_list>
namespace Yupei
{
	// 25.4.7, minimum and maximum:
	/*template<class T> constexpr const T& min(const T& a, const T& b);
	template<class T, class Compare>
	constexpr const T& min(const T& a, const T& b, Compare comp);
	template<class T>
	constexpr T min(initializer_list<T> t);
	template<class T, class Compare>
	constexpr T min(initializer_list<T> t, Compare comp);
	template<class T> constexpr const T& max(const T& a, const T& b);
	template<class T, class Compare>
	constexpr const T& max(const T& a, const T& b, Compare comp);*/
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

	/*template<typename Type>
	const Type& max(std::initializer_list<Type> _list)
	{

	}*/
}