#ifndef VECTOR2I_HPP
#define VECTOR2I_HPP

#include "Vector2.hpp"
#include "Vector3.hpp"

#include <cstdint>

namespace Math
{
    class Vector2I
    {
    public:
        int32_t x;
        int32_t y;

        constexpr Vector2I(int32_t x, int32_t y) : x(x), y(y)
        {
        }
        constexpr Vector2I(const Vector2& vector) : x((int32_t)vector.x), y((int32_t)vector.y)
        {
        }
        constexpr Vector2I(const Vector3& vector) : x((int32_t)vector.x), y((int32_t)vector.y)
        {
        }

        constexpr Vector2I& operator+=(const Vector2I& other) noexcept
        {
            x += other.x;
            y += other.y;

            return *this;
        }

        constexpr Vector2I& operator-=(const Vector2I& other) noexcept
        {
            x -= other.x;
            y -= other.y;

            return *this;
        }

        constexpr Vector2I& operator/=(const Vector2I& other) noexcept
        {
            x /= other.x;
            y /= other.y;

            return *this;
        }

        constexpr Vector2I& operator*=(const Vector2I& other) noexcept
        {
            x *= other.x;
            y *= other.y;

            return *this;
        }

        constexpr Vector2I& operator+=(uint16_t scalar) noexcept
        {
            x += scalar;
            y += scalar;

            return *this;
        }

        constexpr Vector2I& operator-=(uint16_t scalar) noexcept
        {
            x -= scalar;
            y -= scalar;

            return *this;
        }

        constexpr Vector2I& operator/=(uint16_t scalar) noexcept
        {
            x /= scalar;
            y /= scalar;

            return *this;
        }

        constexpr Vector2I& operator*=(uint16_t scalar) noexcept
        {
            x *= scalar;
            y *= scalar;

            return *this;
        }

        [[nodiscard]] constexpr Vector2I operator+(const Vector2I& other) const noexcept
        {
            return Vector2I(*this) += other;
        }

        [[nodiscard]] constexpr Vector2I operator-(const Vector2I& other) const noexcept
        {
            return Vector2I(*this) -= other;
        }

        [[nodiscard]] constexpr Vector2I operator/(const Vector2I& other) const noexcept
        {
            return Vector2I(*this) /= other;
        }

        [[nodiscard]] constexpr Vector2I operator*(const Vector2I& other) const noexcept
        {
            return Vector2I(*this) *= other;
        }

        [[nodiscard]] constexpr Vector2I operator+(uint16_t scalar) const noexcept
        {
            return Vector2I(*this) += scalar;
        }

        [[nodiscard]] constexpr Vector2I operator-(uint16_t scalar) const noexcept
        {
            return Vector2I(*this) -= scalar;
        }

        [[nodiscard]] constexpr Vector2I operator/(uint16_t scalar) const noexcept
        {
            return Vector2I(*this) /= scalar;
        }

        [[nodiscard]] constexpr Vector2I operator*(uint16_t scalar) const noexcept
        {
            return Vector2I(*this) *= scalar;
        }

        [[nodiscard]] constexpr bool operator==(const Vector2I& other) const noexcept
        {
            return (x == other.x && y == other.y);
        }
    };
}

#endif
