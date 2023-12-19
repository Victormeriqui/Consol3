#ifndef RAY_HPP
#define RAY_HPP

#include "Vector3.hpp"

#include <cstdint>

namespace Math
{
    class Ray
    {
    public:
        Vector3 origin;
        Vector3 direction;

        constexpr Ray(const Vector3& origin, const Vector3& direction) : origin(origin), direction(direction)
        {
        }

        constexpr Vector3 March(float step) const noexcept
        {
            return origin + direction * step;
        }
    };
}

#endif
