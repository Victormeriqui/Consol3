#include "DitheredGreyscaleRenderer.hpp"

#include "../Math/Util/MathUtil.hpp"

#include <utility>
#include <algorithm>
#include <map>
#include <cstdint>

namespace Display
{
	using namespace Math;

	DitheredGreyscaleRenderer::DitheredGreyscaleRenderer(std::shared_ptr<FrameBuffer<CHAR_INFO>> framebuffer) :
		framebuffer(std::move(framebuffer)),
		console_manager(ConsoleManager(this->framebuffer->GetWidth(), this->framebuffer->GetHeight(), L"Consolas", 4, 4, palette_dithered_greyscale)),
		shade_map()
	{
		ClearFrameBuffer();
		ComputeShadeMap();
	}

	std::pair<uint8_t, uint8_t> DitheredGreyscaleRenderer::GetClosestIndexInPalette(uint8_t shade) const
	{
		uint8_t low_idx = 0;
		uint8_t high_idx = 15;

		while (low_idx <= high_idx)
		{
			uint8_t mid = (low_idx + high_idx) / 2;

			if (shade < palette_shades[mid])
				high_idx = mid - 1;
			else if (shade > palette_shades[mid])
				low_idx = mid + 1;
			else
				return std::pair<uint8_t, uint8_t>(mid, mid);
		}

		// high and low are swapped here since it went one over
		return std::pair<uint8_t, uint8_t>(high_idx, low_idx);
	}

	void DitheredGreyscaleRenderer::ComputeShadeMap()
	{
		for (uint16_t color_value = 0; color_value <= 255; color_value++)
		{
			uint8_t shade = (uint8_t)color_value;

			std::pair<uint8_t, uint8_t> closest = GetClosestIndexInPalette(shade);

			// exact match
			if (closest.first == closest.second)
				shade_map[shade] = { 32, (WORD)(closest.first * 16) };

			uint8_t low_idx = closest.first;
			uint8_t low_shade = palette_shades[low_idx];
			uint8_t high_idx = closest.second;
			uint8_t high_shade = palette_shades[high_idx];

			float shade_progress = (float)(shade - low_shade) / (high_shade - low_shade);

			WORD attribute = (low_idx << 4) | (0x0F & high_idx);

			// shaded blocks are: 25%, 50% and 75%
			// each of these corresponds to the domain of each shaded block between two colors in the palette
			// which is exactly 25% for each one
			if (shade_progress > 0 && shade_progress < 0.125f)
				shade_map[shade] = { 32, (WORD)(low_idx * 16) };
			else if (shade_progress >= 0.125f && shade_progress < 0.376f)
				shade_map[shade] = { 176, attribute };
			else if (shade_progress >= 0.375f && shade_progress < 0.625f)
				shade_map[shade] = { 177, attribute };
			else if (shade_progress >= 0.625f && shade_progress < 0.875f)
				shade_map[shade] = { 178, attribute };
			else if (shade_progress >= 0.875f && shade_progress < 1.0f)
				shade_map[shade] = { 32, (WORD)(high_idx * 16) };
		}
	}

	void DitheredGreyscaleRenderer::SetPixel(uint16_t x, uint16_t y, const HSVColor& color)
	{
		uint8_t shade = Util::LerpCast<uint8_t>(color.value, 0, 255);
		framebuffer->SetValue(x, y, shade_map[shade]);
	}

	void DitheredGreyscaleRenderer::DisplayFrame()
	{
		console_manager.FillScreenBuffer(framebuffer->GetFrameBufferData());
	}

	void DitheredGreyscaleRenderer::ReportInformation(const std::string& info)
	{
		console_manager.SetTitle(info);
	}

	void DitheredGreyscaleRenderer::ClearFrameBuffer()
	{
		this->framebuffer->FillBuffer({ ' ', 0x00 });
	}

	const uint16_t DitheredGreyscaleRenderer::GetFrameBufferWidth() const
	{
		return framebuffer->GetWidth();
	}

	const uint16_t DitheredGreyscaleRenderer::GetFrameBufferHeight() const
	{
		return framebuffer->GetHeight();
	}
}