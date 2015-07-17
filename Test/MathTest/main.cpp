#include "..\..\Source\Stl\math.h"
#include "..\..\Source\Stl\Auto.h"
#include "..\..\Source\Stl\memory_resource.h"
#include "..\..\Source\Custom\StopWatch.h"

#include <iostream>
#include <vector>


using namespace Yupei;
using namespace Yupei::Pmr;

static constexpr size_t LeastStartSize = 1024;//1KB
int main()
{
	/*Auto([]()
	{
		std::abort();
	}());*/
	std::vector<int, polymorphic_allocator<int>> vec1{ new unsynchronized_pool_resource(new monotonic_buffer_resource()) };
	std::vector<int> vec2;
	StopWatch watch;
	watch.Start();
	for (int i = 0; i < 10000000;++i)
	{
		vec1.push_back(i);
	}
	auto res1 = watch.Stop();
	watch.Start();
	for (int i = 0; i < 10000000;++i)
	{
		vec2.push_back(i);
	}
	auto res2 = watch.Stop();
	//auto x = StaticPow(2, 10);
	std::cout << res1 << " " << res2;/*integral_constant<gcd_type, StaticGetLcm(16, 32)>::value;*/
	std::getchar();
	return 0;
}