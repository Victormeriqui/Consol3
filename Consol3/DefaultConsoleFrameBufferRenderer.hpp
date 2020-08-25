#ifndef DEFAULTCONSOLEFRAMEBUFFERRENDERER_HPP
#define DEFAULTCONSOLEFRAMEBUFFERRENDERER_HPP

#include "AbstractFrameBufferRenderer.hpp"

namespace Display
{
	class DefaultConsoleFrameBufferRenderer : public AbstractFrameBufferRenderer
	{
		void TranslateToConsoleBuffer();
		void DrawConsoleFrame();
	};
}

#endif