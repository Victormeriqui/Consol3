#include "VT8BitFrameDrawer.hpp"

#include "ColorMapping.hpp"

namespace Display
{
	VT8BitFrameDrawer::VT8BitFrameDrawer(std::shared_ptr<FrameBuffer<uint8_t>> framebuffer) :
		framebuffer(std::move(framebuffer)),
		console_manager(ConsoleManager(this->framebuffer->GetWidth(), this->framebuffer->GetHeight(), L"Consolas", 4, 4))
	{
		this->framebuffer->FillBuffer(0x000000);

		framebuffer_string	   = std::string(this->framebuffer->GetWidth() * this->framebuffer->GetHeight() * 20, ' ');
		framebuffer_string_len = 0;
	}

	void VT8BitFrameDrawer::SetPixel(uint16_t x, uint16_t y, RGBColor color)
	{
		uint32_t real_color_hex = color.GetHexValues();

		framebuffer->SetValue(x, y, indexed_colors[vt8bit_color_mappting[real_color_hex]].console_color_index);
	}

	void VT8BitFrameDrawer::TranslateFrameBuffer()
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

	void VT8BitFrameDrawer::DisplayFrame()
	{
		TranslateFrameBuffer();
		console_manager.WriteConsoleString(framebuffer_string, framebuffer_string_len);
	}

	void VT8BitFrameDrawer::ReportInformation(const std::string& info)
	{
		console_manager.SetTitle(info);
	}

	void VT8BitFrameDrawer::ClearFrameBuffer()
	{
		this->framebuffer->FillBuffer(0x000000);
	}

	const uint16_t VT8BitFrameDrawer::GetFrameBufferWidth() const
	{
		return framebuffer->GetWidth();
	}

	const uint16_t VT8BitFrameDrawer::GetFrameBufferHeight() const
	{
		return framebuffer->GetHeight();
	}
}
