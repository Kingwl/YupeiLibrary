#pragma once

#include "__allocator.h"
#include "type_traits.h"
#include "__swap.h"

namespace Yupei
{

	//constexpr implement
	template<typename Type, typename... Args>
	constexpr bool static_and(Type&& t, Args&&... args)
	{
		return t && static_and(args...);
	}

	template<typename Type>
	constexpr bool static_and(Type&& t)
	{
		return t;
	}

	struct tuple_alloc_t {};
	constexpr tuple_alloc_t tuple_alloc{};

	template<typename T1,
		typename T2>
	struct pair;

	//template <class... Types>
	//class tuple {
	//public:
	//	// 20.4.2.1, tuple construction
	//	constexpr tuple();
	//	EXPLICIT constexpr tuple(const Types&...); // only if sizeof...(Types) >= 1
	//	template <class... UTypes>
	//	EXPLICIT constexpr tuple(UTypes&&...); // only if sizeof...(Types) >= 1
	//	tuple(const tuple&) = default;
	//	tuple(tuple&&) = default;
	//	template <class... UTypes>
	//	EXPLICIT constexpr tuple(const tuple<UTypes...>&);
	//	template <class... UTypes>
	//	EXPLICIT constexpr tuple(tuple<UTypes...>&&);
	//	template <class U1, class U2>
	//	EXPLICIT constexpr tuple(const pair<U1, U2>&); // only if sizeof...(Types) == 2
	//	template <class U1, class U2>
	//	EXPLICIT constexpr tuple(pair<U1, U2>&&); // only if sizeof...(Types) == 2
	//											  // allocator-extended constructors
	//	template <class Alloc>
	//	tuple(allocator_arg_t, const Alloc& a);
	//	template <class Alloc>
	//	EXPLICIT tuple(allocator_arg_t, const Alloc& a, const Types&...);
	//	template <class Alloc, class... UTypes>
	//	EXPLICIT tuple(allocator_arg_t, const Alloc& a, UTypes&&...);
	//	template <class Alloc>
	//tuple(allocator_arg_t, const Alloc& a, const tuple&);
	//template <class Alloc>
	//tuple(allocator_arg_t, const Alloc& a, tuple&&);
	//template <class Alloc, class... UTypes>
	//EXPLICIT tuple(allocator_arg_t, const Alloc& a, const tuple<UTypes...>&);
	//template <class Alloc, class... UTypes>
	//EXPLICIT tuple(allocator_arg_t, const Alloc& a, tuple<UTypes...>&&);
	//template <class Alloc, class U1, class U2>
	//EXPLICIT tuple(allocator_arg_t, const Alloc& a, const pair<U1, U2>&);
	//template <class Alloc, class U1, class U2>
	//EXPLICIT tuple(allocator_arg_t, const Alloc& a, pair<U1, U2>&&);
	//// 20.4.2.2, tuple assignment
	//tuple& operator=(const tuple&);
	//tuple& operator=(tuple&&) noexcept(see below);
	//template <class... UTypes>
	//tuple& operator=(const tuple<UTypes...>&);
	//template <class... UTypes>
	//tuple& operator=(tuple<UTypes...>&&);
	//template <class U1, class U2>
	//tuple& operator=(const pair<U1, U2>&); // only if sizeof...(Types) == 2
	//template <class U1, class U2>
	//tuple& operator=(pair<U1, U2>&&); // only if sizeof...(Types) == 2
	//								  // 20.4.2.3, tuple swap
	//void swap(tuple&) noexcept(see below);
	//};

	template<typename... Args>
	struct tuple;

	template<typename ValueType>
	struct tuple_value_wrapper
	{
		ValueType Value;
		using my_type = tuple_value_wrapper<ValueType>;
		using internal_type = ValueType;

		tuple_value_wrapper() noexcept(is_nothrow_default_constructible<ValueType>::value)
			:Value()
		{

		}

		tuple_value_wrapper(const internal_type & value) noexcept(is_nothrow_constructible<ValueType,internal_type>::value)
			:Value(value)
		{

		}

		template<typename Alloc>
		tuple_value_wrapper(integral_constant<int, 0>, const Alloc&)
			:Value()
		{

		}

		template<typename Alloc>
		tuple_value_wrapper(integral_constant<int, 1>, const Alloc& alloc)
			:Value(allocator_arg,alloc)
		{

		}

		template<typename Alloc>
		tuple_value_wrapper(integral_constant<int, 2>, const Alloc& alloc)
			:Value(alloc)
		{

		}

		template<typename U,
			typename = enable_if_t<
			!is_same<decay_t<U>,my_type>::value &&
			is_constructible<internal_type,U&&>::value>>
		explicit tuple_value_wrapper(U&& u) noexcept(is_nothrow_constructible<internal_type,U&&>::value)
			:Value(Yupei::forward<U>(u))
		{

		}

