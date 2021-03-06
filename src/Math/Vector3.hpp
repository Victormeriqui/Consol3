#ifndef VECTOR3_HPP
#define VECTOR3_HPP

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

		[[nodiscard]] constexpr bool operator==(const Vector3& other) const noexcept
		{
			return (x == other.x && y == other.y && z == other.z);
		}
	};
}

#endif
