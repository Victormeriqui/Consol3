#include "Vector3D.hpp"

#include <cmath>

namespace Engine
{
	namespace Math
	{

		constexpr Vector3D::Vector3D() : x(0), y(0), z(0)
		{
		}

		constexpr Vector3D::Vector3D(float x, float y, float z) : x(x), y(y), z(z)
		{
		}

		Vector3D& Vector3D::Normalize()
		{
			float len = GetLength();

			x /= len;
			y /= len;
			z /= len;

			return *this;
		}

		[[nodiscard]] float Vector3D::GetLength() const
		{
			return sqrtf(x*x + y*y + z*z);
		}

		[[nodiscard]] Vector3D Vector3D::GetNormalized() const
		{
			return Vector3D(*this).Normalize();
		}

		[[nodiscard]] float Vector3D::GetDistanceTo(const Vector3D& other) const
		{
			Vector3D sub = Vector3D(other);
			sub -= *this;

			return sub.GetLength();
		}

		[[nodiscard]] float Vector3D::GetDotProduct(const Vector3D& other) const
		{
			return x * other.x + y * other.y + z * other.z;
		}

		[[nodiscard]] Vector3D Vector3D::GetCrossProduct(const Vector3D& other) const
		{
			return Vector3D(
				y * other.z - z * other.y,
				z * other.x - x * other.z,
				x * other.y - y * other.x);
		}

		[[nodiscard]] Vector3D Vector3D::GetDirectionalTo(const Vector3D& other) const
		{
			Vector3D sub = Vector3D(other);
			sub -= *this;

			return sub.Normalize();
		}

		[[nodiscard]] Vector3D Vector3D::GetMiddleFrom(const Vector3D& other) const
		{
			Vector3D mid = Vector3D(other);
			mid += *this;
			mid /= 2.f;

			return mid;
		}


		constexpr Vector3D& Vector3D::operator+=(const Vector3D& other) noexcept
		{
			x += other.x;
			y += other.y;
			z += other.z;

			return *this;
		}

		constexpr Vector3D& Vector3D::operator-=(const Vector3D& other) noexcept
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;

			return *this;
		}

		constexpr Vector3D& Vector3D::operator/=(const Vector3D& other) noexcept
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;

			return *this;
		}

		constexpr Vector3D& Vector3D::operator*=(const Vector3D& other) noexcept
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;

			return *this;
		}

		constexpr Vector3D& Vector3D::operator+=(float scalar) noexcept
		{
			x += scalar;
			y += scalar;
			z += scalar;

			return *this;
		}

		constexpr Vector3D& Vector3D::operator-=(float scalar) noexcept
		{
			x -= scalar;
			y -= scalar;
			z -= scalar;

			return *this;
		}

		constexpr Vector3D& Vector3D::operator/=(float scalar) noexcept
		{
			x /= scalar;
			y /= scalar;
			z /= scalar;

			return *this;
		}

		constexpr Vector3D& Vector3D::operator*=(float scalar) noexcept
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;

			return *this;
		}

		[[nodiscard]] constexpr Vector3D Vector3D::operator+(const Vector3D& other) const noexcept
		{
			return Vector3D(*this) += other;
		}

		[[nodiscard]] constexpr Vector3D Vector3D::operator-(const Vector3D& other) const noexcept
		{
			return Vector3D(*this) -= other;
		}

		[[nodiscard]] constexpr Vector3D Vector3D::operator/(const Vector3D& other) const noexcept
		{
			return Vector3D(*this) /= other;
		}

		[[nodiscard]] constexpr Vector3D Vector3D::operator*(const Vector3D& other) const noexcept
		{
			return Vector3D(*this) *= other;
		}

		[[nodiscard]] constexpr Vector3D Vector3D::operator+(float scalar) const noexcept
		{
			return Vector3D(*this) += scalar;
		}

		[[nodiscard]] constexpr Vector3D Vector3D::operator-(float scalar) const noexcept
		{
			return Vector3D(*this) -= scalar;
		}

		[[nodiscard]] constexpr Vector3D Vector3D::operator/(float scalar) const noexcept
		{
			return Vector3D(*this) /= scalar;
		}

		[[nodiscard]] constexpr Vector3D Vector3D::operator*(float scalar) const noexcept
		{
			return Vector3D(*this) *= scalar;
		}

		[[nodiscard]] constexpr bool Vector3D::operator==(const Vector3D& other) const noexcept
		{
			return (x == other.x && y == other.y && z == other.z);
		}

	}
}