#include "FrameBuffer.hpp"

#include <algorithm>

namespace Display
{
	FrameBuffer::FrameBuffer(uint16_t width, uint16_t height) : width(width), height(height), size(width * height)
	{
		buffer = new uint32_t[size];
	}

	FrameBuffer::~FrameBuffer()
	{
		delete buffer;
	}

	inline void FrameBuffer::SetPixel(uint16_t x, uint16_t y, const Color& color)
	{
		buffer[x + width * y] = color.GetHexValues();
	}

	inline void FrameBuffer::SetPixel(uint16_t x, uint16_t y, uint32_t color)
	{
		buffer[x + width * y] = color;
	}

	const Color& FrameBuffer::GetPixel(uint16_t x, uint16_t y) const
	{
		return buffer[x + width * y];
	}

	void FrameBuffer::ClearBuffer()
	{
		std::fill_n(buffer, size, 0);
	}

	void FrameBuffer::FillBuffer(const Color& color)
	{
		std::fill_n(buffer, size, color.GetHexValues());
	}

	void FrameBuffer::FillBuffer(uint32_t color)
	{
		std::fill_n(buffer, size, color);
	}

}
