#ifndef MATHUTIL_HPP
#define MATHUTIL_HPP

#define PI 3.1415921f

#include "Math/Vector3.hpp"

#include <algorithm>
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

        [[nodiscard]] constexpr inline static float Lerp(float val, float from_min, float from_max, float to_min, float to_max)
        {
            float clamped_val = from_min < from_max ? std::clamp(val, from_min, from_max) : std::clamp(val, from_max, from_min);
            float from_dist   = from_max - from_min;
            float to_dist     = to_max - to_min;

            float scaled = (clamped_val - from_min) / from_dist;

            return to_min + (scaled * to_dist);
        }

        template<typename T>
        [[nodiscard]] constexpr inline static T LerpCast(float val, T min, T max)
        {
            return static_cast<T>((1 - val) * min + val * max);
        }

        template<typename T>
        [[nodiscard]] constexpr inline static bool IsInRange(T val, T min, T max)
        {
            return val >= min && val <= max;
        }

    }
}

#endif
