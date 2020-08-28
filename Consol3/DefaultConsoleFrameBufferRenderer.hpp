#ifndef DEFAULTCONSOLEFRAMEBUFFERRENDERER_HPP
#define DEFAULTCONSOLEFRAMEBUFFERRENDERER_HPP

#include "AbstractFrameBufferRenderer.hpp"

#include "FrameBuffer.hpp"

namespace Display
{
	class DefaultConsoleFrameBufferRenderer : public AbstractFrameBufferRenderer
	{
	public:
		DefaultConsoleFrameBufferRenderer();

		void TranslateToConsoleBuffer(const FrameBuffer& framebuffer);
		void DrawConsoleFrame();
	};
}

#endif