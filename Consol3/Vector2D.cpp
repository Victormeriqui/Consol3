#include "Vector2D.hpp"

#include <cmath>

namespace Engine
{
	namespace Math
	{

		constexpr Vector2D::Vector2D() : x(0), y(0)
		{
		}

		constexpr Vector2D::Vector2D(float x, float y) : x(x), y(y)
		{
		}

		Vector2D& Vector2D::Normalize()
		{
			float len = GetLength();

			x /= len;
			y /= len;

			return *this;
		}

		[[nodiscard]] float Vector2D::GetLength() const
		{
			return sqrtf(x*x + y*y);
		}

		[[nodiscard]] Vector2D Vector2D::GetNormalized() const
		{
			return Vector2D(*this).Normalize();
		}

		[[nodiscard]] float Vector2D::GetDistanceTo(const Vector2D& other) const
		{
			Vector2D sub = Vector2D(other);
			sub -= *this; 

			return sub.GetLength();
		}

		[[nodiscard]] float Vector2D::GetDotProduct(const Vector2D& other) const
		{
			return x * other.x + y * other.y;
		}

		[[nodiscard]] Vector2D Vector2D::GetDirectionalTo(const Vector2D& other) const
		{
			Vector2D sub = Vector2D(other);
			sub -= *this;

			return sub.Normalize();
		}

		[[nodiscard]] Vector2D Vector2D::GetMiddleFrom(const Vector2D& other) const
		{
			Vector2D mid = Vector2D(other);
			mid += *this;
			mid /= 2.f;

			return mid;
		}

		constexpr Vector2D& Vector2D::operator+=(const Vector2D& other) noexcept
		{
			x += other.x;
			y += other.y;

			return *this;
		}

		constexpr Vector2D& Vector2D::operator-=(const Vector2D& other) noexcept
		{
			x -= other.x;
			y -= other.y;

			return *this;
		}

		constexpr Vector2D& Vector2D::operator/=(const Vector2D& other) noexcept
		{
			x /= other.x;
			y /= other.y;

			return *this;
		}

		constexpr Vector2D& Vector2D::operator*=(const Vector2D& other) noexcept
		{
			x *= other.x;
			y *= other.y;

			return *this;
		}

		constexpr Vector2D& Vector2D::operator+=(float scalar) noexcept
		{
			x += scalar;
			y += scalar;

			return *this;
		}

		constexpr Vector2D& Vector2D::operator-=(float scalar) noexcept
		{
			x -= scalar;
			y -= scalar;

			return *this;
		}

		constexpr Vector2D& Vector2D::operator/=(float scalar) noexcept
		{
			x /= scalar;
			y /= scalar;

			return *this;
		}

		constexpr Vector2D& Vector2D::operator*=(float scalar) noexcept
		{
			x *= scalar;
			y *= scalar;

			return *this;
		}

		[[nodiscard]] constexpr Vector2D Vector2D::operator+(const Vector2D& other) const noexcept
		{
			return  Vector2D(*this) += other;
		}

		[[nodiscard]] constexpr Vector2D Vector2D::operator-(const Vector2D& other) const noexcept
		{
			return Vector2D(*this) -= other;
		}

		[[nodiscard]] constexpr Vector2D Vector2D::operator/(const Vector2D& other) const noexcept
		{
			return Vector2D(*this) /= other;
		}

		[[nodiscard]] constexpr Vector2D Vector2D::operator*(const Vector2D& other) const noexcept
		{
			return Vector2D(*this) *= other;
		}

		[[nodiscard]] constexpr Vector2D Vector2D::operator+(float scalar) const noexcept
		{
			return Vector2D(*this) += scalar;
		}

		[[nodiscard]] constexpr Vector2D Vector2D::operator-(float scalar) const noexcept
		{
			return Vector2D(*this) -= scalar;
		}

		[[nodiscard]] constexpr Vector2D Vector2D::operator/(float scalar) const noexcept
		{
			return Vector2D(*this) /= scalar;
		}

		[[nodiscard]] constexpr Vector2D Vector2D::operator*(float scalar) const noexcept
		{
			return Vector2D(*this) *= scalar;
		}

		[[nodiscard]] constexpr bool Vector2D::operator==(const Vector2D& other) const noexcept
		{
			return (x == other.x && y == other.y);
		}
	}
}