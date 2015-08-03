#include "..\..\Source\Stl\algorithm.h"
#include <iostream>

int main()
{
	auto lst = { 2,4,1,2,4,100,2,4,3 };
	auto pr = Yupei::minmax(lst);
	auto mv = Yupei::min(lst);
	auto maxv = Yupei::max(lst);
	constexpr auto x = Yupei::static_max(1, 3.0, 2, 1, 3, 2, 9, 5, 3);
	std::cout << pr.first << " " << pr.second_obj;
	std::getchar();
	return 0;
}