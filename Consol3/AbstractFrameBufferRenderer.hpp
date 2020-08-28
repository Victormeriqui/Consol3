#ifndef ABSTRACTFRAMEBUFFERRENDERER_HPP
#define ABSTRACTFRAMEBUFFERRENDERER_HPP

#include "FrameBuffer.hpp"
#include "ConsoleBuffer.hpp"

namespace Display
{
	class AbstractFrameBufferRenderer 
	{
	protected:
		const ConsoleBuffer consolebuffer;

		AbstractFrameBufferRenderer() : consolebuffer(ConsoleBuffer()) {}

		/**
		* Translates the given frame buffer to the inner consolebuffer held by this class
		* After translation the consolebuffer is ready to drawn to the console
		*/
		virtual void TranslateToConsoleBuffer(const FrameBuffer& framebuffer) = 0;
		virtual void DrawConsoleFrame() = 0;
	};

}

#endif