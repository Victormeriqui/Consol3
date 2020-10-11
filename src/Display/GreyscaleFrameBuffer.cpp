#include "GreyscaleFramebuffer.hpp"

#include "HSVColor.hpp"

// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>
#include <cstdint>
#include <vector>

namespace Display
{
	GreyscaleFrameBuffer::GreyscaleFrameBuffer(uint16_t width, uint16_t height) : AbstractFrameBuffer(width, height)
	{
		// ensure that every char is a space, that way we can just control the color
		ClearBuffer();
	}

	void GreyscaleFrameBuffer::SetPixel(uint16_t x, uint16_t y, const HSVColor& color)
	{
		if (x <= 0 || y <= 0 || x >= width || y >= height)
			return;

		// map it to a 0-15 palette index
		uint8_t index = 16 + (uint8_t)((15.0f * color.value));
		uint8_t background_index = index * 16;

		// in greyscale all pixels are a space and we control the color through the background
		buffer.data()[x + y * width].Attributes = background_index;
	}

	const COLORREF* GreyscaleFrameBuffer::GetColorPalette() const
	{
		return palette_greyscale;
	}
}