#ifndef GREYSCALERENDERER_HPP
#define GREYSCALERENDERER_HPP

#include "IRenderer.hpp"
#include "HSVColor.hpp"
#include "ConsoleManager.hpp"
#include "FrameBuffer.hpp"

// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>
#include <cstdint>
#include <memory>

namespace Display
{
	static const COLORREF palette_greyscale[16] = {
		0x000000, 0x111111, 0x222222, 0x333333,
		0x444444, 0x555555, 0x666666, 0x777777,
		0x888888, 0x999999, 0xAAAAAA, 0xBBBBBB,
		0xCCCCCC, 0xDDDDDD, 0xEEEEEE, 0xFFFFFF
	};

	class GreyscaleRenderer : public IRenderer
	{
	private:
		std::shared_ptr<FrameBuffer<CHAR_INFO>> framebuffer;
		ConsoleManager console_manager;

	public:

		GreyscaleRenderer(std::shared_ptr<FrameBuffer<CHAR_INFO>> framebuffer);

		virtual void SetPixel(uint16_t x, uint16_t y, const HSVColor& color) override;

		virtual void DisplayFrame() override;

		virtual void ClearFrameBuffer() override;

		virtual void ReportInformation(const std::string& info) override;

		[[nodiscard]] virtual const uint16_t GetFrameBufferWidth() const override;
		[[nodiscard]] virtual const uint16_t GetFrameBufferHeight() const override;
	};
}

#endif