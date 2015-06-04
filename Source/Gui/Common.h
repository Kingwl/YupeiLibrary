#pragma once


#include <tuple>
#include <string>
#include <limits>
#include <cstdint>
#include <memory>
#include <map>
#include <functional>
#include <vector>
#include <cassert>
#include <cmath>
#include <tchar.h>
#include <Windows.h>
#include <algorithm>
//#include "Utility.h"



#undef max
#undef min


namespace Yupei
{
	/*Integers*/
	using std::int64_t;
	using std::int32_t;
	using std::size_t;

	/*UTF-16 string*/
	using UnicodeString = std::wstring;

	/*Containers*/
	using std::vector;
	using std::map;

	/*Smart Pointers*/
	using std::unique_ptr;
	using std::make_unique;

	/*Algorithm*/
	using std::find;

	/*Tuples*/
	template<typename Type1,typename Type2>
	using pair = std::pair<Type1, Type2>;

	/*Mathmetics Functions*/
	using std::sqrt;
	using std::pow;
	using std::sin;
	using std::abs;
	using std::cos;

	/*Object*/
	using Object = void;

	/*Functional*/
	template<typename FunctionType>
	using Function = std::function<FunctionType>;


	template<typename EnumType>
	constexpr size_t ToIndex(EnumType someThing)
	{
		return static_cast<size_t>(someThing);
	}
	//std::max_el
	enum class HorizontalAlignmentType
	{
		Left,
		Right,
		Centre,
		Stretch
	};

	enum class VerticalAlignmentType
	{
		Top,
		Bottom,
		Centre,
		Stretch
	};

	
	class ISender
	{
	public:
		//virtual UnicodeString ToString() = 0;
	};
}

