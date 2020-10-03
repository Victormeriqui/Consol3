#include "GreyscaleFrameBufferRenderer.hpp"

#include "FrameBuffer.hpp"
#include "HSVColor.hpp"
#include "ConsoleManager.hpp"

#include <vector>
#include <wincon.h>
#include <cstdint>
#include <string>

namespace Display
{
	GreyscaleFrameBufferRenderer::GreyscaleFrameBufferRenderer(const FrameBuffer& framebuffer) :
		AbstractFrameBufferRenderer(
			framebuffer.GetWidth(), 
			framebuffer.GetHeight(),
			framebuffer.GetWidth(),
			framebuffer.GetHeight(),
			L"Consolas",
			4,
			4
		)
	{
		charbuffer = std::vector<CHAR_INFO>(framebuffer_width * framebuffer_height);

		CHAR_INFO defaultchar;
		defaultchar.Attributes = 0;

		defaultchar.Char.UnicodeChar = (char)32;// space
		defaultchar.Char.AsciiChar = (char)32;

		// set every character to space
		std::fill(charbuffer.begin(), charbuffer.end(), defaultchar);

		consolemanager.SetPalette(palette_greyscale);
	}

	inline uint8_t GreyscaleFrameBufferRenderer::HSVToGreyscaleIndex(const HSVColor& color) const
	{	
		// map it to a 0-15 palette index
		uint8_t index = 16 + (uint8_t)((15.0f * color.value));
		uint8_t background_index = index * 16;

		return background_index;
	}

	void GreyscaleFrameBufferRenderer::TranslateFrameForDrawing(const FrameBuffer& framebuffer)
	{
		const HSVColor* data = framebuffer.GetFrameBufferData();

		for (uint16_t y = 0; y < framebuffer_height; y++)
		{
			for (uint16_t x = 0; x < framebuffer_width; x++)
			{
				// convert the pixel to greyscale palette index
				WORD color = HSVToGreyscaleIndex(data[x + y * framebuffer_width]);

				// set the final cell attributes
				charbuffer[x + y * framebuffer_width].Attributes = color;
			}
		}
	}

	void GreyscaleFrameBufferRenderer::DrawFrame()
	{
		consolemanager.FillScreenBuffer(charbuffer.data());
	}

}
