#include "DitheredRenderer.hpp"

#include "../Math/Util/MathUtil.hpp"

namespace Display
{
	using namespace Math;

	DitheredRenderer::DitheredRenderer(std::shared_ptr<FrameBuffer<CHAR_INFO>> framebuffer) :
		framebuffer(std::move(framebuffer)),
		console_manager(ConsoleManager(this->framebuffer->GetWidth(), this->framebuffer->GetHeight(), L"Consolas", 4, 4))
	{
		ClearFrameBuffer();
		GenerateLookupTable();
	}

	void DitheredRenderer::GenerateLookupTable()
	{
		for (uint16_t b = 0; b < 256; b++)
		{
			for (uint16_t g = 0; g < 256; g++)
			{
				for (uint16_t r = 0; r < 256; r++)
				{
					RGBColor color = RGBColor((uint8_t)r, (uint8_t)g, (uint8_t)b);

					float closest_dist	= 999;
					uint8_t closest_idx = 0;

					for (uint8_t i = 0; i < dithered_colors_len; i++)
					{
						float dist = color.GetColorDistance(dithered_colors[i].real_color);

						if (dist < closest_dist)
						{
							closest_dist = dist;
							closest_idx	 = i;

							if (dist == 0)
								break;
						}
					}

					uint32_t real_color_hex			   = color.GetHexValues();
					color_lookup_table[real_color_hex] = closest_idx;
				}
			}
		}
	}

	void DitheredRenderer::SetPixel(uint16_t x, uint16_t y, RGBColor color)
	{
		uint32_t real_color_hex = color.GetHexValues();

		framebuffer->SetValue(x, y, dithered_colors[color_lookup_table[real_color_hex]].console_color);
	}

	void DitheredRenderer::DisplayFrame()
	{
		console_manager.FillScreenBuffer(framebuffer->GetFrameBufferData());
	}

	void DitheredRenderer::ReportInformation(const std::string& info)
	{
		console_manager.SetTitle(info);
	}

	void DitheredRenderer::ClearFrameBuffer()
	{
		this->framebuffer->FillBuffer({ ' ', 0x00 });
	}

	const uint16_t DitheredRenderer::GetFrameBufferWidth() const
	{
		return framebuffer->GetWidth();
	}

	const uint16_t DitheredRenderer::GetFrameBufferHeight() const
	{
		return framebuffer->GetHeight();
	}
}
