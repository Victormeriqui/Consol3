#ifndef DEPTHBUFFER_HPP
#define DEPTHBUFFER_HPP

#include "../../Display/FrameBuffer.hpp"
#include <immintrin.h>

namespace Engine
{
	namespace Rendering
	{
		using namespace Display;
		// using DepthBuffer = Display::FrameBuffer<float>;

		union DepthRow {
			struct
			{
				float v0;
				float v1;
				float v2;
				float v3;
				float v4;
				float v5;
				float v6;
				float v7;
			};
			__m256 row;
		};

		class DepthBuffer : public FrameBuffer<float>
		{
			using FrameBuffer::FrameBuffer;

		public:
			[[nodiscard]] DepthRow GetValueRow(uint16_t x, uint16_t y) const
			{
				uint32_t index = x + width * y;
				DepthRow row   = { buffer[index],	  buffer[index + 1], buffer[index + 2], buffer[index + 3],
								   buffer[index + 4], buffer[index + 5], buffer[index + 6], buffer[index + 7] };
				return row;
			};
		};

	}
}

#endif
