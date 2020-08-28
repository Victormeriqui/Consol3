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
		const uint16_t size;

	public:

		FrameBuffer(uint16_t width, uint16_t height);


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
		[[nodiscard]] Color GetPixel(uint16_t x, uint16_t y) const;

		void ClearBuffer();
		void FillBuffer(const Color& color);
		void FillBuffer(uint32_t color);
	};

}

#endif