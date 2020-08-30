#ifndef WIN10CONSOLEFRAMEBUFFERRENDERER_HPP
#define WIN10CONSOLEFRAMEBUFFERRENDERER_HPP

#include "AbstractFrameBufferRenderer.hpp"

#include "FrameBuffer.hpp"

namespace Display
{
	class Win10ConsoleFrameBufferRenderer : public AbstractFrameBufferRenderer
	{
	public:
		Win10ConsoleFrameBufferRenderer(const FrameBuffer& framebuffer);

		void TranslateToConsoleBuffer(const FrameBuffer& framebuffer);
		void DrawConsoleFrame();
	};
}

#endif