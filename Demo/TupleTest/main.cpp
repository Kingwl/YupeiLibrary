#include "..\..\Source\Stl\tuple.h"
#include <iostream>

//template<typename T,
//typename...Args,
//std::enable_if_t<
//	sizeof...(Args) != 0 ||
//	(sizeof...(Args) == 0 && !std::is_same<std::decay_t<T>, int>::value) >>
//void foo(T&& t, Args... args)
//{
//	std::cout << 1;
//}
//
//void foo(const int& i)
//{
//	std::cout << 2;
//}

int main()
{
	Yupei::tuple<int, char,float,int,int> t1{ 1,2 };
	auto t2 = t1;
	//auto x = Yupei::get<1>(t1);


	return 0;
}