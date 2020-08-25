#ifndef ABSTRACTFRAMEBUFFERRENDERER_HPP
#define ABSTRACTFRAMEBUFFERRENDERER_HPP

#include "FrameBuffer.hpp"
#include "ConsoleBuffer.hpp"

namespace Display
{
	class AbstractFrameBufferRenderer 
	{
	protected:
		const FrameBuffer& framebuffer;
		const ConsoleBuffer consolebuffer;

		AbstractFrameBufferRenderer(const FrameBuffer& framebuffer) : framebuffer(framebuffer), consolebuffer(ConsoleBuffer()) {}

		virtual void TranslateToConsoleBuffer() = 0;
		virtual void DrawConsoleFrame() = 0;
	};

}

#endif