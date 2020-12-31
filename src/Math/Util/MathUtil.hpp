#ifndef MATHUTIL_HPP
#define MATHUTIL_HPP

#define PI 3.1415921f

#include "../Vector3.hpp"

#include <cstdint>

namespace Math
{
	namespace Util
	{
		const static Vector3 yaxis = Vector3(0.0f, 1.0f, 0.0f);

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

		template <class T>
		[[nodiscard]] constexpr inline static T LerpCast(float val, T min, T max)
		{
			return (T)((1 - val) * min + val * max);
		}
	}
}

#endif