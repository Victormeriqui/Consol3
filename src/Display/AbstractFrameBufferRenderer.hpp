#ifndef ABSTRACTFRAMEBUFFERRENDERER_HPP
#define ABSTRACTFRAMEBUFFERRENDERER_HPP

#include "FrameBuffer.hpp"
#include "ConsoleManager.hpp"

namespace Display
{
	class AbstractFrameBufferRenderer 
	{
	protected:
		ConsoleManager consolemanager;
		uint16_t framebuffer_width;
		uint16_t framebuffer_height;

		AbstractFrameBufferRenderer(uint16_t framebuffer_width, 
				uint16_t framebuffer_height,
				short charbuffer_width, short
				charbuffer_height,
				std::wstring font_name,
				short font_width,
				short font_height) :
			consolemanager(
				ConsoleManager(
					charbuffer_width,
					charbuffer_height,
					font_name,
					font_width,
					font_height
				)
			),
			framebuffer_width(framebuffer_width),
			framebuffer_height(framebuffer_height)
		{}

		/**
		* Translates the given frame buffer to the inner representation needed by the FrameBufferRenderer class
		* After translation the frame is ready to drawn
		*/
		virtual void TranslateFrameForDrawing(const FrameBuffer& framebuffer) = 0;
		virtual void DrawFrame() = 0;
	};

}

#endif