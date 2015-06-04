#pragma once

#include "traits_internal.h"

namespace Yupei
{

//Why ?
#define ADD_CDECL(FUNC,CV_OPT,REF_OPT)\
	FUNC(_cdecl,CV_OPT,REF_OPT)

//https://msdn.microsoft.com/en-us/library/vstudio/b0084kay(v=vs.140).aspx
//for Predefined Macros


//_M_CEE
// Defined for a compilation that uses any form of /clr
// (/clr:oldSyntax, /clr:safe, for example).

#ifdef _M_CEE
#define ADD_CLRCALL(FUNC, CV_OPT, REF_OPT) \
	FUNC(__clrcall, CV_OPT, REF_OPT)

#else /* _M_CEE */
#define ADD_CLRCALL(FUNC, CV_OPT, REF_OPT)
#endif /* _M_CEE */

#if defined(_M_IX86) && !defined(_M_CEE)
#define ADD_FASTCALL(FUNC,CV_OPT,REF_OPT)\
	FUNC(__fastcall,CV_OPT,REF_OPT)
#else
	FUNC(__fastcall, CV_OPT, REF_OPT)
#endif

#if defined(_M_IX86)
#define ADD_STDCALL(FUNC,CV_OPT,REF_OPT)\
	FUNC(__stdcall,CV_OPT,REF_OPT)

#define ADD_THISCALL(FUNC,CV_OPT,REF_OPT)\
	FUNC(__thiscall,CV_OPT,REF_OPT)
#else
#define ADD_STDCALL(FUNC,CV_OPT,REF_OPT)
#define ADD_THISCALL(FUNC,CV_OPT,REF_OPT)
#endif

// _M_IX86_FP 
// Expands to an integer literal value indicating which /arch compiler option was used:
// •0 if /arch:IA32 was used.
// •1 if /arch:SSE was used.
// •2 if /arch:SSE2, /arch:AVX or /arch:AVX2 was used. This value is the default if /arch was not specified. When /arch:AVX is specified, the macro __AVX__ is also defined. When /arch:AVX2 is specified, __AVX__ and __AVX2__ are also defined.
// •See /arch (x86) for more information.

#if ((defined(_M_IX86) && _M_IX86_FP >= 2) \
	|| defined(_M_X64)) && !defined(_M_CEE)
#define ADD_VECTORCALL(FUNC, CV_OPT, REF_OPT) \
	FUNC(__vectorcall, CV_OPT, REF_OPT)

#else /* defined(_M_IX86) && _M_IX86_FP >= 2 etc. */
#define ADD_VECTORCALL(FUNC, CV_OPT, REF_OPT)
#endif /* defined(_M_IX86) && _M_IX86_FP >= 2 etc. */

#define ADD_CONV_TO_MEMBER_FUNC(FUNC, CV_OPT, REF_OPT) \
	ADD_CDECL(FUNC, CV_OPT, REF_OPT) \
	ADD_CLRCALL(FUNC, CV_OPT, REF_OPT) \
	ADD_FASTCALL(FUNC, CV_OPT, REF_OPT) \
	ADD_STDCALL(FUNC, CV_OPT, REF_OPT) \
	ADD_THISCALL(FUNC, CV_OPT, REF_OPT) \
	ADD_VECTORCALL(FUNC, CV_OPT, REF_OPT)

#define FUNCTION_ADAPTER(CALL_OPT,FUNC,REF_OPT)\
	FUNC(CALL_OPT)

#define ADD_CALL_OPT_TO_NONMEMBER_FUNC(FUNC) \
	ADD_CDECL(FUNCTION_ADAPTER, FUNC,) \
	ADD_CLRCALL(FUNCTION_ADAPTER, FUNC,) \
	ADD_FASTCALL(FUNCTION_ADAPTER, FUNC,) \
	ADD_STDCALL(FUNCTION_ADAPTER, FUNC,) \
	ADD_THISCALL(FUNCTION_ADAPTER, FUNC,) \
	ADD_VECTORCALL(FUNCTION_ADAPTER, FUNC,)

#define ADD_CV_TO_MEMBER_FUNC(FUNC , REF_OPT)\
	ADD_CONV_TO_MEMBER_FUNC(FUNC, ,REF_OPT)\
	ADD_CONV_TO_MEMBER_FUNC(FUNC,const ,REF_OPT)\
	ADD_CONV_TO_MEMBER_FUNC(FUNC,volatile ,REF_OPT)\
	ADD_CONV_TO_MEMBER_FUNC(FUNC,const volatile ,REF_OPT)\


