#pragma once

//#include "basic.h"
#include "utility_internal.h"
#include "type_traits.h"

namespace Yupei
{
	
//  #include <initializer_list>
//	namespace std {
//		// 20.2.1, operators:
//		namespace rel_ops {
//			template<class T> bool operator!=(const T&, const T&);
//			template<class T> bool operator> (const T&, const T&);
//			template<class T> bool operator<=(const T&, const T&);
//			template<class T> bool operator>=(const T&, const T&);
//		}
//		// 20.2.2, swap:
//		template<class T> void swap(T& a, T& b) noexcept(see below);
//		template <class T, size_t N> void swap(T(&a)[N], T(&b)[N]) noexcept(noexcept(swap(*a, *b)));

	namespace rel_ops
	{
		template<typename Type> 
		bool operator != (const Type& lhs, const Type& rhs)
		{
			return !(lhs == rhs);
		}

		template<typename Type>
		bool operator > (const Type& lhs, const Type& rhs)
		{
			return rhs < lhs;
		}

		template<typename Type>
		bool operator <= (const Type& lhs, const Type& rhs)
		{
			return !(rhs < lhs);
		}

		template<typename Type>
		bool operator >= (const Type& lhs, const Type& rhs)
		{
			return !(lhs < rhs);
		}
	}
	// 20.2.3, exchange:
	//template <class T, class U = T> T exchange(T& obj, U&& new_val);

	template <class T, class U = T> 
	T exchange(T& obj, U&& new_val)
	{
		T old_val = Yupei::move(obj);
		obj = Yupei::forward<U>(new_val);
		return old_val;
	}

	/*template <class T1, class T2>
	struct pair {
		typedef T1 first_type;
		typedef T2 second_type;
		T1 first;
		T2 second;
		pair(const pair&) = default;
		pair(pair&&) = default;
		constexpr pair();
		EXPLICIT constexpr pair(const T1& x, const T2& y);
		template<class U, class V> EXPLICIT constexpr pair(U&& x, V&& y);
		template<class U, class V> EXPLICIT constexpr pair(const pair<U, V>& p);
		template<class U, class V> EXPLICIT constexpr pair(pair<U, V>&& p);
		template <class... Args1, class... Args2>
		pair(piecewise_construct_t,
			tuple<Args1...> first_args, tuple<Args2...> second_args);
		pair& operator=(const pair& p);
		template<class U, class V> pair& operator=(const pair<U, V>& p);
		pair& operator=(pair&& p) noexcept(see below);
		template<class U, class V> pair& operator=(pair<U, V>&& p);
		void swap(pair& p) noexcept(see below);
	};*/
}
