#include "GreyscaleFrameBufferRenderer.hpp"

#include "FrameBuffer.hpp"
#include "Color.hpp"
#include "ConsoleManager.hpp"

#include <vector>
#include <wincon.h>
#include <cstdint>

namespace Display
{
	GreyscaleFrameBufferRenderer::GreyscaleFrameBufferRenderer(const FrameBuffer& framebuffer) :
		AbstractFrameBufferRenderer(
			framebuffer.GetWidth(), 
			framebuffer.GetHeight(),
			framebuffer.GetWidth(),
			framebuffer.GetHeight() / 2, // see charbuffer initialization (the inner ConsoleManager needs to know this ahead of time for the CONSOLE_SCREEN_BUFFER_INFOEX struct
			L"Terminal", // other fonts have bleeding artifacts with lower sizes, this corresponds to "Raster Fonts"
			4,
			6 // effectively 4x3 pixel size, TODO: counteract this with the viewport matrix
		)
	{
		// since the char buffer height is divided by 2 we cant have half a cell
		if ((framebuffer_height & 0x01) == 1)
			throw "Unsupported odd height for framebuffer renderer";

		// we only need half the vertical resolution in char cells, due to the use of the half blocks
		charbuffer = std::vector<CHAR_INFO>(framebuffer_width * (framebuffer_height / 2));

		CHAR_INFO defaultchar;
		defaultchar.Attributes = 0;

		defaultchar.Char.UnicodeChar = (char)223;// upwards half block
		defaultchar.Char.AsciiChar = (char)223;

		// set the character of every cell to the half block, the position inside the cell (up or down) will be controled by the background and foreground colors
		std::fill(charbuffer.begin(), charbuffer.end(), defaultchar);

		// TODO: return to grey scale
		consolemanager.SetPalette(palette_default);
	}

	inline uint8_t GreyscaleFrameBufferRenderer::RGBToGreyscaleIndex(uint32_t color) const
	{
		// color brightness 0f-255f
		float brightness = Color::HexToGreyscale(color);

		// map it to a 0-15 palette index
		uint8_t index = (uint8_t)((15.0f * brightness) / 255.0f);

		return index;
	}

	void GreyscaleFrameBufferRenderer::TranslateFrameForDrawing(const FrameBuffer& framebuffer)
	{
		const uint32_t* data = framebuffer.GetFrameBufferData();

		for (uint16_t y = 0; y < framebuffer_height; y++)
		{
			for (uint16_t x = 0; x < framebuffer_width; x++)
			{
				// the corresponding Y on the charbuffer for this framebuffer pixel
				uint16_t charbuffer_y = y / 2;

				DWORD old_attributes = charbuffer[x + charbuffer_y * framebuffer_width].Attributes;
				uint8_t old_foreground = old_attributes & 0x0F;
				uint8_t old_background = old_attributes & 0xF0;

				bool odd = y & 0x01;

				// convert the pixel to greyscale palette index
				WORD color = RGBToGreyscaleIndex(data[x + y * framebuffer_width]);

				// decide whether to affect the background (downwards halfblock) or the foreground (upwards halfblock)
				if (odd)
					color = color << 4 | old_foreground;
				else
					color = old_background | color;

				// set the final cell attributes
				charbuffer[x + charbuffer_y * framebuffer_width].Attributes = color;
			}
		}
	}

	void GreyscaleFrameBufferRenderer::DrawFrame()
	{
		consolemanager.FillScreenBuffer(charbuffer.data());
	}

}