		template<typename U,
			typename Alloc>
		explicit tuple_value_wrapper(integral_constant<int, 0>, const Alloc&, U&& u)
			:Value(Yupei::forward<U>(u))
		{

		}

		template<typename U,
			typename Alloc>
		explicit tuple_value_wrapper(integral_constant<int, 1>, const Alloc& alloc,U&& u)
			: Value(allocator_arg, alloc,Yupei::forward<U>(u))
		{

		}

		template<typename U,
			typename Alloc>
		explicit tuple_value_wrapper(integral_constant<int, 2>, const Alloc& alloc,U&& u)
			: Value(Yupei::forward<U>(u),alloc)
		{

		}

		tuple_value_wrapper(const tuple_value_wrapper&) = default;
		tuple_value_wrapper(tuple_value_wrapper&&) = default;

		template<typename U>
		my_type& operator=(U&& u) noexcept(is_nothrow_assignable<internal_type&, U&&>::value)
		{
			Value = Yupei::forward<U>(u);
			return *this;
		}

		int swap(my_type& rhs) noexcept(is_nothrow_swappable<my_type>::value)
		{
			swap(Value, rhs);
			return 0;
		}
		ValueType& get() noexcept
		{
			return Value;
		}
		const ValueType& get() const noexcept
		{
			return Value;
		}
		
	};

	template<typename... Args>
	struct tuple;

	template<>
	struct tuple<>
	{
		constexpr tuple() = default;
		tuple(const tuple&) = default;
		tuple(tuple&&) = default;

		template<typename Alloc>
		tuple(allocator_arg_t, const Alloc& a) noexcept
		{

		}
		template<typename Alloc>
		tuple(allocator_arg_t, const Alloc& a,const tuple&) noexcept
		{

		}
		template<typename Alloc>
		tuple(allocator_arg_t, const Alloc& a,tuple&&) noexcept
		{

		}

		void swap(const tuple& t) noexcept
		{

		}

		tuple& operator=(const tuple&) noexcept
		{

		}


		
	};

	/*template<bool... Pred>
	struct is_all_true :
		is_same<is_all_true<Pred...>, is_all_true<(Pred, true)...>>
	{
		
	};*/

	template<typename ThisType,
		typename... Args>
	class tuple<ThisType, Args...> : public tuple<Args...>
	{
	public:
		using base_type = tuple<Args...>;
		using this_type = ThisType;
		tuple_value_wrapper<ThisType> this_value;
		constexpr tuple()  noexcept(static_and(is_nothrow_default_constructible<ThisType>::value,
			is_nothrow_default_constructible<Args>::value...))
			:base_type(),
			this_value()
		{
			
		}

		constexpr tuple(const ThisType& thisValue, const Args&... args) noexcept(
			static_and(is_nothrow_copy_constructible<
			ThisType>::value,is_nothrow_copy_constructible<Args>::value...>))
			:this_value(thisValue),
			base_type(args...)
		{

		}

		constexpr tuple(ThisType&& thisValue, Args&&... args) noexcept(
			static_and(is_nothrow_move_constructible<
			ThisType>::value, is_nothrow_move_constructible<Args>::value...))
			:this_value(Yupei::forward<ThisType&&>(thisValue)),
			base_type(Yupei::forward<Args&&>(args)...)
		{

		}

		template<typename UThisType, typename... UArgs>
		explicit constexpr tuple(UThisType&& thisValue,UArgs&&... args)
			:this_value(Yupei::forward<UThisType>(thisValue)),
			base_type(Yupei::forward<UArgs>(args)...)
		{
			
		}

		tuple(const tuple&) = default;
		tuple(tuple&&) = default;


		template<typename UThisType, typename... UArgs>
		explicit constexpr tuple(tuple<UThisType, UArgs...>&& rhs)
			:this_value(Yupei::forward<typename tuple<UThisType, UArgs...>::this_type>(rhs.this_value)),
			base_type(Yupei::forward<typename tuple<UThisType, UArgs...>::base_type>(rhs))
		{

		}

		template<typename UThisType, typename... UArgs>
		explicit constexpr tuple(const tuple<UThisType, UArgs...>& rhs)
			:this_value(rhs.this_value),
			base_type(rhs.get_sliced())
		{

		}

		base_type& get_sliced() noexcept
		{
			return static_cast<base_type&>(*this);
		}

		const base_type& get_sliced() const noexcept
		{
			return static_cast<const base_type&>(*this);
		}

		template<typename Alloc>
		tuple(Yupei::allocator_arg_t, const Alloc& a)
			:this_value(can_be_constructed_by_allocator_t<this_type, Alloc>{}, a),
			base_type(allocator_arg,a)
		{

		}

		template<typename Alloc>
		explicit tuple(allocator_arg_t,
			const Alloc& a,
			const ThisType& t,
			const Args&... args)
			:this_value(can_be_constructed_by_allocator_t<this_type, Alloc>{}, a, t),
			base_type(allocator_arg,a,args...)
		{

		}

