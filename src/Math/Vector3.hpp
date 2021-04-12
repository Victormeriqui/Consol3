#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include "Matrix4.hpp"

namespace Math
{
	class Quaternion;

	class Vector3
	{
	public:
		float x;
		float y;
		float z;

		constexpr Vector3() : x(0.0f), y(0.0f), z(0.0f)
		{
		}

		constexpr Vector3(float x, float y, float z) : x(x), y(y), z(z)
		{
		}

		Vector3& Normalize();
		Vector3& Rotate(const Vector3& axis, float amount);
		Vector3& Rotate(const Quaternion& quat);
		Vector3& Lerp(const Vector3& other, float amount);
		Vector3& Round();

		[[nodiscard]] float GetLength() const;
		[[nodiscard]] Vector3 GetNormalized() const;
		[[nodiscard]] Vector3 GetRotated(const Vector3& axis, float amount) const;
		[[nodiscard]] Vector3 GetRotated(const Quaternion& quat) const;

		[[nodiscard]] float GetDistanceTo(const Vector3& other) const;
		[[nodiscard]] float GetDotProduct(const Vector3& other) const;
		[[nodiscard]] Vector3 GetCrossProduct(const Vector3& other) const;
		[[nodiscard]] Vector3 GetDirectionalTo(const Vector3& other) const;
		[[nodiscard]] Vector3 GetMiddleFrom(const Vector3& other) const;
		[[nodiscard]] Vector3 GetLerped(const Vector3& other, float amount) const;
		[[nodiscard]] Vector3 GetRounded() const;

		constexpr Vector3& operator+=(const Vector3& other) noexcept
		{
			x += other.x;
			y += other.y;
			z += other.z;

			return *this;
		}

		constexpr Vector3& operator-=(const Vector3& other) noexcept
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;

			return *this;
		}

		constexpr Vector3& operator/=(const Vector3& other) noexcept
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;

			return *this;
		}

		constexpr Vector3& operator*=(const Vector3& other) noexcept
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;

			return *this;
		}

		constexpr Vector3& operator+=(float scalar) noexcept
		{
			x += scalar;
			y += scalar;
			z += scalar;

			return *this;
		}

		constexpr Vector3& operator-=(float scalar) noexcept
		{
			x -= scalar;
			y -= scalar;
			z -= scalar;

			return *this;
		}

		constexpr Vector3& operator/=(float scalar) noexcept
		{
			x /= scalar;
			y /= scalar;
			z /= scalar;

			return *this;
		}

		constexpr Vector3& operator*=(float scalar) noexcept
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;

			return *this;
		}

		[[nodiscard]] constexpr Vector3 operator+(const Vector3& other) const noexcept
		{
			return Vector3(*this) += other;
		}

		[[nodiscard]] constexpr Vector3 operator-(const Vector3& other) const noexcept
		{
			return Vector3(*this) -= other;
		}

		[[nodiscard]] constexpr Vector3 operator/(const Vector3& other) const noexcept
		{
			return Vector3(*this) /= other;
		}

		[[nodiscard]] constexpr Vector3 operator*(const Vector3& other) const noexcept
		{
			return Vector3(*this) *= other;
		}

		[[nodiscard]] constexpr Vector3 operator+(float scalar) const noexcept
		{
			return Vector3(*this) += scalar;
		}

		[[nodiscard]] constexpr Vector3 operator-(float scalar) const noexcept
		{
			return Vector3(*this) -= scalar;
		}

		[[nodiscard]] constexpr Vector3 operator/(float scalar) const noexcept
		{
			return Vector3(*this) /= scalar;
		}

		[[nodiscard]] constexpr Vector3 operator*(float scalar) const noexcept
		{
			return Vector3(*this) *= scalar;
		}

		[[nodiscard]] constexpr Vector3 operator-() const
		{
			return Vector3(-x, -y, -z);
		}

		constexpr Vector3 operator*=(const Matrix4& mat) noexcept
		{
			float x_new = mat.values[0][0] * x + mat.values[0][1] * y + mat.values[0][2] * z + mat.values[0][3] * 1;
			float y_new = mat.values[1][0] * x + mat.values[1][1] * y + mat.values[1][2] * z + mat.values[1][3] * 1;
			float z_new = mat.values[2][0] * x + mat.values[2][1] * y + mat.values[2][2] * z + mat.values[2][3] * 1;

			x = x_new;
			y = y_new;
			z = z_new;

			return *this;
		}

		[[nodiscard]] constexpr Vector3 operator*(const Matrix4& mat) const noexcept
		{
			return Vector3(*this) *= mat;
		}

		[[nodiscard]] constexpr bool operator==(const Vector3& other) const noexcept
		{
			return (x == other.x && y == other.y && z == other.z);
		}
	};
}

#endif
