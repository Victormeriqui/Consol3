#ifndef RGBCOLOR_HPP
#define RGBCOLOR_HPP

#include <algorithm>
#include <cmath>
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
		[[nodiscard]] inline constexpr float GetGreyscale() const
		{
			return 0.3f * r + 0.59f * g + 0.11f * b;
		}

		/**
		 * Returns a 0-255 float value representing the brightness of the color based on luminosity
		 */
		[[nodiscard]] static inline constexpr float GetGreyscaleFromHex(uint32_t color)
		{
			return 0.3f * (color >> 16) + 0.59f * (color >> 8 & 0xFF) + 0.11f * (color & 0xFF);
		}

		// avoids converting to HSV to get the value
		[[nodiscard]] inline constexpr float GetColorNormal() const
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

		[[nodiscard]] RGBColor GetBlendMultiplied(RGBColor other) const
		{
			return RGBColor(*this).BlendMultiply(other);
		}

		[[nodiscard]] RGBColor GetBlendMultiplied(float value) const
		{
			return RGBColor(*this).BlendMultiply(value);
		}

		[[nodiscard]] float GetColorDistance(RGBColor color) const
		{
			int16_t red	  = color.r - r;
			int16_t green = color.g - g;
			int16_t blue  = color.b - b;

			red	  = std::abs(red);
			green = std::abs(green);
			blue  = std::abs(blue);

			return std::sqrtf((float)red * (float)red + (float)green * (float)green + (float)blue * (float)blue);
		}

		constexpr RGBColor& operator+=(RGBColor other) noexcept
		{
			uint32_t red   = r + other.r;
			uint32_t green = g + other.g;
			uint32_t blue  = b + other.b;

			r = (uint8_t)std::min((uint32_t)255, red);
			g = (uint8_t)std::min((uint32_t)255, green);
			b = (uint8_t)std::min((uint32_t)255, blue);

			return *this;
		}

		[[nodiscard]] constexpr RGBColor operator+(RGBColor other) const noexcept
		{
			return RGBColor(*this) += other;
		}
	};
}

#endif
