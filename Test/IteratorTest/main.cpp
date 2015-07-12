#include "..\..\Source\Stl\iterator.h"
#include <vector>
#include <memory>

int main()
{
	int i;
	Yupei::IteratorCategory<int*>{};
	Yupei::ValueType<int*>{};
	Yupei::iterator_traits<int*>::difference_type{};
	Yupei::iterator_traits<int*>::pointer x{};
	Yupei::iterator_traits<std::vector<int>::iterator>::reference ri = i;
	//Yupei::iterator_traits<Yupei::ostream_iterator<int>>::pointer i;
	ri = 4;
	//Yupei::po
	//Yupei::declval<std::unique_ptr<int>>().operator->();
	return 0;
}