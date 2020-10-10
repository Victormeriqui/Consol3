#ifndef IPIXELTRANSLATOR_HPP
#define IPIXELTRANSLATOR_HPP

#include "FrameBuffer.hpp"
#include "HSVColor.hpp"

// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>
#include <cstdint>
#include <string>

namespace Display
{
	class IPixelTranslator
	{
	public:
		virtual CHAR_INFO TranslatePixelToFrameBuffer(const HSVColor& color) const = 0;

		virtual const COLORREF* GetColorPalette() const = 0;
	
		virtual std::wstring GetFontName() const = 0;
		virtual short GetFontWidth() const = 0;
		virtual short GetFontHeight() const = 0;
	};
}

#endif
