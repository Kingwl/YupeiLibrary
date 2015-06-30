#include "..\..\Source\Stl\tuple.h"
#include <iostream>
#include <vector>
#include <memory>
#include <string>

#pragma warning(disable:4101)
#pragma warning(disable:4091)
#pragma warning(disable:4189)

template<typename T1,
	typename T2>
	void foo(Yupei::pair<T1, T2>& p)
{
	//static_assert(Yupei::is_lvalue_reference<decltype(p.first)>::value, "haha");
}

int main()
{
	Yupei::tuple<int, char,float,int,int> t1{ 1,2 };
	auto t2 = t1;
	Yupei::tuple<int, char, float, int, int> t4{ 2,1 };
	swap(t1, t4);
	int a;
	char b;
	Yupei::tuple<int, char,std::string> t5{ 2,3,"hahah" };
	Yupei::tie(a, b,Yupei::ignore) = t5;
	std::cout << (t1 <= t4);
	auto x = Yupei::get<3>(t1);
	Yupei::tuple<std::vector<int>, std::vector<int>> t3{ Yupei::allocator_arg,std::allocator<int>{} };
	Yupei::get<0>(t3).push_back(3);
	Yupei::reference_wrapper<int> li(a);
	auto t6 = Yupei::make_tuple(li, b);
	Yupei::is_trivially_copyable<Yupei::reference_wrapper<int>>::value;
	auto f = Yupei::get<char>(t1);
	Yupei::pair<int, char> p;
	auto py = Yupei::get<int>(p);
	auto px = Yupei::get<0>(p);
	p = Yupei::make_pair(2, 3);
	foo(p);
	return 0;
}