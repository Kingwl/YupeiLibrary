#pragma once

#include <dwrite.h>
#include <wingdi.h>

namespace Yupei
{
	/*enum DWRITE_FONT_WEIGHT						// GDI Font Weight
	{												#define FW_DONTCARE         0
		DWRITE_FONT_WEIGHT_THIN = 100,              #define FW_THIN             100   
		DWRITE_FONT_WEIGHT_EXTRA_LIGHT = 200,		#define FW_EXTRALIGHT       200
		DWRITE_FONT_WEIGHT_ULTRA_LIGHT = 200,		#define FW_ULTRALIGHT       FW_EXTRALIGHT
		DWRITE_FONT_WEIGHT_LIGHT = 300,				#define FW_LIGHT            300
		DWRITE_FONT_WEIGHT_SEMI_LIGHT = 350,		
		DWRITE_FONT_WEIGHT_NORMAL = 400,			#define FW_NORMAL           400
		DWRITE_FONT_WEIGHT_REGULAR = 400,			#define FW_REGULAR          FW_NORMAL
		DWRITE_FONT_WEIGHT_MEDIUM = 500,			#define FW_MEDIUM           500
		DWRITE_FONT_WEIGHT_DEMI_BOLD = 600,			#define FW_DEMIBOLD         FW_SEMIBOLD
		DWRITE_FONT_WEIGHT_SEMI_BOLD = 600,			#define FW_SEMIBOLD         600
		DWRITE_FONT_WEIGHT_BOLD = 700,				#define FW_BOLD             700
		DWRITE_FONT_WEIGHT_EXTRA_BOLD = 800,		#define FW_EXTRABOLD        800
		DWRITE_FONT_WEIGHT_ULTRA_BOLD = 800,		#define FW_ULTRABOLD        FW_EXTRABOLD
		DWRITE_FONT_WEIGHT_BLACK = 900,				#define FW_BLACK            FW_HEAVY
		DWRITE_FONT_WEIGHT_HEAVY = 900,				#define FW_HEAVY            900
		DWRITE_FONT_WEIGHT_EXTRA_BLACK = 950,
		DWRITE_FONT_WEIGHT_ULTRA_BLACK = 950
	};*/
	//看起来GDI与DWrite一样，那就好办了！
	
	enum class FontWeight
	{
		Thin = 100,
		ExtraLight = 200,
		UltraLight = 200,
		Light = 300,
		/*SemiLight = 350*/
		Normal = 400,
		Regular = 400,
		Medium = 500,
		DemiBold = 600,
		SemiBold = 600,
		Bold = 700,
		ExtraBold = 800,
		UltraBold = 800,
		Black = 900,
		Heavy = 900
		/*ExtraBlack = 950,
		UltraBlack - 950*/
	};

	const wchar_t* const DefaultFontFamily = L"Microsoft YaHei";
	const float DefaultFontSize = 16.f;
	const FontWeight DefaultFontWeight = FontWeight::Normal;

	struct TextRange
	{
		uint32_t startPosition = 0u;
		uint32_t charactersCount = 0u;
		DWRITE_TEXT_RANGE GetDWriteTextRange() const noexcept
		{
			return{ startPosition,charactersCount };
		}
	};

	class ITextLayout
	{
		//virtual
	};


}
