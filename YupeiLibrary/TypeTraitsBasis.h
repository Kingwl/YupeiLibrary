#pragma once

#include "Basic.h"

namespace Yupei
{
	template<typename T, T V>
	struct integral_constant
	{
		static constexpr T value = V;
		using value_type = T;
		using type = integral_constant<T, V>;

		constexpr operator value_type () const noexcept
		{
			return value;
		}

		constexpr value_type operator ()() const noexcept
		{
			return value;
		}
	};

	using true_type = integral_constant<bool, true>;
	using false_type = integral_constant<bool, false>;

	template<bool TestVal,
		typename Type = void>
	struct enable_if
	{

	};

	template<typename Type>
	struct enable_if <true, Type>
	{
		using type = Type;
	};

	template<bool TestVal,
		typename Type1,
		typename Type2>
	struct conditional
	{
		using type = Type2;
	};

	template<typename Type1,
		typename Type2>
	struct conditional<true,Type1,Type2>
	{
		using type = Type1;
	};

	template<bool TestVal,
		typename Type1,
		typename Type2>
		using conditional_t = typename conditional<TestVal, Type1, Type2>::type;

	template<typename Type1,
		typename Type2>
	struct is_same : public false_type
	{

	};

	template<typename Type>
	struct is_same<Type,Type> : public true_type
	{

	};

#define REMOVE_TOGETHER(CV) remove_##CV

#define REMOVE_HELPER(CV)			\
	template<typename Type>			\
	struct REMOVE_TOGETHER(CV)		\
	{								\
		using type = Type;			\
	};								\
									\
	template<typename Type>			\
	struct REMOVE_TOGETHER(CV)<CV Type>\
	{								\
		using type = Type;			\
	};								\
									\
	template<typename Type>			\
	struct REMOVE_TOGETHER(CV)<CV Type[]>\
	{\
		using type = Type[];		\
	};\
	\
	template<typename Type,size_t Index>	\
	struct REMOVE_TOGETHER(CV)<CV Type[Index]>\
	{								\
		using type = Type[Index];	\
	};

	REMOVE_HELPER(const)
	REMOVE_HELPER(volatile)

	template<typename Type>
	struct remove_cv
	{
		using type = typename remove_const<typename
			remove_volatile<Type>::type>::type;
	};

	template<typename Type>
	using remove_cv_t = typename remove_cv<Type>::type;

	template<typename T>
	struct remove_reference
	{
		using type = T;
	};

	template<typename T>
	struct remove_reference<T&>
	{
		using type = T;
	};

	template<typename T>
	struct remove_reference<T&&>
	{
		using type = T;
	};

	template<typename T>
	using remove_reference_t = typename remove_reference<T>::type;

	namespace Extension
	{

		//for testing;
		template<bool value, typename Type1, typename Type2>
		struct TypeSelector
		{
			using type = Type2;
		};

		template<typename Type1, typename Type2>
		struct TypeSelector<true, Type1, Type2>
		{
			using type = Type1;
		};

		struct WrapInt
		{
			WrapInt(int) {};
		};

		template<typename Type>
		struct Identity
		{
			using type = Type;
		};

#define DEFAULT_OR_OTHER(DefaultType) \
		{\
			template<typename T>\
			static auto _TestFunction(int)->Extension::Identity<typename T::DefaultType>;\
			template<typename T>\
			static auto _TestFunction(Extension::WrapInt)->Extension::Identity<OtherType>;\
			using temp_type = decltype(_TestFunction<ParentType>(0));\
			using type = typename temp_type::type;\
		}

		template<typename ParentType, typename OtherType>
		struct GetPointer
			DEFAULT_OR_OTHER(pointer);

		template<typename ParentType, typename OtherType>
		using GetPointerType = typename GetPointer<ParentType, OtherType>::type;
	}

	template<typename Type , typename... Args>
	struct is_one_of;

	template<typename Type>
	struct is_one_of<Type> : false_type
	{

	};

	template<typename Type,typename... Args>
	struct is_one_of<Type, Type, Args...> : true_type
	{

	};

	template<typename Type,typename P,typename... Args>
	struct is_one_of<Type,P,Args...> :
		is_one_of<Type,Args...>
	{

	};

	template<typename Type>
	using is_integral = is_one_of<remove_cv_t<Type>,
		bool,
		char,
		signed char,
		unsigned char,
		signed short,
		unsigned short,
		signed int,
		unsigned int,
		signed long,
		unsigned long,
		long long,
		unsigned long long>;

	template<typename Type>
	using is_floating_point = is_one_of<
		remove_cv_t<Type>,
		float,
		double,
		long double>;


	template<typename Type>
	struct is_arithmetic :
		conditional_t<is_integral<Type>::value || is_floating_point<Type>::value, true_type, false_type>
	{

	};

};
