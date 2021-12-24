#ifndef RGBCOLOR_HPP
#define RGBCOLOR_HPP

#include <cstdint>

namespace Display
{
	class RGBColor
	{
	public:
		uint8_t r;
		uint8_t g;
		uint8_t b;

		constexpr RGBColor() : r(0), g(0), b(0)
		{
		}
		constexpr RGBColor(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b)
		{
		}
		constexpr RGBColor(uint32_t color) : r((uint8_t)(color >> 16)), g((uint8_t)(color >> 8 & 0xFF)), b((uint8_t)(color & 0xFF))
		{
		}

		[[nodiscard]] inline constexpr uint32_t GetHexValues() const
		{
			return r << 16 | g << 8 | b;
		}

		constexpr RGBColor& SetHexValues(uint32_t color)
		{
			r = (uint8_t)(color >> 16);
			g = (uint8_t)(color >> 8 & 0xFF);
			b = (uint8_t)(color & 0xFF);

			return *this;
		}

		/**
		 * Returns a 0-255 float value representing the lightness of the color based on luminosity
		 */
		[[nodiscard]] inline constexpr float ToGreyscale()
		{
			return 0.3f * r + 0.59f * g + 0.11f * b;
		}

		/**
		 * Returns a 0-255 float value representing the brightness of the color based on luminosity
		 */
		[[nodiscard]] static inline constexpr float HexToGreyscale(uint32_t color)
		{
			return 0.3f * (color >> 16) + 0.59f * (color >> 8 & 0xFF) + 0.11f * (color & 0xFF);
		}

		[[nodiscard]] inline constexpr float GetLuminance()
		{
			return ((r + g + b) / 3.0f) / 255.0f;
		}

		constexpr RGBColor& BlendMultiply(RGBColor other)
		{
			r = (uint8_t)((r * other.r + 0xFF) >> 8);
			g = (uint8_t)((g * other.g + 0xFF) >> 8);
			b = (uint8_t)((b * other.b + 0xFF) >> 8);

			return *this;
		}

		constexpr RGBColor& BlendMultiply(float value)
		{
			r = (uint8_t)((r * (uint8_t)(255 * value) + 0xFF) >> 8);
			g = (uint8_t)((g * (uint8_t)(255 * value) + 0xFF) >> 8);
			b = (uint8_t)((b * (uint8_t)(255 * value) + 0xFF) >> 8);

			return *this;
		}

		[[nodiscard]] constexpr RGBColor GetBlendMultiplied(RGBColor other)
		{
			return RGBColor(*this).BlendMultiply(other);
		}

		[[nodiscard]] constexpr RGBColor GetBlendMultiplied(float value)
		{
			return RGBColor(*this).BlendMultiply(value);
		}
	};
}

#endif
