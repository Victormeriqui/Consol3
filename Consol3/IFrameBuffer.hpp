#ifndef IFRAMEBUFFER_HPP
#define IFRAMEBUFFER_HPP

#include <stdint.h>

namespace Engine
{
	namespace Rendering
	{
		class IFrameBuffer
		{

		public:
			IFrameBuffer() {};
			~IFrameBuffer() {};

			virtual void SetPixel(uint16_t x, uint16_t y, void* data) = 0;
			virtual void* GetPixel(uint16_t x, uint16_t y) = 0;

			virtual void ClearBuffer() = 0;
			virtual void FillBuffer(void* data) = 0;
		};

	}
}


#endif