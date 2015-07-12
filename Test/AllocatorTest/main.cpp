#include "..\..\Source\Stl\memory.h"
#include <memory>

int main()
{
	Yupei::pointer_traits<int*>::element_type{};
	Yupei::pointer_traits<std::shared_ptr<int>>::element_type;
	return 0;
}