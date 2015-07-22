#include "..\..\Source\Stl\math.h"
#include "..\..\Source\Stl\Auto.h"
#include "..\..\Source\Stl\memory_resource.h"
#include "..\..\Source\Custom\StopWatch.h"

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <unordered_map>


using namespace Yupei;
using namespace Yupei::Pmr;

template<typename T1,
	typename T2>
	void test(T1& t1, T2& t2)
{
	StopWatch watch;
	watch.Start();
	for (int i = 0; i < 100000;++i)
	{
		t1.push_back(i);
	}
	auto res1 = watch.Stop();
	watch.Start();
	for (int i = 0; i < 100000;++i)
	{
		t2.push_back(i);
	}
	auto res2 = watch.Stop();
	std::cout << "With Yupei's MemoryResource " << res1 << "\n" 
		<<"With new/delete "<< res2 <<  "\n";
}

namespace MyPmr
{
	template <class charT, class traits = std::char_traits<charT>>
	using basic_string =
		std::basic_string<charT, traits, polymorphic_allocator<charT>>;
	typedef basic_string<char> string;
	typedef basic_string<char16_t> u16string;
	typedef basic_string<char32_t> u32string;
	typedef basic_string<wchar_t> wstring;

}

static constexpr size_t LeastStartSize = 1024;//1KB
int main()
{
	auto mono = new monotonic_buffer_resource();
	auto pool = new unsynchronized_pool_resource();
	{
		std::vector<int, polymorphic_allocator<int>> vec1{
			pool };
		std::vector<int> vec2;
		std::cout << "vector with pool \n";
		test(vec1, vec2);
	}

	{
		std::list<int, polymorphic_allocator<int>> lst1{
			pool };
		std::list<int> lst2;
		std::cout << "list with pool \n";
		test(lst1, lst2);
	}

	{
		std::map<int, int,
			std::less<>,
			polymorphic_allocator<int>> map1{
			pool };
		std::map<int, int> map2;
		int str2{};
		/*for (int i = 0; i < 17;++i)
		{
			str2.push_back(i);
		}*/
		StopWatch watch;
		std::cout << "map<int,int> with pool\n";
		watch.Start();
		for (int i = 0; i < 100000;++i)
		{
			map1.insert({ i,str2 });
		}
		auto res1 = watch.Stop();
		watch.Start();
		for (int i = 0; i < 100000;++i)
		{
			map2.insert({ i,str2 });
		}
		auto res2 = watch.Stop();
		std::cout << "With Yupei's MemoryResource " << res1 << "\n"
			<< "With new/delete " << res2 << "\n";
	}

	{
		std::vector<int, polymorphic_allocator<int>> vec1{
			mono };
		std::vector<int> vec2;
		std::cout << "vector with mono \n";
		test(vec1, vec2);
	}

	{
		std::list < std::string
			, polymorphic_allocator < std::string >> lst1{
			pool };
		std::list<std::string> lst2;
		std::string str;
		for (int i = 0; i < 100;++i)
		{
			str.push_back(i);
		}
		StopWatch watch;
		watch.Start();
		for (int i = 0; i < 10000;++i)
		{
			lst1.push_back(str);
		}
		auto res1 = watch.Stop();
		watch.Start();
		for (int i = 0; i < 10000;++i)
		{
			lst2.push_back(str);
		}
		auto res2 = watch.Stop();
		std::cout << "list<std::string>\n" "With Yupei's MemoryResource " << res1 << "\n"
			<< "With new/delete " << res2 <<"\n";
	}

	{
		std::list < MyPmr::string
			, polymorphic_allocator <  MyPmr::string >> lst1{
			pool };
		std::list< MyPmr::string> lst2;
		MyPmr::string str{ mono };
		for (int i = 0; i < 100;++i)
		{
			str.push_back(i);
		}
		StopWatch watch;
		watch.Start();
		for (int i = 0; i < 10000;++i)
		{
			lst1.push_back(str);
		}
		auto res1 = watch.Stop();
		watch.Start();
		for (int i = 0; i < 10000;++i)
		{
			lst2.push_back(str);
		}
		auto res2 = watch.Stop();
		std::cout << "list<Pmr::string>\n" "With Yupei's MemoryResource " << res1 << "\n"
			<< "With new/delete " << res2 << "\n";
	}
	delete pool;
	delete mono;
	//auto x = StaticPow(2, 10);
	/*integral_constant<gcd_type, StaticGetLcm(16, 32)>::value;*/
	_CrtDumpMemoryLeaks();
	std::getchar();

	return 0;
}