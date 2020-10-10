#include "DitheredPixelTranslator.hpp"

#include "HSVColor.hpp"
#include "../Math/Util/MathUtil.hpp"

// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>

using namespace Math;

namespace Display
{
	CHAR_INFO DitheredPixelTranslator::TranslatePixelToFrameBuffer(const HSVColor& color) const
	{
		// white/black
		if (color.saturation <= 0.25f)
		{
			uint8_t shade_index = Util::LerpToIndex(color.value, 0, 15);
			return dithered_white[shade_index];;
		}

		for (const DitheredColor& dithered_color : dithered_colors)
		{
			if (dithered_color.min_hue <= color.hue && color.hue <= dithered_color.max_hue)
			{
				uint8_t shade_index = Util::LerpToIndex(color.value, 0, 10);
				return dithered_color.color_shades[shade_index];
			}
		}

		return dithered_black;
	}

	const COLORREF* DitheredPixelTranslator::GetColorPalette() const
	{
		return palette_default;
	}

	std::wstring DitheredPixelTranslator::GetFontName() const
	{
		return L"Consolas";
	}

	short DitheredPixelTranslator::GetFontWidth() const
	{
		return 4;
	}

	short DitheredPixelTranslator::GetFontHeight() const
	{
		return 4;
	}
}