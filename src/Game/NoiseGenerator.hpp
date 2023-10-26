#ifndef NOISEGENERATOR_HPP
#define NOISEGENERATOR_HPP

#include "Math/Util/MathUtil.hpp"
#include "Math/Vector2.hpp"

#include <cmath>
#include <cstdint>

/*
https://en.wikipedia.org/wiki/Perlin_noise
*/

namespace Game
{
    using namespace Math;

    Vector2 GenerateRandomGradient(uint32_t x, uint32_t y)
    {
        // Random float. No precomputed gradients mean this works for any number of grid coordinates
        float random = 2920.f * std::sin(x * 21942.f + y * 171324.f + 8912.f) * std::cos(x * 23157.f * y * 217832.f + 9758.f);

        return Vector2(cos(random), sin(random));
    }

    // Computes the dot product of the distance and gradient vectors.
    float GetDotGridGradient(uint32_t grid_x, uint32_t grid_y, const Vector2& position)
    {
        Vector2 gradient = GenerateRandomGradient(grid_x, grid_y);

        float dist_x = position.x - (float)grid_x;
        float dist_y = position.y - (float)grid_y;

        return (dist_x * gradient.x) + (dist_y * gradient.y);
    }

    inline static float GetPerlinNoise(const Vector2& position)
    {
        // Determine grid cell coordinates
        uint32_t x0 = (uint32_t)position.x;
        uint32_t x1 = x0 + 1;
        uint32_t y0 = (uint32_t)position.y;
        uint32_t y1 = y0 + 1;

        float sx = position.x - (float)x0;
        float sy = position.y - (float)y0;

        // Interpolate between grid point gradients
        float n0, n1, ix0, ix1, value;

        n0  = GetDotGridGradient(x0, y0, position);
        n1  = GetDotGridGradient(x1, y0, position);
        ix0 = Util::Lerp(n0, n1, sx);

        n0  = GetDotGridGradient(x0, y1, position);
        n1  = GetDotGridGradient(x1, y1, position);
        ix1 = Util::Lerp(n0, n1, sx);

        value = Util::Lerp(ix0, ix1, sy);

        return value;
    }
}

#endif
