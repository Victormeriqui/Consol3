#include "Win10ConsoleFrameBufferRenderer.hpp"

#include "FrameBuffer.hpp"

namespace Display
{
	Win10ConsoleFrameBufferRenderer::Win10ConsoleFrameBufferRenderer(const FrameBuffer& framebuffer) :
		AbstractFrameBufferRenderer(
			framebuffer.GetWidth(),
			framebuffer.GetHeight(),
			framebuffer.GetWidth(),
			framebuffer.GetHeight() / 2, // see charbuffer initialization (the inner ConsoleManager needs to know this ahead of time for the CONSOLE_SCREEN_BUFFER_INFOEX struct
			L"Terminal", // other fonts have bleeding artifacts with lower sizes, this corresponds to "Raster Fonts"
			4,
			6 // effectively 4x3 pixel size, TODO: counteract this with the viewport matrix
		)
	{
	}

	void Win10ConsoleFrameBufferRenderer::TranslateToConsoleBuffer(const FrameBuffer& framebuffer)
	{
	}

	void Win10ConsoleFrameBufferRenderer::DrawConsoleFrame()
	{
	}
}
