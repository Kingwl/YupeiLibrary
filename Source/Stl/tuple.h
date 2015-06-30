#pragma once

#include "__allocator.h"
#include "type_traits.h"
#include "utility.h"

namespace Yupei
{
	//20.4.2, class template tuple:
	// template <class... Types> class tuple;
	//20.4.2.4, tuple creation functions:
	// const unspecified ignore;
	// template <class... Types>
	// constexpr tuple<VTypes ...> make_tuple(Types&&...);
	// template <class... Types>
	// constexpr tuple<Types&&...> forward_as_tuple(Types&&...) noexcept;
	// template<class... Types>
	// constexpr tuple<Types&...> tie(Types&...) noexcept;
	// template <class... Tuples>
	// constexpr tuple<Ctypes ...> tuple_cat(Tuples&&...);
	//20.4.2.5, tuple helper classes:
	// template <class T> class tuple_size; // undefined
	// template <class T> class tuple_size<const T>;
	// template <class T> class tuple_size<volatile T>;
	// template <class T> class tuple_size<const volatile T>;
	// template <class... Types> class tuple_size<tuple<Types...> >;
	// template <size_t I, class T> class tuple_element; // undefined
	// template <size_t I, class T> class tuple_element<I, const T>;
	// template <size_t I, class T> class tuple_element<I, volatile T>;
	// template <size_t I, class T> class tuple_element<I, const volatile T>;
	// template <size_t I, class... Types> class tuple_element<I, tuple<Types...> >;
	// template <size_t I, class T>
	// using tuple_element_t = typename tuple_element<I, T>::type;
	//20.4.2.6, element access:
	// template <size_t I, class... Types>
	// constexpr tuple_element_t<I, tuple<Types...>>&
	// get(tuple<Types...>&) noexcept;
	// template <size_t I, class... Types>
	// constexpr tuple_element_t<I, tuple<Types...>>&&
	// get(tuple<Types...>&&) noexcept;
	// template <size_t I, class... Types>
	// constexpr const tuple_element_t<I, tuple<Types...>>&
	// get(const tuple<Types...>&) noexcept;
	// template <class T, class... Types>
	// constexpr T& get(tuple<Types...>& t) noexcept;
	// template <class T, class... Types>
	// constexpr T&& get(tuple<Types...>&& t) noexcept;
	// template <class T, class... Types>
	// constexpr const T& get(const tuple<Types...>& t) noexcept;
	//20.4.2.7, relational operators:
	// template<class... TTypes, class... UTypes>
	// constexpr bool operator==(const tuple<TTypes...>&, const tuple<UTypes...>&);
	// template<class... TTypes, class... UTypes>
	// constexpr bool operator<(const tuple<TTypes...>&, const tuple<UTypes...>&);
	// template<class... TTypes, class... UTypes>
	// constexpr bool operator!=(const tuple<TTypes...>&, const tuple<UTypes...>&);
	// template<class... TTypes, class... UTypes>
	// constexpr bool operator>(const tuple<TTypes...>&, const tuple<UTypes...>&);
	// template<class... TTypes, class... UTypes>
	// constexpr bool operator<=(const tuple<TTypes...>&, const tuple<UTypes...>&);
	// template<class... TTypes, class... UTypes>
	// constexpr bool operator>=(const tuple<TTypes...>&, const tuple<UTypes...>&);
	//20.4.2.8, allocator-related traits
	// template <class... Types, class Alloc>
	// struct uses_allocator<tuple<Types...>, Alloc>;
	//20.4.2.9, specialized algorithms:
	// template <class... Types>
	// void swap(tuple<Types...>& x, tuple<Types...>& y) noexcept(see below );
	//constexpr implement
	
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


	struct ignore_t
	{
		template<typename U>
		const ignore_t& operator =(U&& u) const noexcept
		{
			return *this;
		}
	};

	const ignore_t ignore{};

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

		explicit tuple_value_wrapper(const internal_type & value) noexcept(is_nothrow_constructible<ValueType,internal_type>::value)
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
		my_type& operator=(const tuple_value_wrapper<U>& u) 
		{
			Value = u.Value;
			return *this;
		}

