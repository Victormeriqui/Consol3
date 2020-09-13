#include "FrameBuffer.hpp"

#include <algorithm>

namespace Display
{
	FrameBuffer::FrameBuffer(uint16_t width, uint16_t height) : width(width), height(height)
	{
		buffer = std::vector<uint32_t>(width * height, 0xffffff);
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
		// TODO: remove this, this is just for easier testing
		if (x >= width || x < 0 || y >= height || y < 0)
			return;

		buffer.data()[x + width * y] = color.GetHexValues();
	}

	void FrameBuffer::SetPixel(uint16_t x, uint16_t y, uint32_t color)
	{
		// TODO: remove this, this is just for easier testing
		if (x >= width || x < 0 || y >= height || y < 0)
			return;

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
		FillBuffer(0x000000);
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
