#ifndef DITHEREDRENDERER_HPP
#define DITHEREDRENDERER_HPP

#include "IRenderer.hpp"
#include "FrameBuffer.hpp"
#include "HSVColor.hpp"
#include "ConsoleManager.hpp"

// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>
#include <cstdint>
#include <string>
#include <memory>

namespace Display
{
	static const COLORREF palette_default[16] =
	{
		0x000000, 0x800000, 0x008000, 0x808000,
		0x000080, 0x800080, 0x008080, 0xC0C0C0,
		0x808080, 0xFF0000, 0x00FF00, 0xFFFF00,
		0x0000FF, 0xFF00FF, 0x00FFFF, 0xFFFFFF
	};

	// represents a color with all the shades(dither character + palette color), does not include the white shades
	struct DitheredColor
	{
		// defines a hue slice to select the the right color depending on the pixel hue
		float min_hue;
		float max_hue;

		CHAR_INFO color_shades[11];
	};

	static const DitheredColor dithered_red =
	{
		0,
		60,
		{
			{(WCHAR)32, 0x00},
			{(WCHAR)176, 0x04},
			{(WCHAR)177, 0x04},
			{(WCHAR)177, 0x40},
			{(WCHAR)176, 0x40},
			{(WCHAR)32, 0x40},
			{(WCHAR)176, 0x4C},
			{(WCHAR)177, 0x4C},
			{(WCHAR)177, 0xC4},
			{(WCHAR)176, 0xC4},
			{(WCHAR)32, 0xC4}
		}
	};

	static const DitheredColor dithered_green =
	{
		61,
		120,
		{
			{(WCHAR)32, 0x00},
			{(WCHAR)176, 0x02},
			{(WCHAR)177, 0x02},
			{(WCHAR)177, 0x20},
			{(WCHAR)176, 0x20},
			{(WCHAR)32, 0x20},
			{(WCHAR)176, 0x2A},
			{(WCHAR)177, 0x2A},
			{(WCHAR)177, 0xA2},
			{(WCHAR)176, 0xA2},
			{(WCHAR)32, 0xA2}
		}
	};

	static const DitheredColor dithered_blue =
	{
		121,
		180,
		{
			{(WCHAR)32, 0x00},
			{(WCHAR)176, 0x01},
			{(WCHAR)177, 0x01},
			{(WCHAR)177, 0x10},
			{(WCHAR)176, 0x10},
			{(WCHAR)32, 0x10},
			{(WCHAR)176, 0x19},
			{(WCHAR)177, 0x19},
			{(WCHAR)177, 0x91},
			{(WCHAR)176, 0x91},
			{(WCHAR)32, 0x91}
		}
	};

	static const DitheredColor dithered_yellow =
	{
		181,
		240,
		{
			{(WCHAR)32, 0x00},
			{(WCHAR)176, 0x06},
			{(WCHAR)177, 0x06},
			{(WCHAR)177, 0x60},
			{(WCHAR)176, 0x60},
			{(WCHAR)32, 0x60},
			{(WCHAR)176, 0x6E},
			{(WCHAR)177, 0x6E},
			{(WCHAR)177, 0xE6},
			{(WCHAR)176, 0xE6},
			{(WCHAR)32, 0xE6}
		}
	};

	static const DitheredColor dithered_cyan =
	{
		241,
		300,
		{
			{(WCHAR)32, 0x00},
			{(WCHAR)176, 0x03},
			{(WCHAR)177, 0x03},
			{(WCHAR)177, 0x30},
			{(WCHAR)176, 0x30},
			{(WCHAR)32, 0x30},
			{(WCHAR)176, 0x3B},
			{(WCHAR)177, 0x3B},
			{(WCHAR)177, 0xB3},
			{(WCHAR)176, 0xB3},
			{(WCHAR)32, 0xB3}
		}
	};

	static const DitheredColor dithered_magenta =
	{
		301,
		360,
		{
			{(WCHAR)32, 0x00},
			{(WCHAR)176, 0x05},
			{(WCHAR)177, 0x05},
			{(WCHAR)177, 0x50},
			{(WCHAR)176, 0x50},
			{(WCHAR)32, 0x50},
			{(WCHAR)176, 0x5D},
			{(WCHAR)177, 0x5D},
			{(WCHAR)177, 0xD5},
			{(WCHAR)176, 0xD5},
			{(WCHAR)32, 0xD5}
		}
	};

	static const CHAR_INFO dithered_white[16] = {
		{(WCHAR)32, 0x00},
		{(WCHAR)176, 0x08},
		{(WCHAR)177, 0x08},
		{(WCHAR)177, 0x80},
		{(WCHAR)176, 0x80},
		{(WCHAR)32, 0x80},
		{(WCHAR)176, 0x87},
		{(WCHAR)177, 0x87},
		{(WCHAR)177, 0x78},
		{(WCHAR)176, 0x78},
		{(WCHAR)32, 0x78},
		{(WCHAR)176, 0x7F},
		{(WCHAR)177, 0x7F},
		{(WCHAR)177, 0xF7},
		{(WCHAR)176, 0xF7},
		{(WCHAR)32, 0xF7},
	};

	static const CHAR_INFO dithered_black = { (WCHAR)32, 0x00 };

	// excludes white and black (not determined by hue)
	static const std::vector<DitheredColor> dithered_colors = { dithered_red, dithered_green, dithered_blue, dithered_yellow, dithered_cyan, dithered_magenta };

	class DitheredRenderer : public IRenderer
	{
	private:
		std::shared_ptr<FrameBuffer<CHAR_INFO>> framebuffer;
		ConsoleManager console_manager;

	public:
		DitheredRenderer(std::shared_ptr<FrameBuffer<CHAR_INFO>> framebuffer);

		virtual void SetPixel(uint16_t x, uint16_t y, const HSVColor& color) override;

		virtual void DisplayFrame() override;

		virtual void ClearFrameBuffer() override;
		
		virtual void ReportInformation(const std::string& info) override;

		[[nodiscard]] virtual const uint16_t GetFrameBufferWidth() const override;
		[[nodiscard]] virtual const uint16_t GetFrameBufferHeight() const override;
	};
}

#endif