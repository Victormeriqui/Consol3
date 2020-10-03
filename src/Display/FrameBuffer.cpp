#include "FrameBuffer.hpp"

#include <algorithm>

namespace Display
{
	FrameBuffer::FrameBuffer(uint16_t width, uint16_t height) : width(width), height(height)
	{
		buffer = std::vector<HSVColor>(width * height, HSVColor());
	}

	uint16_t FrameBuffer::GetWidth() const
	{
		return width;
	}

	uint16_t FrameBuffer::GetHeight() const
	{
		return height;
	}

	void FrameBuffer::SetPixel(uint16_t x, uint16_t y, const HSVColor& color)
	{
		if (x <= 0 || y <= 0 || x >= width || y >= height)
			return;

		buffer.data()[x + width * y].hue = color.hue;
		buffer.data()[x + width * y].saturation = color.saturation;
		buffer.data()[x + width * y].value = color.value;
	}

	HSVColor FrameBuffer::GetPixel(uint16_t x, uint16_t y) const
	{
		return buffer.data()[x + width * y];
	}

	const HSVColor* FrameBuffer::GetFrameBufferData() const
	{
		return buffer.data();
	}

	void FrameBuffer::ClearBuffer()
	{
		FillBuffer(HSVColor());
	}

	void FrameBuffer::FillBuffer(const HSVColor& color)
	{
		HSVColor* data = buffer.data();
		for (uint32_t i = 0; i < buffer.size(); i++)
		{
			data[i].hue = color.hue;
			data[i].saturation = color.saturation;
			data[i].value = color.value;
		}
	}

}
