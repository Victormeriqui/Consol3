#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include "Vector3.hpp"

#include <cstdint>

namespace Math
{
	class Vector2
	{
	public:
		float x;
		float y;

		constexpr Vector2() : x(0), y(0)
		{
		}

		constexpr Vector2(float x, float y) : x(x), y(y)
		{
		}

		constexpr Vector2(const Vector3& vector) : x((float)vector.x), y((float)vector.y)
		{
		}

		Vector2& Normalize();

		[[nodiscard]] float GetLength() const;
		[[nodiscard]] Vector2 GetNormalized() const;
		Vector2& Lerp(const Vector2& other, float amount);

		[[nodiscard]] float GetDistanceTo(const Vector2& other) const;
		[[nodiscard]] float GetDotProduct(const Vector2& other) const;
		[[nodiscard]] Vector2 GetDirectionalTo(const Vector2& other) const;
		[[nodiscard]] Vector2 GetMiddleFrom(const Vector2& other) const;
		[[nodiscard]] Vector2 GetLerped(const Vector2& other, float amount) const;

		constexpr Vector2& operator+=(const Vector2& other) noexcept
		{
			x += other.x;
			y += other.y;

			return *this;
		}

		constexpr Vector2& operator-=(const Vector2& other) noexcept
		{
			x -= other.x;
			y -= other.y;

			return *this;
		}

		constexpr Vector2& operator/=(const Vector2& other) noexcept
		{
			x /= other.x;
			y /= other.y;

			return *this;
		}

		constexpr Vector2& operator*=(const Vector2& other) noexcept
		{
			x *= other.x;
			y *= other.y;

			return *this;
		}

		constexpr Vector2& operator+=(float scalar) noexcept
		{
			x += scalar;
			y += scalar;

			return *this;
		}

		constexpr Vector2& operator-=(float scalar) noexcept
		{
			x -= scalar;
			y -= scalar;

			return *this;
		}

		constexpr Vector2& operator/=(float scalar) noexcept
		{
			x /= scalar;
			y /= scalar;

			return *this;
		}

		constexpr Vector2& operator*=(float scalar) noexcept
		{
			x *= scalar;
			y *= scalar;

			return *this;
		}

		[[nodiscard]] constexpr Vector2 operator+(const Vector2& other) const noexcept
		{
			return  Vector2(*this) += other;
		}

		[[nodiscard]] constexpr Vector2 operator-(const Vector2& other) const noexcept
		{
			return Vector2(*this) -= other;
		}

		[[nodiscard]] constexpr Vector2 operator/(const Vector2& other) const noexcept
		{
			return Vector2(*this) /= other;
		}

		[[nodiscard]] constexpr Vector2 operator*(const Vector2& other) const noexcept
		{
			return Vector2(*this) *= other;
		}

		[[nodiscard]] constexpr Vector2 operator+(float scalar) const noexcept
		{
			return Vector2(*this) += scalar;
		}

		[[nodiscard]] constexpr Vector2 operator-(float scalar) const noexcept
		{
			return Vector2(*this) -= scalar;
		}

		[[nodiscard]] constexpr Vector2 operator/(float scalar) const noexcept
		{
			return Vector2(*this) /= scalar;
		}

		[[nodiscard]] constexpr Vector2 operator*(float scalar) const noexcept
		{
			return Vector2(*this) *= scalar;
		}

		[[nodiscard]] constexpr bool operator==(const Vector2& other) const noexcept
		{
			return (x == other.x && y == other.y);
		}
	};
}

#endif