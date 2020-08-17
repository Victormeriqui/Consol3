#ifndef ABSTRACTFRAMEBUFFERRENDERER_HPP
#define ABSTRACTFRAMEBUFFERRENDERER_HPP

#include "IFrameBuffer.hpp"
#include "IFrameBufferRenderer.hpp"

namespace Engine
{
	namespace Rendering
	{
		class AbstractFrameBufferRenderer : public IFrameBufferRenderer 
		{
		protected:
			IFrameBuffer& framebuffer;
		};

	}
}

#endif