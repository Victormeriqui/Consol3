#include "DitheredFrameBufferRenderer.hpp"

#include "../Engine/Math/Util/MathUtil.hpp"

#include <cstdint>

using namespace Engine::Math;

namespace Display
{
	DitheredFrameBufferRenderer::DitheredFrameBufferRenderer(const FrameBuffer& framebuffer) :
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

		consolemanager.SetPalette(palette_default);
	}


	[[nodiscard]] CHAR_INFO DitheredFrameBufferRenderer::GetDitheredColor(const HSVColor& color) const
	{
		if (color.saturation <= 0.25f)
		{
			uint8_t shade_index = Util::LerpFloatUInt8(color.value, 0, 15);
			return  dithered_white[shade_index];;
		}

		for (const DitheredColor& dithered_color : dithered_colors)
		{
			if (dithered_color.min_hue <= color.hue && color.hue <= dithered_color.max_hue)
			{
				uint8_t shade_index = Util::LerpFloatUInt8(color.value, 0, 10);
				return dithered_color.color_shades[shade_index];
			}
		}

		return dithered_black;
	}


	void DitheredFrameBufferRenderer::TranslateFrameForDrawing(const FrameBuffer& framebuffer)
	{
		const HSVColor* data = framebuffer.GetFrameBufferData();

		for (uint16_t y = 0; y < framebuffer_height; y++)
		{
			for (uint16_t x = 0; x < framebuffer_width; x++)
			{
				CHAR_INFO dithered_pixel = GetDitheredColor(data[x + y * framebuffer_width]);

				charbuffer[x + y * framebuffer_width].Char.AsciiChar = dithered_pixel.Char.AsciiChar;
				charbuffer[x + y * framebuffer_width].Attributes = dithered_pixel.Attributes;
			}
		}
	}

	void DitheredFrameBufferRenderer::DrawFrame()
	{
		consolemanager.FillScreenBuffer(charbuffer.data());
	}

}