		template<typename U>
		my_type& operator=(tuple_value_wrapper<U>&& u) noexcept(is_nothrow_assignable<internal_type&, U&&>::value)
		{
			Value = Yupei::forward<U>(u).Value;
			return *this;
		}

		void swap(my_type& rhs) noexcept(is_nothrow_swappable<my_type>::value)
		{
			Yupei::swap(Value, rhs.Value);
		}
		ValueType& get() noexcept
		{
			return Value;
		}
		const ValueType& get() const noexcept
		{
			return Value;
		}

		template<typename Type>
		bool Less(const tuple_value_wrapper<Type>& t) const
		{
			return Value < t.Value;
		}

		template<typename Type>
		bool Equal(const tuple_value_wrapper<Type>& t) const
		{
			return Value == t.Value;
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
			return *this;
		}

		tuple& operator=(tuple&&) noexcept
		{
			return *this;
		}

		bool Less(const tuple& rhs) const noexcept
		{
			return false;
		}

		bool Equal(const tuple& rhs) const noexcept
		{
			return true;
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

		explicit constexpr tuple(const ThisType& thisValue, const Args&... args) noexcept(static_and(is_nothrow_copy_constructible<ThisType>::value,is_nothrow_copy_constructible<Args>::value...))
			:this_value(thisValue),
			base_type(args...)
		{

		}

		template<typename UThisType, typename... UArgs,
		typename = enable_if_t<!is_one_of<
			decay_t<UThisType>,
			allocator_arg_t,
			tuple>::value>>
		explicit constexpr tuple(UThisType&& thisValue,UArgs&&... args)
			:this_value(Yupei::forward<UThisType>(thisValue)),
			base_type(Yupei::forward<UArgs>(args)...)
		{
			
		}

		template <class U1, class U2,
		typename = enable_if_t<sizeof...(Args) == 2 &&
		is_convertible<U1,>
		explicit constexpr tuple(pair<U1, U2>&&) // only if sizeof...(Types) == 2
		{

		}

		tuple(const tuple&) = default;
		tuple(tuple&&) = default;


		template<typename UThisType, typename... UArgs>
		constexpr tuple(tuple<UThisType, UArgs...>&& rhs)
			:this_value(Yupei::forward<typename tuple<UThisType, UArgs...>::this_type>(rhs.this_value)),
			base_type(Yupei::forward<typename tuple<UThisType, UArgs...>::base_type>(rhs))
		{

		}

		template<typename UThisType, typename... UArgs>
		constexpr tuple(const tuple<UThisType, UArgs...>& rhs)
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
		tuple(allocator_arg_t,
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
		tuple(allocator_arg_t,
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

		void swap(tuple& rhs) noexcept(static_and(
			is_nothrow_swappable<tuple_value_wrapper<ThisType>>::value,
			is_nothrow_swappable<Args>::value...))
		{
			this_value.swap(rhs.this_value);
			get_sliced().swap(rhs.get_sliced());
		}

		template<typename UType,
			typename... UArgs,
			typename = enable_if_t<sizeof...(UArgs) == sizeof...(Args)>>
			bool Less(const tuple<UType, UArgs...>& rhs) const
		{
			return this_value.Less(rhs.this_value) && get_sliced().Less(rhs.get_sliced());
		}

		template<typename UType,
			typename... UArgs,
			typename = enable_if_t<sizeof...(UArgs) == sizeof...(Args) >>
			bool Equal(const tuple<UType, UArgs...>& rhs) const
		{
			return this_value.Equal(rhs.this_value) && get_sliced().Equal(rhs.get_sliced());
		}

		template<typename Type,
		typename... Args>
		tuple& operator=(const tuple<Type,Args...>& rhs)
		{
			this_value = rhs.this_value;
			get_sliced() = rhs.get_sliced();
			return *this;
		}

		template<typename Type,
			typename... Args>
			tuple& operator=(tuple<Type, Args...>&& rhs) 
		{
			this_value = Yupei::forward<tuple<Type, Args...>&&>(rhs).this_value;
			get_sliced() = Yupei::forward<tuple<Type, Args...>&&>(rhs).get_sliced();
			return *this;
		}

		
		tuple& operator=(const tuple& rhs)
		{
			this_value = rhs.this_value;
			get_sliced() = rhs.get_sliced();
			return *this;
		}

		tuple& operator=(tuple&& rhs) noexcept(static_and(is_nothrow_move_constructible<Type>::value, is_nothrow_move_constructible<Args>::value...))
		{
			this_value = Yupei::forward<tuple<Type, Args...>&&>(rhs).this_value;
			get_sliced() = Yupei::forward<tuple<Type, Args...>&&>(rhs).get_sliced();
			return *this;
		}
	};

	template<typename... Args>
	void swap(tuple<Args...>& lhs, tuple<Args...>& rhs)
	{
		lhs.swap(rhs);
	}

	template<typename... Args,
		typename... UArgs>
	constexpr bool operator == (const tuple<Args...>& lhs, const tuple<UArgs...>& rhs)
	{
		return lhs.Equal(rhs);
	}

	template<typename... Args,
		typename... UArgs>
		constexpr bool operator < (const tuple<Args...>& lhs, const tuple<UArgs...>& rhs)
	{
		return lhs.Less(rhs);
	}

	template<typename... Args,
		typename... UArgs>
		constexpr bool operator != (const tuple<Args...>& lhs, const tuple<UArgs...>& rhs)
	{
		return !lhs.Equal(rhs);
	}

	template<typename... Args,
		typename... UArgs>
		constexpr bool operator > (const tuple<Args...>& lhs, const tuple<UArgs...>& rhs)
	{
		return rhs.Less(lhs);
	}

	template<typename... Args,
		typename... UArgs>
		constexpr bool operator <= (const tuple<Args...>& lhs, const tuple<UArgs...>& rhs)
	{
		return !rhs.Less(lhs);
	}

	template<typename... Args,
		typename... UArgs>
		constexpr bool operator >= (const tuple<Args...>& lhs, const tuple<UArgs...>& rhs)
	{
		return !lhs.Less(rhs);
	}

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
		typename... Args>
	class tuple_element<Id, tuple<Args...>>  
	{
		static_assert(Id < sizeof...(Args), "tuple_element<> out of range");
	public:
		using type = typename Internal::tuple_element_impl<0, Id, tuple<Args...>>::type;
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
			static_assert(Sp <= Ep, "Start > End!");
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

	
	template<typename... Args>
	class tuple_size<tuple<Args...>> : public integral_constant<
		std::size_t,
		sizeof...(Args)>
	{

	};

	template< typename T >
	class tuple_size<const T> : public integral_constant<std::size_t, tuple_size<T>::value>
	{

	};
	
	template< typename T >
	class tuple_size<volatile T> : public integral_constant<std::size_t, tuple_size<T>::value>
	{

	};

	template< typename T >
	class tuple_size<const volatile T> : public integral_constant<std::size_t, tuple_size<T>::value>
	{

	};

	template< std::size_t I, typename... Types,typename = enable_if_t< (I < sizeof...(Types)) > >
	inline constexpr tuple_element_t<I, tuple<Types...> >& get(tuple<Types...>&t)
	{
		return static_cast<Internal::make_tuple_type_t<tuple<Types...>,sizeof...(Types),I>&>(t).get();
	}

	template< std::size_t I, typename... Types, typename = enable_if_t<( I < sizeof...(Types))>  >
	inline constexpr const tuple_element_t<I, tuple<Types...> >& get(const tuple<Types...>& t)
	{
		return static_cast<const Internal::make_tuple_type_t<tuple<Types...>, sizeof...(Types), I>&>(t).get();
	}

	template< std::size_t I, typename... Types, typename = enable_if_t< (I < sizeof...(Types)) >  >
	inline constexpr tuple_element_t<I, tuple<Types...> >&& get(tuple<Types...>&& t)
	{
		return static_cast<Internal::make_tuple_type_t<tuple<Types...>, sizeof...(Types), I>&&>(t).this_value.Value;
	}

	template<typename... Args>
	inline constexpr tuple<Args&...> tie(Args&... t) noexcept
	{
		return tuple<Args&...>{t...};
	}

	namespace Internal
	{
		template<typename TypeToFind,
			typename Type,
			std::size_t NowIndex>
		struct get_tuple_by_type_checker;

		template<typename TypeToFind,
			typename Type,
		std::size_t NowIndex>
		struct get_tuple_by_type_impl;

		template<typename TypeToFind,
			typename Type
		>
		struct get_tuple_by_type;

		template<typename TypeToFind,
			typename Type,
			typename... Args,
			std::size_t NowIndex>
		struct get_tuple_by_type_impl < TypeToFind, tuple<Type, Args...>,NowIndex>
		{
			static constexpr std::size_t value = conditional_t <
				is_same<TypeToFind, Type>::value,
				get_tuple_by_type_checker<TypeToFind,tuple<Args...>,NowIndex >,//find it,check. 
				get_tuple_by_type_impl < TypeToFind, tuple<Args...>, NowIndex + 1 >
				>::value;
		};

		template<typename TypeToFind,
			std::size_t NowIndex>
		struct get_tuple_by_type_impl < TypeToFind, tuple<>, NowIndex>
		{
			static constexpr std::size_t value = -1;
		};


		template<typename TypeToFind,
			typename... Args,
			std::size_t NowIndex>
		struct get_tuple_by_type_checker<TypeToFind,tuple<Args...>,NowIndex>
		{
			static_assert(get_tuple_by_type_impl<TypeToFind, tuple<Args...>, 0>::value == -1, "tuple get<>() type duplicate");
			static constexpr std::size_t value = NowIndex;
		};

		template<typename TypeToFind,
			typename... Args
		>
		struct get_tuple_by_type<TypeToFind, tuple<Args...>>
		{
			static constexpr std::size_t value = get_tuple_by_type_impl<
				TypeToFind,
				tuple<Args...>,
				0>::value;
			static_assert(value != -1, "type not found.");
		};
	}
	template< typename Type, typename... Types>
	inline constexpr decltype(auto) get(tuple<Types...>&& t) noexcept
	{
		return Yupei::get<Internal::get_tuple_by_type<Type, tuple<Types...>>::value >(Yupei::forward<tuple<Types...>&&>(t));
	}

	template< typename Type, typename... Types >
	inline constexpr decltype(auto) get(tuple<Types...>& t) noexcept
	{
		return  Yupei::get<Internal::get_tuple_by_type<Type, tuple<Types...>>::value >(t);
	}

	template< typename Type, typename... Types >
	inline constexpr decltype(auto) get(const tuple<Types...>& t) noexcept
	{
		return  Yupei::get<Internal::get_tuple_by_type<Type, tuple<Types...>>::value >(t);
	}

	

	template<typename... Types>
	inline constexpr decltype(auto) make_tuple(Types&&... t)
	{
		return tuple<Internal::tuple_ret_type<Types>...>(Yupei::forward<Types>(t)...);
	}

	template<class... Types>
	inline constexpr tuple<Types&&...> forward_as_tuple(Types&&... t) noexcept
	{
		return tuple<Types&&...>(Yupei::forward<Types>(t)...)
	}

	template<typename T1,typename T2>
	template<class TupleType1,
	class TupleType2,
		std::size_t... Indexes1,
		std::size_t... Indexes2>
		pair<T1, T2>::pair(TupleType1& t1, TupleType2& t2,
			index_sequence<Indexes1...>,
			index_sequence<Indexes2...>)
		:first(Yupei::get<Indexes1>(t1)...),
		second(Yupei::get<Indexes2>(t2)...)
	{

	}

	template<class T1,
	class T2>
		template<class... Types1,
	class... Types2> inline
		pair<T1, T2>::pair(piecewise_construct_t,
			tuple<Types1...> Val1,
			tuple<Types2...> Val2)
		:pair(Val1, Val2, make_index_sequence<sizeof...(Types1)>{}, make_index_sequence<sizeof...(Types2)>{})
	{	
	}
}
   