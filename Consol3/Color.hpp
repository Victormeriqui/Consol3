#ifndef COLOR_HPP
#define COLOR_HPP

#include <cstdint>

namespace Display
{
	class Color
	{
	public:
		uint8_t r;
		uint8_t g;
		uint8_t b;

		constexpr Color() : r(0), g(0), b(0) {}
		constexpr Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
		constexpr Color(uint32_t color) : r((uint8_t)(color >> 16)), g((uint8_t)(color >> 8 & 0xFF)), b((uint8_t)(color & 0xFF)) {}

		[[nodiscard]] inline constexpr uint32_t GetHexValues() const
		{
			return r << 16 | g << 8 | b;
		}

		constexpr Color& SetHexValues(uint32_t color)
		{
			r = (uint8_t)(color >> 16);
			g = (uint8_t)(color >> 8 & 0xFF);
			b = (uint8_t)(color & 0xFF);
		}
	};
}

#endif