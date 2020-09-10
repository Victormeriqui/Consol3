#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "Color.hpp"

#include <vector>
#include <cstdint>

namespace Display
{
	class FrameBuffer
	{
	private:
		std::vector<uint32_t> buffer;

		const uint16_t width;
		const uint16_t height;

	public:

		FrameBuffer(uint16_t width, uint16_t height);

		[[nodiscard]] uint16_t GetWidth() const;
		[[nodiscard]] uint16_t GetHeight() const;

		/**
		* Sets the framebuffer value at x, y.
		* The bounds are NOT checked
		*/
		void SetPixel(uint16_t x, uint16_t y, const Color& color);
		/**
		* Sets the framebuffer value at x, y.
		* The bounds are NOT checked
		*/
		void SetPixel(uint16_t x, uint16_t y, uint32_t color);
		/**
		* Gets the framebuffer value at x, y.
		* The bounds are NOT checked
		*/
		[[nodiscard]] Color GetPixel(uint16_t x, uint16_t y) const;

		[[nodiscard]] const uint32_t* GetFrameBufferData() const;

		void ClearBuffer();
		void FillBuffer(const Color& color);
		void FillBuffer(uint32_t color);
	};

}

#endif