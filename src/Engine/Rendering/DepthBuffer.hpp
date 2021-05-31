#ifndef DEPTHBUFFER_HPP
#define DEPTHBUFFER_HPP

#include "../../Display/FrameBuffer.hpp"
#include <xmmintrin.h>

namespace Engine
{
	namespace Rendering
	{
		using namespace Display;
		// using DepthBuffer = Display::FrameBuffer<float>;

		union DepthRow {
			struct
			{
				float x;
				float y;
				float z;
				float w;
			};
			__m128 xyzw;
		};

		class DepthBuffer : public FrameBuffer<float>
		{
			using FrameBuffer::FrameBuffer;

		public:
			[[nodiscard]] DepthRow GetValueRow(uint16_t x, uint16_t y) const
			{
				uint32_t index = x + width * y;
				DepthRow row   = { buffer[index], buffer[index + 1], buffer[index + 2], buffer[index + 3] };
				return row;
			};
		};

	}
}

#endif
