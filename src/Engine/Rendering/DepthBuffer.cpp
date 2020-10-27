#include "DepthBuffer.hpp"

#include <cstdint>
#include <algorithm>
#include <limits>
#include <vector>

namespace Engine
{
	namespace Rendering
	{
		DepthBuffer::DepthBuffer(uint16_t width, uint16_t height) : width(width), height(height)
		{
			buffer = std::vector<float>(width * height, std::numeric_limits<float>::max());
		}

		void DepthBuffer::SetDepth(uint16_t x, uint16_t y, float depth)
		{
			if (x < 0 || x >= width || y < 0 || y >= height)
				return;

			buffer.data()[x + y * width] = depth;
		}

		[[nodiscard]] float DepthBuffer::GetDepth(uint16_t x, uint16_t y) const
		{
			if (x < 0 || x >= width || y < 0 || y >= height)
				return -1;

			return buffer.data()[x + y * width];
		};

		[[nodiscard]] uint16_t DepthBuffer::GetWidth() const
		{
			return width;
		}

		[[nodiscard]] uint16_t DepthBuffer::GetHeight() const
		{
			return height;
		}

		[[nodiscard]] const float* DepthBuffer::GetDeptBufferData() const
		{
			return buffer.data();
		}

		void DepthBuffer::ClearBuffer()
		{
			FillBuffer(std::numeric_limits<float>::max());
		}

		void DepthBuffer::FillBuffer(float value)
		{
			std::fill(buffer.begin(), buffer.end(), value);
		}
	}
}