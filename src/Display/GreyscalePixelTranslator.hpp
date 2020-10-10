#ifndef GREYSCALEPIXELTRANSLATOR_HPP
#define GREYSCALEPIXELTRANSLATOR_HPP

#include "IPixelTranslator.hpp"
#include "HSVColor.hpp"

// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>

namespace Display
{
	static const COLORREF palette_greyscale[16] = {
		0x000000, 0x111111, 0x222222, 0x333333,
		0x444444, 0x555555, 0x666666, 0x777777,
		0x888888, 0x999999, 0xAAAAAA, 0xBBBBBB,
		0xCCCCCC, 0xDDDDDD, 0xEEEEEE, 0xFFFFFF
	};

	class GreyscalePixelTranslator : public IPixelTranslator
	{
		virtual CHAR_INFO TranslatePixelToFrameBuffer(const HSVColor& color) const override;

		virtual const COLORREF* GetColorPalette() const override;

		virtual std::wstring GetFontName() const override;
		virtual short GetFontWidth() const override;
		virtual short GetFontHeight() const override;
	};

}

#endif
