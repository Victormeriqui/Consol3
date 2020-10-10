#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>
#include <vector>
#include <cstdint>

namespace Display
{
	class FrameBuffer
	{
	private:
		std::vector<CHAR_INFO> buffer;

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
		void SetPixel(uint16_t x, uint16_t y, const CHAR_INFO& chr);
		/**
		* Gets the framebuffer value at x, y.
		* The bounds are NOT checked
		*/
		[[nodiscard]] CHAR_INFO GetPixel(uint16_t x, uint16_t y) const;

		[[nodiscard]] const CHAR_INFO* GetFrameBufferData() const;

		void ClearBuffer();
		void FillBuffer(const CHAR_INFO& chr);
	};

}

#endif