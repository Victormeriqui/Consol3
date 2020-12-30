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
		console_manager(ConsoleManager(framebuffer->GetWidth(), framebuffer->GetHeight(), L"Consolas", 4, 4, palette_dithered))
	{
		ClearFrameBuffer();
	}

	void DitheredRenderer::SetPixel(uint16_t x, uint16_t y, const HSVColor& color)
	{
		// white/black
		if (color.saturation <= 0.25f)
		{
			uint8_t shade_index = Util::LerpCast<uint8_t>(color.value, 0, 15);

			framebuffer->SetValue(x, y, dithered_white[shade_index]);
			return;
		}

		// TODO: this shouldn't be a linear interpolation as the shade_indexes are not linear
		// the shade amount should be pre calculated and this should be made into a binary search
		uint8_t shade_index = Util::LerpCast<uint8_t>(color.value, 0, 10);

		// special red case because it wraps over the 360 mark
		if (color.hue >= dithered_red.min_hue || color.hue <= dithered_red.max_hue)
			framebuffer->SetValue(x, y, dithered_red.color_shades[shade_index]);
		
		// select color by hue
		for (const DitheredColor& dithered_color : sequential_dithered_colors)
		{
			if (dithered_color.min_hue <= color.hue && color.hue <= dithered_color.max_hue)
			{
				framebuffer->SetValue(x, y, dithered_color.color_shades[shade_index]);
				break;
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