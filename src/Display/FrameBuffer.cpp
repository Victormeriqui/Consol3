#include "FrameBuffer.hpp"

// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>
#include <algorithm>

namespace Display
{
	FrameBuffer::FrameBuffer(uint16_t width, uint16_t height) : width(width), height(height)
	{
		buffer = std::vector<CHAR_INFO>(width * height, CHAR_INFO());
	}

	uint16_t FrameBuffer::GetWidth() const
	{
		return width;
	}

	uint16_t FrameBuffer::GetHeight() const
	{
		return height;
	}

	void FrameBuffer::SetPixel(uint16_t x, uint16_t y, const CHAR_INFO& chr)
	{
		if (x <= 0 || y <= 0 || x >= width || y >= height)
			return;

		buffer.data()[x + width * y] = chr;
	}

	CHAR_INFO FrameBuffer::GetPixel(uint16_t x, uint16_t y) const
	{
		return buffer.data()[x + width * y];
	}

	const CHAR_INFO* FrameBuffer::GetFrameBufferData() const
	{
		return buffer.data();
	}

	void FrameBuffer::ClearBuffer()
	{
		FillBuffer({' ', 0x00});
	}

	void FrameBuffer::FillBuffer(const CHAR_INFO& chr)
	{
		std::fill(buffer.begin(), buffer.end(), chr);
	}

}
