#ifndef ABSTRACTFRAMEBUFFER_HPP
#define ABSTRACTFRAMEBUFFER_HPP

#include "HSVColor.hpp"

// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>
#include <cstdint>
#include <vector>

namespace Display
{
	template<typename T>
	class AbstractFrameBuffer
	{
	protected:
		const uint16_t width;
		const uint16_t height;

		std::vector<T> buffer;

	public:

		AbstractFrameBuffer(uint16_t width, uint16_t height) : width(width), height(height)
		{
			buffer = std::vector<T>(width * height, T());
		}

		virtual void SetPixel(uint16_t x, uint16_t y, const HSVColor& color) = 0;

		virtual const COLORREF* GetColorPalette() const = 0;


		[[nodiscard]] T GetPixel(uint16_t x, uint16_t y) const
		{
			return buffer.data()[x + width * y];
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

		void ClearBuffer()
		{
			FillBuffer({ ' ', 0x00 });
		}
		
		void FillBuffer(const T& value)
		{
			std::fill(buffer.begin(), buffer.end(), value);
		}
	};
}

#endif