#define ADD_REF_CV_TO_MEMBER_FUNC(FUNC) \
	ADD_CV_TO_MEMBER_FUNC(FUNC, )\
	ADD_CV_TO_MEMBER_FUNC(FUNC, &)\
	ADD_CV_TO_MEMBER_FUNC(FUNC, &&)

#define ADD_CV_TO_CLASS(CLASS,REF_OPT)\
	CLASS(const,REF_OPT)\
	CLASS(,REF_OPT)\
	CLASS(volatile,REF_OPT)\
	CLASS(const volatile,REF_OPT)

#define ADD_CV_REF_TO_CLASS(CLASS)\
	ADD_CV_TO_CLASS(CLASS,)\
	ADD_CV_TO_CLASS(CLASS,&)\
	ADD_CV_TO_CLASS(CLASS,&&)

	template<typename Type>
	struct IsMemberFunctionHelper
	{
		using BoolType = false_type;
	};


#define IS_MEMBER_FUNCTION_POINTER(CALL_OPT,CV_OPT,REF_OPT)\
	template<typename R,typename ClassType,typename... Args>\
	struct IsMemberFunctionHelper<R(CALL_OPT ClassType::*)(Args...)CV_OPT REF_OPT> \
{\
	using ClassType = ClassType;\
	using ReturnType = R;	\
	using BoolType = true_type;\
};

	ADD_REF_CV_TO_MEMBER_FUNC(IS_MEMBER_FUNCTION_POINTER)

#define IS_MEMBER_FUNCTION_POINTER_ELLIPSIS(CV_OPT,REF_OPT)\
	template<typename R,typename ClassType,typename... Args>\
	struct IsMemberFunctionHelper<R(ClassType::*)(Args..., ...)CV_OPT REF_OPT> \
{\
	using ClassType = ClassType;\
	using ReturnType = R;	\
	using BoolType = true_type;\
};

	ADD_CV_REF_TO_CLASS(IS_MEMBER_FUNCTION_POINTER_ELLIPSIS)

	template<typename Type>
	struct IsFunctionHelper
	{
		using BoolType = false_type;
	};


