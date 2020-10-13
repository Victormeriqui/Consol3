#include "GreyscaleRenderer.hpp"

#include "FrameBuffer.hpp"
#include "HSVColor.hpp"

// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>
#include <cstdint>
#include <memory>

namespace Display
{
	GreyscaleRenderer::GreyscaleRenderer(std::shared_ptr<FrameBuffer<CHAR_INFO>> framebuffer) :
		framebuffer(framebuffer),
		console_manager(ConsoleManager(framebuffer->GetWidth(), framebuffer->GetHeight(), L"Consolas", 4, 4, palette_greyscale))
	{
		// ensure that every char is a space, that way we can just control the color
		ClearFrameBuffer();
	}

	void GreyscaleRenderer::SetPixel(uint16_t x, uint16_t y, const HSVColor& color)
	{
		// map it to a 0-15 palette index
		uint8_t index = 16 + (uint8_t)((15.0f * color.value));
		uint8_t background_index = index * 16;

		// in greyscale all pixels are a space and we control the color through the background
		framebuffer->SetPixel(x, y, { ' ', background_index });
	}

	void GreyscaleRenderer::DisplayFrame()
	{
		console_manager.FillScreenBuffer(framebuffer->GetFrameBufferData());
	}

	void GreyscaleRenderer::ReportInformation(const std::string& info)
	{
		console_manager.SetTitle(info);
	}

	void GreyscaleRenderer::ClearFrameBuffer()
	{
		this->framebuffer->FillBuffer({ ' ', 0x00 });
	}

	const uint16_t GreyscaleRenderer::GetFrameBufferWidth() const
	{
		return framebuffer->GetWidth();
	}

	const uint16_t GreyscaleRenderer::GetFrameBufferHeight() const
	{
		return framebuffer->GetHeight();
	}

}