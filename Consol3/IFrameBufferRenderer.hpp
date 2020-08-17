#ifndef IFRAMEBUFFERRENDERER_HPP
#define IFRAMEBUFFERRENDERER_HPP

namespace Engine 
{
	namespace Rendering
	{
		class IFrameBufferRenderer
		{
		protected:
			virtual void GenerateConsoleBuffer() = 0;

		public:
			IFrameBufferRenderer() {}
			~IFrameBufferRenderer() {}

			virtual void RenderFrameBuffer() = 0;

		};

	}
}

#endif