#include "DefaultConsoleFrameBufferRenderer.hpp"

#include "FrameBuffer.hpp"

namespace Display
{
	DefaultConsoleFrameBufferRenderer::DefaultConsoleFrameBufferRenderer(const FrameBuffer& framebuffer) : AbstractFrameBufferRenderer(framebuffer)
	{
	}

	void DefaultConsoleFrameBufferRenderer::TranslateToConsoleBuffer()
	{
	}

	void DefaultConsoleFrameBufferRenderer::DrawConsoleFrame()
	{
	}
}
