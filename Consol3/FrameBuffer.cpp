#include "FrameBuffer.hpp"

#include <algorithm>

namespace Display
{
	FrameBuffer::FrameBuffer(uint16_t width, uint16_t height) : width(width), height(height)
	{
		buffer = std::vector<uint32_t>(width * height);
	}

	uint16_t FrameBuffer::GetWidth() const
	{
		return width;
	}

	uint16_t FrameBuffer::GetHeight() const
	{
		return height;
	}

	void FrameBuffer::SetPixel(uint16_t x, uint16_t y, const Color& color)
	{
		buffer.data()[x + width * y] = color.GetHexValues();
	}

	void FrameBuffer::SetPixel(uint16_t x, uint16_t y, uint32_t color)
	{
		buffer.data()[x + width * y] = color;
	}

	Color FrameBuffer::GetPixel(uint16_t x, uint16_t y) const
	{
		return Color(buffer.data()[x + width * y]);
	}

	const uint32_t* FrameBuffer::GetFrameBufferData() const
	{
		return buffer.data();
	}

	void FrameBuffer::ClearBuffer()
	{
		buffer.clear();
	}

	void FrameBuffer::FillBuffer(const Color& color)
	{
		std::fill(buffer.begin(), buffer.end(), color.GetHexValues());
	}

	void FrameBuffer::FillBuffer(uint32_t color)
	{
		std::fill(buffer.begin(), buffer.end(), color);
	}

}
