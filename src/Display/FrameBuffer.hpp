#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "HSVColor.hpp"

#include <cstdint>
#include <vector>

namespace Display
{
	template<typename T>
	class FrameBuffer
	{
	protected:
		const uint16_t width;
		const uint16_t height;

		std::vector<T> buffer;

	public:

		FrameBuffer(uint16_t width, uint16_t height) : width(width), height(height)
		{
			buffer = std::vector<T>(width * height, T());
		}

		void SetPixel(uint16_t x, uint16_t y, const T& value)
		{
			// TODO: figure out why this is needed, this check shouldnt be necessary if we have correct clipping
			if (x < 0 || x >= width || y < 0 || y >= height)
				return;

			buffer.data()[x + width * y] = value;
		}

		[[nodiscard]] T GetPixel(uint16_t x, uint16_t y) const
		{
			return buffer[x + width * y];
		};

		[[nodiscard]] uint16_t GetWidth() const
		{
			return width;
		}

		[[nodiscard]] uint16_t GetHeight() const
		{
			return height;
		}

		[[nodiscard]] const T* GetFrameBufferData() const
		{
			return buffer.data();
		}

		void FillBuffer(const T& value)
		{
			std::fill(buffer.begin(), buffer.end(), value);
		}
	};
}

#endif