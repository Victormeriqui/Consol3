

#ifndef ANSIRENDERER_HPP
#define ANSIRENDERER_HPP

#include "ConsoleManager.hpp"
#include "FrameBuffer.hpp"
#include "HSVColor.hpp"
#include "IRenderer.hpp"
#include "RGBColor.hpp"

// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>
#include <cstdint>
#include <memory>
#include <string>

namespace Display
{
	static const COLORREF palette_ansi[16] = {
		0x000000, 0x800000, 0x008000, 0x808000, 0x000080, 0x800080, 0x008080, 0xC0C0C0,
		0x808080, 0xFF0000, 0x00FF00, 0xFFFF00, 0x0000FF, 0xFF00FF, 0x00FFFF, 0xFFFFFF,
	};

	class ANSIRenderer : public IRenderer
	{
	private:
		std::shared_ptr<FrameBuffer<uint32_t>> framebuffer;
		ConsoleManager console_manager;

		std::string framebuffer_string;
		uint64_t framebuffer_string_len;

		const std::string esc_sequence_start = "\x1b[48;2;";
		const uint8_t esc_sequence_start_len = (uint8_t)esc_sequence_start.length();
		const std::string esc_sequence_end	 = "m";
		const uint8_t esc_sequence_len		 = (uint8_t)esc_sequence_end.length();

		void TranslateFrameBuffer();

	public:
		ANSIRenderer(std::shared_ptr<FrameBuffer<uint32_t>> framebuffer);

		virtual void SetPixel(uint16_t x, uint16_t y, RGBColor color) override;

		virtual void DisplayFrame() override;

		virtual void ClearFrameBuffer() override;

		virtual void ReportInformation(const std::string& info) override;

		[[nodiscard]] virtual const uint16_t GetFrameBufferWidth() const override;
		[[nodiscard]] virtual const uint16_t GetFrameBufferHeight() const override;
	};
}

#endif
