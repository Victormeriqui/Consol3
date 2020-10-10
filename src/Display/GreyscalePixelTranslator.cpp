#include "GreyscalePixelTranslator.hpp"

#include "HSVColor.hpp"

// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>

namespace Display
{
	CHAR_INFO GreyscalePixelTranslator::TranslatePixelToFrameBuffer(const HSVColor& color) const
	{
		// map it to a 0-15 palette index
		uint8_t index = 16 + (uint8_t)((15.0f * color.value));
		uint8_t background_index = index * 16;

		// in this translator all pixels are a space and we control the color through the background
		return { ' ', background_index };
	}


	const COLORREF* GreyscalePixelTranslator::GetColorPalette() const
	{
		return palette_greyscale;
	}

	std::wstring GreyscalePixelTranslator::GetFontName() const
	{
		return L"Consolas";
	}

	short GreyscalePixelTranslator::GetFontWidth() const
	{
		return 4;
	}

	short GreyscalePixelTranslator::GetFontHeight() const
	{
		return 4;
	}
}