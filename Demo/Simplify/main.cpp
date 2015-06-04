#include "..\..\Source\Stl\memory.h"
#include "..\..\Source\Stl\type_traits.h"

#include <cstdio>

using namespace Yupei;

struct test
{
	int i;
	int foo(...)const &&;
	typedef int value_type;
};
void bar();


int main()
{
	typedef Yupei::allocator_traits<test>::pointer pointer;
	auto x = Yupei::remove_const<const int>::type{};
	Yupei::is_arithmetic<float>::value; //true
	pointer p;
	is_one_of<int, long, char, Yupei::true_type> ::value; //false
	is_array<int>::value; //false
	is_member_function_pointer<decltype(&test::foo)>::value; //true
	is_function<decltype(std::printf)>::value; //true
	is_enum<int>::value; //false
	is_fundamental<nullptr_t>::value; //true
	is_object<decltype(bar)>::value; //false
	is_pointer<int*>::value; //true
	is_const<const int*>::value; //false
	is_assignable<std::unique_ptr<int>&, std::unique_ptr<int>&&>::value;
	is_const<const volatile int>::value; // true
	is_const<const int*>::value; // false
	is_const<const int&>::value;// false
	is_const<int[3]>::value; // false
	is_const<const int[3]>::value; // true

	remove_const<const volatile int>::type;// volatile int
	remove_const<const int* const>::type;// const int*
	remove_const<const int&>::type; // const int&
	remove_const<const int[3]>::type; // int[3]
	//IsMemberFunctionHelper<decltype(&test::foo)>::
}