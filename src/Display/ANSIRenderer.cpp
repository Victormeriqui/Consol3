#include "ANSIRenderer.hpp"

#include "FrameBuffer.hpp"
#include "HSVColor.hpp"
#include "../Math/Util/MathUtil.hpp"

// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>
#include <cstdint>
#include <memory>

#define PIXEL_SIZE_CHARS 20 // how many chars each pixel takes on the framebuffer_string
#define PIXEL_RED_OFFSET 7 // how many chars away from the start of the pixel string the component is at
#define PIXEL_GREEN_OFFSET 11 // ^
#define PIXEL_BLUE_OFFSET 15 // ^

namespace Display
{
	ANSIRenderer::ANSIRenderer(std::shared_ptr<FrameBuffer<RGBColor>> framebuffer) :
		framebuffer(framebuffer),
		console_manager(ConsoleManager(framebuffer->GetWidth(), framebuffer->GetHeight(), L"Consolas", 8, 8, palette_ansi))
	{
		ClearFrameBuffer();
		framebuffer_string = std::shared_ptr<std::string>(new std::string());
		CreateFrameBufferString();
	}

	void ANSIRenderer::SetPixel(uint16_t x, uint16_t y, const HSVColor& color)
	{
		// TODO: Calculate the rgb value from the HSVColor
		// or... refactor so we can make the engine output whatever we want (hsv or rgb) - the main issue here is how the IRenderer interface would look like
		if (color.value != 0)
			framebuffer->SetValue(x, y, RGBColor(255, 255, 255));
		else
			framebuffer->SetValue(x, y, RGBColor());
	}

	void ANSIRenderer::CreateFrameBufferString()
	{
		// create the stringified framebuffer with all black pixels
		for (uint16_t y = 0; y < framebuffer->GetHeight(); y++)
		{
			for (uint16_t x = 0; x < framebuffer->GetWidth(); x++)
				framebuffer_string->append("\x1b[48;2;000;000;000m ");
		}
	}

	void ANSIRenderer::TranslateFrameBuffer()
	{
		char* string_data = framebuffer_string->data();

		for (uint16_t y = 0; y < framebuffer->GetHeight(); y++)
		{
			for (uint16_t x = 0; x < framebuffer->GetWidth(); x++)
			{
				const RGBColor& color = framebuffer->GetValue(x, y);

				std::string red_string = std::to_string(color.r);
				red_string = std::string(3 - red_string.length(), '0') + red_string;

				std::string green_string = std::to_string(color.g);
				green_string = std::string(3 - green_string.length(), '0') + green_string;

				std::string blue_string = std::to_string(color.b);
				blue_string = std::string(3 - blue_string.length(), '0') + blue_string;

				uint32_t base_offset = (x * PIXEL_SIZE_CHARS) + (y * PIXEL_SIZE_CHARS * framebuffer->GetWidth());
				uint32_t red_offset = base_offset + PIXEL_RED_OFFSET;
				uint32_t green_offset = base_offset + PIXEL_GREEN_OFFSET;
				uint32_t blue_offset = base_offset + PIXEL_BLUE_OFFSET;

				red_string.copy(string_data + red_offset, 3);
				green_string.copy(string_data + green_offset, 3);
				blue_string.copy(string_data + blue_offset, 3);
			}
		}
	}

	void ANSIRenderer::DisplayFrame()
	{
		TranslateFrameBuffer();
		console_manager.WriteConsoleString(framebuffer_string, framebuffer_string->length());
	}

	void ANSIRenderer::ReportInformation(const std::string& info)
	{
		console_manager.SetTitle(info);
	}

	void ANSIRenderer::ClearFrameBuffer()
	{
		this->framebuffer->FillBuffer(RGBColor());
	}

	const uint16_t ANSIRenderer::GetFrameBufferWidth() const
	{
		return framebuffer->GetWidth();
	}

	const uint16_t ANSIRenderer::GetFrameBufferHeight() const
	{
		return framebuffer->GetHeight();
	}
}