#ifndef TextOnlyFrameDrawer_HPP
#define TextOnlyFrameDrawer_HPP

#include "ConsoleManager.hpp"
#include "FrameBuffer.hpp"
#include "HSVColor.hpp"
#include "IFrameDrawer.hpp"

// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>
#include <cstdint>
#include <memory>
#include <string>

namespace Display
{
	static const COLORREF palette_textonly[16] = { 0x000000, 0x111111, 0x212121, 0x333333, 0x444444, 0x565656, 0x666666, 0x777777,
												   0x898989, 0x999999, 0xAAAAAA, 0xBCBCBC, 0xCCCCCC, 0xDDDDDD, 0xEFEFEF, 0xFFFFFF };

	class TextOnlyFrameDrawer : public IFrameDrawer
	{
	private:
		std::shared_ptr<FrameBuffer<CHAR_INFO>> framebuffer;
		ConsoleManager console_manager;

		const std::string shades;
		const uint8_t shades_count;

	public:
		TextOnlyFrameDrawer(std::shared_ptr<FrameBuffer<CHAR_INFO>> framebuffer);

		virtual void SetPixel(uint16_t x, uint16_t y, RGBColor color) override;

		virtual void DisplayFrame() override;

		virtual void ClearFrameBuffer() override;

		virtual void ReportInformation(const std::string& info) override;

		[[nodiscard]] virtual const uint16_t GetFrameBufferWidth() const override;
		[[nodiscard]] virtual const uint16_t GetFrameBufferHeight() const override;
	};
}

#endif
