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
	constexpr auto y = Yupei::is_arithmetic<float>::value;
	pointer p;
	constexpr auto x = Yupei::is_one_of<int, long, char, Yupei::true_type> ::value;
	constexpr auto x = is_array<int>::value;
	constexpr auto x = is_member_function_pointer<decltype(&test::foo)>::value;
	constexpr auto x = is_function<decltype(std::printf)>::value;
	constexpr auto x = is_enum<int>::value;
	constexpr auto x = is_fundamental<nullptr_t>::value;
	constexpr auto x = is_object<decltype(bar)>::value;
	constexpr auto x = is_pointer<int*>::value;
	constexpr auto x = is_const<const int*>::value;
	//IsMemberFunctionHelper<decltype(&test::foo)>::
}