#include "DitheredFrameBuffer.hpp"

#include "HSVColor.hpp"
#include "../Math/Util/MathUtil.hpp"

// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>

using namespace Math;

namespace Display
{
	DitheredFrameBuffer::DitheredFrameBuffer(uint16_t width, uint16_t height) : AbstractFrameBuffer(width, height)
	{}

	void DitheredFrameBuffer::SetPixel(uint16_t x, uint16_t y, const HSVColor& color)
	{
		if (x <= 0 || y <= 0 || x >= width || y >= height)
			return;

		// white/black
		if (color.saturation <= 0.25f)
		{
			uint8_t shade_index = Util::LerpToIndex(color.value, 0, 15);

			buffer.data()[x + y * width].Char = dithered_white[shade_index].Char;
			buffer.data()[x + y * width].Attributes = dithered_white[shade_index].Attributes;

			return;
		}

		// select color by hue
		for (const DitheredColor& dithered_color : dithered_colors)
		{
			if (dithered_color.min_hue <= color.hue && color.hue <= dithered_color.max_hue)
			{
				uint8_t shade_index = Util::LerpToIndex(color.value, 0, 10);
			
				buffer.data()[x + y * width].Char = dithered_color.color_shades[shade_index].Char;
				buffer.data()[x + y * width].Attributes = dithered_color.color_shades[shade_index].Attributes;

				return;
			}
		}
	}

	const COLORREF* DitheredFrameBuffer::GetColorPalette() const
	{

		return palette_default;
	}
}