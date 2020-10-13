#include "DitheredRenderer.hpp"

#include "FrameBuffer.hpp"
#include "HSVColor.hpp"
#include "../Math/Util/MathUtil.hpp"

// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>
#include <memory>

namespace Display
{
	using namespace Math;

	DitheredRenderer::DitheredRenderer(std::shared_ptr<FrameBuffer<CHAR_INFO>> framebuffer) :
		framebuffer(framebuffer),
		console_manager(ConsoleManager(framebuffer->GetWidth(), framebuffer->GetHeight(), L"Consolas", 4, 4, palette_default))
	{
		ClearFrameBuffer();
	}

	void DitheredRenderer::SetPixel(uint16_t x, uint16_t y, const HSVColor& color)
	{
		// white/black
		if (color.saturation <= 0.25f)
		{
			uint8_t shade_index = Util::LerpToIndex(color.value, 0, 15);

			framebuffer->SetPixel(x, y, dithered_white[shade_index]);
		}

		// select color by hue
		for (const DitheredColor& dithered_color : dithered_colors)
		{
			if (dithered_color.min_hue <= color.hue && color.hue <= dithered_color.max_hue)
			{
				uint8_t shade_index = Util::LerpToIndex(color.value, 0, 10);

				framebuffer->SetPixel(x, y, dithered_color.color_shades[shade_index]);
			}
		}
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