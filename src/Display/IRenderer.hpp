#ifndef IRENDERER_HPP
#define IRENDERER_HPP

#include "HSVColor.hpp"

// Windows.h overrides std::min
#define NOMINMAX
#include <cstdint>
#include <string>

namespace Display
{
	class IRenderer
	{
	public:
		virtual void SetPixel(uint16_t x, uint16_t y, const HSVColor& color) = 0;

		virtual void ClearFrameBuffer() = 0;
		virtual void DisplayFrame() = 0;

		virtual void ReportInformation(const std::string& info) = 0;

		[[nodiscard]] virtual const uint16_t GetFrameBufferWidth() const = 0;
		[[nodiscard]] virtual const uint16_t GetFrameBufferHeight() const = 0;
	};
}

#endif