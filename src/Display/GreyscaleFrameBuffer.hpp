#ifndef GREYSCALEFRAMEBUFFER_HPP
#define GREYSCALEFRAMEBUFFER_HPP

#include "AbstractFrameBuffer.hpp"
#include "HSVColor.hpp"

// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>
#include <cstdint>

namespace Display
{
	static const COLORREF palette_greyscale[16] = {
		0x000000, 0x111111, 0x222222, 0x333333,
		0x444444, 0x555555, 0x666666, 0x777777,
		0x888888, 0x999999, 0xAAAAAA, 0xBBBBBB,
		0xCCCCCC, 0xDDDDDD, 0xEEEEEE, 0xFFFFFF
	};

	class GreyscaleFrameBuffer : public AbstractFrameBuffer<CHAR_INFO>
	{
	public:

		GreyscaleFrameBuffer(uint16_t width, uint16_t height);

		virtual void SetPixel(uint16_t x, uint16_t y, const HSVColor& color) override;

		virtual const COLORREF* GetColorPalette() const override;
	};
}

#endif