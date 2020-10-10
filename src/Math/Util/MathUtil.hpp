#ifndef MATHUTIL_HPP
#define MATHUTIL_HPP

#define PI 3.1415921f

#include <cstdint>

namespace Math
{
	namespace Util
	{

		[[nodiscard]] constexpr inline static float ToRadians(float deg) noexcept
		{
			return PI * deg / 180.f;
		}

		[[nodiscard]] constexpr inline static float ToDegrees(float rad) noexcept
		{
			return 180.f * rad / PI;
		}

		[[nodiscard]] constexpr inline static float Lerp(float val, float min, float max)
		{
			return (1 - val) * min + val * max;
		}

		[[nodiscard]] constexpr inline static uint8_t LerpToIndex(float val, uint8_t min, uint8_t max)
		{
			return (uint8_t)((1 - val) * min + val * max);
		}

	}
}

#endif