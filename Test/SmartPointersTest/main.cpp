#include "..\..\Source\Stl\memory.h"
#include "..\..\Source\Stl\type_traits.h"
#include "..\..\Source\Stl\compress_pair.h"

template<bool B>
struct bar
{

};


int main()
{
	Yupei::compress_pair<
		Yupei::allocator<int>,
		int> p( Yupei::compress_value_initialize_first_arg,2);
	auto ptr = p.first().allocate(20, nullptr);
	return 0;
}