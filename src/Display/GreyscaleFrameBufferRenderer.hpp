#ifndef GREYSCALEFRAMEBUFFERRENDERER_HPP
#define GREYSCALEFRAMEBUFFERRENDERER_HPP

#include "AbstractFrameBufferRenderer.hpp"

#include "FrameBuffer.hpp"

#include <vector>
#include <wincon.h>
#include <string>

namespace Display
{
	static const COLORREF palette_greyscale[16] = {
		0x000000, 0x111111, 0x222222, 0x333333,
		0x444444, 0x555555, 0x666666, 0x777777,
		0x888888, 0x999999, 0xAAAAAA, 0xBBBBBB,
		0xCCCCCC, 0xDDDDDD, 0xEEEEEE, 0xFFFFFF
	};


	class GreyscaleFrameBufferRenderer : public AbstractFrameBufferRenderer
	{
	private:
		std::vector<CHAR_INFO> charbuffer;

		[[nodiscard]] inline uint8_t HSVToGreyscaleIndex(const HSVColor& color) const;

	public:
		GreyscaleFrameBufferRenderer(const FrameBuffer& framebuffer);

		void TranslateFrameForDrawing(const FrameBuffer& framebuffer);
		void DrawFrame();

	};
}

#endif