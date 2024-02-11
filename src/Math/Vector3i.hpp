#ifndef VECTOR3I_HPP
#define VECTOR3I_HPP

namespace Math
{
    class Vector3I
    {
    public:
        int x;
        int y;
        int z;

        constexpr Vector3I() : x(0), y(0), z(0)
        {
        }

        constexpr Vector3I(int x, int y, int z) : x(x), y(y), z(z)
        {
        }

        [[nodiscard]] constexpr Vector3I GetSignVector() const
        {
            return Vector3I((x > 0) - (x < 0), (y > 0) - (y < 0), (z > 0) - (z < 0));
        }

        constexpr Vector3I& operator+=(const Vector3I& other) noexcept
        {
            x += other.x;
            y += other.y;
            z += other.z;

            return *this;
        }

        constexpr Vector3I& operator-=(const Vector3I& other) noexcept
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;

            return *this;
        }

        constexpr Vector3I& operator/=(const Vector3I& other) noexcept
        {
            x /= other.x;
            y /= other.y;
            z /= other.z;

            return *this;
        }

        constexpr Vector3I& operator*=(const Vector3I& other) noexcept
        {
            x *= other.x;
            y *= other.y;
            z *= other.z;

            return *this;
        }

        constexpr Vector3I& operator+=(int scalar) noexcept
        {
            x += scalar;
            y += scalar;
            z += scalar;

            return *this;
        }

        constexpr Vector3I& operator-=(int scalar) noexcept
        {
            x -= scalar;
            y -= scalar;
            z -= scalar;

            return *this;
        }

        constexpr Vector3I& operator/=(int scalar) noexcept
        {
            x /= scalar;
            y /= scalar;
            z /= scalar;

            return *this;
        }

        constexpr Vector3I& operator*=(int scalar) noexcept
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;

            return *this;
        }

        [[nodiscard]] constexpr Vector3I operator+(const Vector3I& other) const noexcept
        {
            return Vector3I(*this) += other;
        }

        [[nodiscard]] constexpr Vector3I operator-(const Vector3I& other) const noexcept
        {
            return Vector3I(*this) -= other;
        }

        [[nodiscard]] constexpr Vector3I operator/(const Vector3I& other) const noexcept
        {
            return Vector3I(*this) /= other;
        }

        [[nodiscard]] constexpr Vector3I operator*(const Vector3I& other) const noexcept
        {
            return Vector3I(*this) *= other;
        }

        [[nodiscard]] constexpr Vector3I operator+(int scalar) const noexcept
        {
            return Vector3I(*this) += scalar;
        }

        [[nodiscard]] constexpr Vector3I operator-(int scalar) const noexcept
        {
            return Vector3I(*this) -= scalar;
        }

        [[nodiscard]] constexpr Vector3I operator/(int scalar) const noexcept
        {
            return Vector3I(*this) /= scalar;
        }

        [[nodiscard]] constexpr Vector3I operator*(int scalar) const noexcept
        {
            return Vector3I(*this) *= scalar;
        }

        [[nodiscard]] constexpr Vector3I operator-() const
        {
            return Vector3I(-x, -y, -z);
        }

        [[nodiscard]] constexpr bool operator==(const Vector3I& other) const noexcept
        {
            return (x == other.x && y == other.y && z == other.z);
        }
    };
}

#endif
