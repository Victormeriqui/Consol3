#ifndef GREYSCALEFRAMEBUFFERRENDERER_HPP
#define GREYSCALEFRAMEBUFFERRENDERER_HPP

#include "AbstractFrameBufferRenderer.hpp"

#include "FrameBuffer.hpp"

#include <vector>
#include <wincon.h>
#include <string>

namespace Display
{
	static const COLORREF palette_greyscale[] = {
		0x000000, 0x111111, 0x222222, 0x333333,
		0x444444, 0x555555, 0x666666, 0x777777,
		0x888888, 0x999999, 0xAAAAAA, 0xBBBBBB,
		0xCCCCCC, 0xDDDDDD, 0xEEEEEE, 0xFFFFFF
	};
	
	static const COLORREF palette_default[16] =
	{
		0x000000, 0x800000, 0x008000, 0x808000,
		0x000080, 0x800080, 0x008080, 0xC0C0C0,
		0x808080, 0xFF0000, 0x00FF00, 0xFFFF00,
		0x0000FF, 0xFF00FF, 0x00FFFF, 0xFFFFFF
	};

	class GreyscaleFrameBufferRenderer : public AbstractFrameBufferRenderer
	{
	private:
		std::vector<CHAR_INFO> charbuffer;

		[[nodiscard]] inline uint8_t RGBToGreyscaleIndex(uint32_t color) const;

	public:
		GreyscaleFrameBufferRenderer(const FrameBuffer& framebuffer);

		void TranslateFrameForDrawing(const FrameBuffer& framebuffer);
		void DrawFrame();

		void ReportFPS(uint16_t frame_count);
	};
}

#endif