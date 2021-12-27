#include "VT8BitRenderer.hpp"

namespace Display
{
	VT8BitRenderer::VT8BitRenderer(std::shared_ptr<FrameBuffer<uint8_t>> framebuffer) :
		framebuffer(std::move(framebuffer)),
		console_manager(ConsoleManager(this->framebuffer->GetWidth(), this->framebuffer->GetHeight(), L"Consolas", 4, 4, palette_ansi1))
	{
		ClearFrameBuffer();
		GenerateLookupTable();

		framebuffer_string	   = std::string(this->framebuffer->GetWidth() * this->framebuffer->GetHeight() * 20, ' ');
		framebuffer_string_len = 0;
	}

	void VT8BitRenderer::GenerateLookupTable()
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

					for (uint8_t i = 0; i < indexed_colors_len; i++)
					{
						float dist = color.GetColorDistance(indexed_colors[i].real_color);

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

	void VT8BitRenderer::SetPixel(uint16_t x, uint16_t y, RGBColor color)
	{
		uint32_t real_color_hex = color.GetHexValues();
		framebuffer->SetValue(x, y, indexed_colors[color_lookup_table[real_color_hex]].console_color_index);
	}

	void VT8BitRenderer::TranslateFrameBuffer()
	{
		uint8_t last_color			  = 0x000000;
		uint64_t current_string_index = 0;

		for (uint16_t y = 0; y < framebuffer->GetHeight(); y++)
		{
			for (uint16_t x = 0; x < framebuffer->GetWidth(); x++)
			{
				uint8_t current_color = framebuffer->GetValue(x, y);

				// always set color on the first pixel
				if (current_color != last_color || (y == 0 && x == 0))
				{
					std::string index_str = std::to_string(current_color);
					uint8_t index_str_len = (uint8_t)index_str.length();

					esc_sequence_start.copy(framebuffer_string.data() + current_string_index, esc_sequence_start_len, 0);
					current_string_index += esc_sequence_start_len;

					index_str.copy(framebuffer_string.data() + current_string_index, index_str_len, 0);
					current_string_index += index_str_len;

					esc_sequence_end.copy(framebuffer_string.data() + current_string_index, esc_sequence_len, 0);
					current_string_index += esc_sequence_len;

					last_color = current_color;
				}

				framebuffer_string.data()[current_string_index++] = ' ';
			}

			framebuffer_string.data()[current_string_index++] = '\n';
		}

		framebuffer_string.data()[current_string_index++] = '\0';

		framebuffer_string_len = current_string_index;
	}

	void VT8BitRenderer::DisplayFrame()
	{
		TranslateFrameBuffer();
		console_manager.WriteConsoleString(framebuffer_string, framebuffer_string_len);
	}

	void VT8BitRenderer::ReportInformation(const std::string& info)
	{
		console_manager.SetTitle(info);
	}

	void VT8BitRenderer::ClearFrameBuffer()
	{
		this->framebuffer->FillBuffer(0x000000);
	}

	const uint16_t VT8BitRenderer::GetFrameBufferWidth() const
	{
		return framebuffer->GetWidth();
	}

	const uint16_t VT8BitRenderer::GetFrameBufferHeight() const
	{
		return framebuffer->GetHeight();
	}
}