		template<typename Alloc,
			typename UThisType,
			typename... UArgs>
			explicit tuple(allocator_arg_t,
				const Alloc& a,
				UThisType&& u,
				UArgs&&... args)
			:this_value(can_be_constructed_by_allocator_t<this_type, Alloc>{}, a, Yupei::forward<UThisType>(u)),
			base_type(allocator_arg,a,Yupei::forward<UArgs>(args)...)
		{

		}

		template<typename Alloc>
		tuple(allocator_arg_t,const Alloc& a, const tuple& rhs) //lvalue
			:this_value(allocator_arg, a ,rhs.this_value),
			base_type(allocator_arg, a,rhs.get_sliced())
		{

		}

		template<typename Alloc>
		tuple(allocator_arg_t, const Alloc& a,tuple&& rhs)
			:this_value(allocator_arg, a, Yupei::forward<tuple::this_type&&>(rhs.this_value)),
			base_type(allocator_arg, a, Yupei::forward<tuple::base_type&&>(rhs.get_sliced()))
		{

		}

		template<typename Alloc,
			typename... U>
			tuple(allocator_arg_t, const Alloc& a, const tuple<U...>& rhs)
			:this_value(allocator_arg, a, rhs.this_value),
			base_type(allocator_arg, a,rhs.get_sliced())
		{

		}

		template<typename Alloc,
			typename... U>
		tuple(allocator_arg_t, const Alloc& a, tuple<U...>&& rhs)
			:this_value(allocator_arg, a, Yupei::forward<typename tuple<U...>::this_type&&>(rhs.this_value)),
			base_type(allocator_arg, a, Yupei::forward<typename tuple<U...>::base_type&&>(rhs.get_sliced()))
		{

		}

		this_type& get() noexcept
		{
			return this_value.get();
		}

		const this_type& get() const noexcept
		{
			return this_value.get();
		}
	};

	namespace Internal
	{
		template<std::size_t N,
			std::size_t F,
			typename Type>
		struct tuple_element_impl;

		template<std::size_t N,
			std::size_t F,
			typename Type,
			typename... Args>
		struct tuple_element_impl<N, F, tuple<Type, Args...> >
		{
			using type = typename tuple_element_impl<N + 1, F, tuple<Args...>>::type;
		};

		template<
			std::size_t F,
			typename Type,
			typename... Args>
		struct tuple_element_impl<F, F, tuple<Type, Args...> >
		{
			using type = Type;
		};

		template<std::size_t N,
			std::size_t F
		>
		struct tuple_element_impl<N, F, tuple<>>
		{
			static_assert(N == 0, "Out of tuple range.");
			static_assert(N != 0, "Out of tuple range.");
		};

		
	}

	template<std::size_t Id,
		typename Type>
	class tuple_element;

	template<std::size_t Id,
		typename... Args>
	class tuple_element<Id, tuple<Args...>> : public Internal::tuple_element_impl<0,Id,tuple<Args...>>
	{

	};

	template<std::size_t Id,
		typename T>
	class tuple_element<Id, const T>
	{
	public:
		using type = Yupei::add_const_t<
			typename tuple_element<Id, T>::type>;
	};

	template<std::size_t Id,
		typename T>
	class tuple_element<Id, volatile T>
	{
	public:
		using type = Yupei::add_volatile_t<
			typename tuple_element<Id, T>::type>;
	};

	template<std::size_t Id,
		typename T>
	class tuple_element<Id, const volatile T>
	{
	public:
		using type = Yupei::add_cv_t<
			typename tuple_element<Id, T>::type>;
	};

	template<std::size_t Id,
		typename T>
	using tuple_element_t = typename tuple_element<Id, T>::type;

	namespace Internal
	{
		template<typename Type,
			typename NowType,
			std::size_t Ep,
			std::size_t Sp
			>
		struct make_tuple_type;

		template<
			typename... Args,
			typename... UArgs,
			std::size_t Ep,
			std::size_t Sp
			>
		struct make_tuple_type<tuple<Args...>, tuple<UArgs...>,Ep, Sp>
		{
			using type = typename make_tuple_type<tuple<Args...>, tuple < UArgs..., tuple_element_t<Sp, tuple<Args...>>>, Ep,Sp + 1>::type;
		};

		template<
			typename... Args,
			typename... UArgs,
			std::size_t Ep
			>
		struct make_tuple_type<tuple<Args...>, tuple<UArgs...>,Ep, Ep >
		{
			using type = tuple<UArgs...>;
		};

		template<typename Type,
			std::size_t Ep,
			std::size_t Sp = 0
		>
		using make_tuple_type_t = typename make_tuple_type<Type, tuple<>, Ep, Sp>::type;
	}

	template< std::size_t I, typename... Types >
	constexpr tuple_element_t<I, tuple<Types...> >& get(tuple<Types...>& t)
	{
		return static_cast<Internal::make_tuple_type_t<tuple<Types...>,I+1>&>(t).get();
	}
}
