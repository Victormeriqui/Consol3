#include "VT24BitRenderer.hpp"

namespace Display
{
	VT24BitRenderer::VT24BitRenderer(std::shared_ptr<FrameBuffer<uint32_t>> framebuffer) :
		framebuffer(std::move(framebuffer)),
		console_manager(ConsoleManager(this->framebuffer->GetWidth(), this->framebuffer->GetHeight(), L"Consolas", 4, 4))
	{
		this->framebuffer->FillBuffer(0x000000);

		framebuffer_string	   = std::string(this->framebuffer->GetWidth() * this->framebuffer->GetHeight() * 20, ' ');
		framebuffer_string_len = 0;
	}

	void VT24BitRenderer::SetPixel(uint16_t x, uint16_t y, RGBColor color)
	{
		framebuffer->SetValue(x, y, color.GetHexValues());
	}

	void VT24BitRenderer::TranslateFrameBuffer()
	{
		uint32_t last_color			  = 0x000000;
		uint64_t current_string_index = 0;

		for (uint16_t y = 0; y < framebuffer->GetHeight(); y++)
		{
			for (uint16_t x = 0; x < framebuffer->GetWidth(); x++)
			{
				uint32_t current_color = framebuffer->GetValue(x, y);

				// always set color on the first pixel
				if (current_color != last_color || (y == 0 && x == 0))
				{
					RGBColor rgbcurrent_color = RGBColor(current_color);

					std::string red_string = std::to_string(rgbcurrent_color.r) + ";";
					uint8_t red_string_len = (uint8_t)red_string.length();

					std::string green_string = std::to_string(rgbcurrent_color.g) + ";";
					uint8_t green_string_len = (uint8_t)green_string.length();

					std::string blue_string = std::to_string(rgbcurrent_color.b);
					uint8_t blue_string_len = (uint8_t)blue_string.length();

					esc_sequence_start.copy(framebuffer_string.data() + current_string_index, esc_sequence_start_len, 0);
					current_string_index += esc_sequence_start_len;

					red_string.copy(framebuffer_string.data() + current_string_index, red_string_len, 0);
					current_string_index += red_string_len;

					green_string.copy(framebuffer_string.data() + current_string_index, green_string_len, 0);
					current_string_index += green_string_len;

					blue_string.copy(framebuffer_string.data() + current_string_index, blue_string_len, 0);
					current_string_index += blue_string_len;

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

	void VT24BitRenderer::DisplayFrame()
	{
		TranslateFrameBuffer();
		console_manager.WriteConsoleString(framebuffer_string, framebuffer_string_len);
	}

	void VT24BitRenderer::ReportInformation(const std::string& info)
	{
		console_manager.SetTitle(info);
	}

	void VT24BitRenderer::ClearFrameBuffer()
	{
		this->framebuffer->FillBuffer(0x000000);
	}

	const uint16_t VT24BitRenderer::GetFrameBufferWidth() const
	{
		return framebuffer->GetWidth();
	}

	const uint16_t VT24BitRenderer::GetFrameBufferHeight() const
	{
		return framebuffer->GetHeight();
	}
}
