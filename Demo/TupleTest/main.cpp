#include "..\..\Source\Stl\tuple.h"
#include <iostream>
#include <vector>
#include <memory>
#include <string>


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
	return 0;
}