#define IS_FUNCTION_(CALL_OPT)\
	template<typename R,typename... Args>\
	struct IsFunctionHelper<R CALL_OPT (Args...)>\
	{\
		using ReturnType = R;\
		using BoolType = true_type;\
	};

	ADD_CALL_OPT_TO_NONMEMBER_FUNC(IS_FUNCTION_)

	template<typename R, typename... Args>
	struct IsFunctionHelper<R(Args..., ...)>
	{
		using ReturnType = R;
		using BoolType = true_type;
	};

	template<typename Type>
	struct is_empty : bool_constant<__is_empty(Type)>
	{

	};

	template<typename Type>
	struct add_const
	{
		using type = const Type;
	};

	template<typename Type>
	struct add_volatile
	{
		using type = volatile Type;
	};

	template<typename Type>
	struct add_cv
	{
		using type = const volatile Type;
	};

	template<typename Type>
	struct add_rvalue_reference
	{
		using type = Type&&;
	};

	template<typename Type>
	using add_rvalue_reference_t = typename add_rvalue_reference<Type>::type;

	template<typename Type>
	struct add_lvalue_reference
	{
		using type = Type&;
	};

	template<typename Type>
	using add_lvalue_reference_t = typename add_lvalue_reference<Type>::type;

	template<typename Type>
	struct remove_extent
	{
		using type = Type;
	};

	template<typename Type>
	struct remove_extent<Type[]>
	{
		using type = Type;
	};

	template<typename Type,size_t Index>
	struct remove_extent<Type[Index]>
	{
		using type = Type;
	};

	template<typename Type>
	struct remove_all_extents
	{
		using type = Type;
	};

	template<typename Type>
	struct remove_all_extents<Type[]>
	{
		using type = typename remove_all_extents<Type>::type;
	};

	template<typename Type,size_t Index>
	struct remove_all_extents<Type[Index]>
	{
		using type = typename remove_all_extents<Type>::type;
	};

	// 20.10.4.1, primary type categories:
	/*template <class T> struct is_void;
	template <class T> struct is_null_pointer;
	template <class T> struct is_integral;
	template <class T> struct is_floating_point;
	template <class T> struct is_array;
	template <class T> struct is_pointer;
	template <class T> struct is_lvalue_reference;
	template <class T> struct is_rvalue_reference;
	template <class T> struct is_member_object_pointer;
	template <class T> struct is_member_function_pointer;
	template <class T> struct is_enum;
	template <class T> struct is_union;
	template <class T> struct is_class;
	template <class T> struct is_function;*/
	/*20.10.4.1 Primary type categories [meta.unary.cat]
	1 The primary type categories correspond to the descriptions given in section 3.9 of the C++ standard.
	2 For any given type T, the result of applying one of these templates to T and to cv-qualified T shall yield the
	same result.
	3 [ Note: For any given type T, exactly one of the primary type categories has a value member that evaluates
	to true. —end note ]*/

	template<typename Type>
	using is_void = is_same<remove_cv_t<Type>, void>;

	template<typename Type>
	using is_null_pointer = is_same<remove_cv_t<Type>, std::nullptr_t>;

	template<typename Type>
	struct is_array : false_type
	{

	};

	template<typename Type>
	struct is_array<Type[]> : true_type
	{

	};

	template<typename Type,std::size_t N>
	struct is_array<Type[N]> : true_type
	{

	};

	
	template<typename Type>
	struct is_lvalue_reference : false_type
	{

	};

	template<typename Type>
	struct is_lvalue_reference<Type&> : true_type
	{

	};

	template<typename Type>
	struct is_rvalue_reference : false_type
	{

	};

	template<typename Type>
	struct is_rvalue_reference<Type&&> : true_type
	{

	};

	template<typename Type>
	struct is_member_object_pointer : false_type
	{

	};

	template<typename ClassType,typename MemberType>
	struct is_member_object_pointer<MemberType ClassType::*> : true_type
	{

	};

	template<typename Type>
	using is_member_function_pointer = typename IsMemberFunctionHelper<Type>::BoolType;

	/*Includes pointers to
	functions but not pointers
	to non-static members.*/
	template<typename Type>
	struct is_pointer : false_type
	{

	};

	template<typename Type>
	struct is_pointer<Type*> : bool_constant<
		!is_member_function_pointer<Type*>::value&&
		!is_member_object_pointer<Type*>::value
	>
	{

	};

	template<typename Type>
	using is_enum = bool_constant< __is_enum(Type)>;

	template<typename Type>
	using is_union = bool_constant< __is_union(Type)>;

	template<typename Type>
	using is_class = bool_constant< __is_class(Type)>;

	template<typename Type>
	using is_function = typename IsFunctionHelper<Type>::BoolType;

	// 20.10.4.2, composite type categories:
	// template <class T> struct is_reference;
	// template <class T> struct is_arithmetic;
	// template <class T> struct is_fundamental;
	// template <class T> struct is_object;
	// template <class T> struct is_scalar;
	// template <class T> struct is_compound;
	// template <class T> struct is_member_pointer;

	template<typename Type>
	using is_reference = integral_constant<
		bool, 
		is_lvalue_reference<Type>::value || 
		is_rvalue_reference<Type>::value
	>;

	//N3797 3.9.1 Fundamental types
	template<typename Type>
	using is_fundamental = integral_constant<
		bool,
		is_arithmetic<Type>::value ||
		is_void<Type>::value ||
		is_null_pointer<Type>::value
	>;

	//N3797 3.9 
	// 8 An object type is a (possibly cv-qualified) type that is not a function type, not a reference type, and not a
	// void type.
	template<typename Type>
	using is_object = bool_constant<
		!is_function<Type>::value&&
		!is_reference<Type>::value&&
		!is_void<Type>::value
	>;

	// T is a pointer to non-static
	// data member or non-static
	// member function
	template<typename Type>
	using is_member_pointer = bool_constant<
		is_member_function_pointer<Type>::value ||
		is_member_object_pointer<Type>::value
	>;

	//N3797 3.9 
	//9 Arithmetic types (3.9.1), enumeration types, pointer types, pointer to member types (3.9.2), 
	//std::nullptr_t, and cv-qualified versions of these types (3.9.3) are collectively called scalar types.
	template<typename Type>
	using is_scalar = bool_constant<
		is_arithmetic<Type>::value ||
		is_enum<Type>::value ||
		is_pointer<Type>::value ||
		is_member_pointer<Type>::value ||
		is_null_pointer<Type>::value
	>;

	template<typename Type>
	using is_compound = bool_constant<
		!is_fundamental<Type>::value
	>;

	//20.10.4.3, type properties :
	// template <class T> struct is_const;
	// template <class T> struct is_volatile;
	// template <class T> struct is_trivial;
	// template <class T> struct is_trivially_copyable;
	// template <class T> struct is_standard_layout;
	// template <class T> struct is_pod;
	// template <class T> struct is_literal_type;
	// template <class T> struct is_empty;
	// template <class T> struct is_polymorphic;
	// template <class T> struct is_abstract;
	// template <class T> struct is_signed;
	// template <class T> struct is_unsigned;
	// template <class T, class... Args> struct is_constructible;
	// template <class T> struct is_default_constructible;
	// template <class T> struct is_copy_constructible;
	// template <class T> struct is_move_constructible;
	// template <class T, class U> struct is_assignable;
	// template <class T> struct is_copy_assignable;
	// template <class T> struct is_move_assignable;
	// template <class T> struct is_destructible;
	// template <class T, class... Args> struct is_trivially_constructible;
	// template <class T> struct is_trivially_default_constructible;
	// template <class T> struct is_trivially_copy_constructible;
	// template <class T> struct is_trivially_move_constructible;
	// template <class T, class U> struct is_trivially_assignable;
	// template <class T> struct is_trivially_copy_assignable;
	// template <class T> struct is_trivially_move_assignable;
	// template <class T> struct is_trivially_destructible;
	// template <class T, class... Args> struct is_nothrow_constructible;
	// template <class T> struct is_nothrow_default_constructible;
	// template <class T> struct is_nothrow_copy_constructible;
	// template <class T> struct is_nothrow_move_constructible;
	// template <class T, class U> struct is_nothrow_assignable;
	// template <class T> struct is_nothrow_copy_assignable;
	// template <class T> struct is_nothrow_move_assignable;
	// template <class T> struct is_nothrow_destructible;
	// template <class T> struct has_virtual_destructor;

	template<typename Type>
	struct is_const : false_type
	{

	};

	template<typename Type>
	struct is_const<const Type> : true_type
	{

	};

	template<typename Type>
	struct is_volatile : false_type
	{

	};

	template<typename Type>
	struct is_volatile<volatile Type> : true_type
	{

	};


	// A trivially copyable class is a class that:
	// — has no non-trivial copy constructors (12.8),
	// — has no non-trivial move constructors (12.8),
	// — has no non-trivial copy assignment operators (13.5.3, 12.8),
	// — has no non-trivial move assignment operators (13.5.3, 12.8), and
	// — has a trivial destructor (12.4).
	//Scalar types, trivially copyable class types(Clause 9), arrays of such types, and non - volatile constqualified
	// versions of these types (3.9.3) are collectively called trivially copyable types.
	template<typename Type>
	using is_trivially_copyable = bool_constant<
		__is_trivially_copyable(Type)
	>;


	// A trivial class is a class that has a default constructor (12.1), has no non-trivial default constructors,
	// and is trivially copyable.
	// [ Note: In particular, a trivially copyable or trivial class does not have virtual functions or virtual base
	// classes.—end note ]

	template<typename Type>
	using is_trivial = bool_constant<__is_trivial(Type)>;

	// 7 A standard-layout class is a class that:
	// — has no non-static data members of type non-standard-layout class (or array of such types) or reference,
	// — has no virtual functions (10.3) and no virtual base classes (10.1),
	// — has the same access control (Clause 11) for all non-static data members,
	// — has no non-standard-layout base classes,
	// — either has no non-static data members in the most derived class and at most one base class with
	// non-static data members, or has no base classes with non-static data members, and
	// — has no base classes of the same type as the first non-static data member.110
	// 8 A standard-layout struct is a standard-layout class defined with the class-key struct or the class-key class.
	// A standard-layout union is a standard-layout class defined with the class-key union.
	// 9 [ Note: Standard-layout classes are useful for communicating with code written in other programming languages.
	// Their layout is specified in 9.2.—end note ]
	//Scalar types, standard-layout class types (Clause 9), arrays of such types and cv-qualified
    // versions of these types (3.9.3) are collectively called standard-layout types.

	template<typename Type>
	using is_standard_layout = bool_constant<
		__is_standard_layout(Type)
	>;

	// 10 A POD struct111 is a non-union class that is both a trivial class and a standard-layout class, and has no
	// non-static data members of type non-POD struct, non-POD union (or array of such types). Similarly, a
	// POD union is a union that is both a trivial class and a standard-layout class, and has no non-static data
	// members of type non-POD struct, non-POD union (or array of such types). A POD class is a class that is
	// either a POD struct or a POD union.
	// [ Example:
	// struct N { // neither trivial nor standard-layout
	//	int i;
	//	int j;
	// virtual ~N();
	// };
	// struct T { // trivial but not standard-layout
	//	int i;
	// private:
	//	int j;
	// };
	// struct SL { // standard-layout but not trivial
	//	int i;
	//	int j;
	//	~SL();
	// };
	// struct POD { // both trivial and standard-layout
	//	int i;
	//	int j;
	// };
	// —end example ]

}