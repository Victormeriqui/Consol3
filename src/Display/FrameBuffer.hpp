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
		uint16_t width;
		uint16_t height;

		std::vector<T> buffer;

	public:
		FrameBuffer() : width(0), height(0)
		{
			buffer = std::vector<T>(width * height, T());
		}

		FrameBuffer(uint16_t width, uint16_t height) : width(width), height(height)
		{
			buffer = std::vector<T>(width * height, T());
		}

		void SetValue(uint16_t x, uint16_t y, const T& value)
		{
			buffer.data()[x + width * y] = value;
		}

		[[nodiscard]] T GetValue(uint16_t x, uint16_t y) const
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
