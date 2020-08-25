#ifndef WIN10CONSOLEFRAMEBUFFERRENDERER_HPP
#define WIN10CONSOLEFRAMEBUFFERRENDERER_HPP

#include "AbstractFrameBufferRenderer.hpp"

namespace Display
{
	class Win10ConsoleFrameBufferRenderer : public AbstractFrameBufferRenderer
	{
		void TranslateToConsoleBuffer();
		void DrawConsoleFrame();
	};
}

#endif