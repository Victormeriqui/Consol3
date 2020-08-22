#include "Vector3.hpp"

#include "Quaternion.hpp"
#include "MathUtil.hpp"

#include <cmath>

namespace Engine
{
	namespace Math
	{

		constexpr Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f), w(1.0f)
		{
		}

		constexpr Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z), w(1.0f)
		{
		}

		constexpr Vector3::Vector3(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
		{
		}

		Vector3& Vector3::Normalize()
		{
			float len = GetLength();

			x /= len;
			y /= len;
			z /= len;

			return *this;
		}

		Vector3& Vector3::PerspectiveDivide()
		{
			x /= w;
			y /= w;
			z /= w;

			return *this;
		}

		Vector3& Vector3::Rotate(const Vector3& axis, float amount)
		{
			Vector3 rotated = GetRotated(axis, amount);

			x = rotated.x;
			y = rotated.x;
			z = rotated.x;
			
			return *this;
		}

		[[nodiscard]] Vector3 Vector3::GetRotated(const Vector3& axis, float amount) const
		{
			float amount_sin = sinf(-amount);
			float amount_cos = cosf(-amount);

			const Vector3 rx = GetCrossProduct(axis * amount_sin);
			const Vector3 ry = axis * GetDotProduct((axis * (1 - amount_cos)));
			const Vector3 rz = *this * amount_cos;

			return rx + rz + ry;
		}

		Vector3& Vector3::Rotate(const Quaternion& quat)
		{
			Quaternion rotated_quat = quat * (*this) * quat.GetConjugate();

			x = rotated_quat.x;
			y = rotated_quat.y;
			z = rotated_quat.z;
			
			return *this;
		}

		[[nodiscard]] Vector3 Vector3::GetRotated(const Quaternion& quat) const
		{
			return Vector3(*this).Rotate(quat);
		}

		[[nodiscard]] Vector3 Vector3::GetPerspectiveDivided() const
		{
			return Vector3(*this).PerspectiveDivide();
		}

		[[nodiscard]] float Vector3::GetLength() const
		{
			return sqrtf(x*x + y*y + z*z);
		}

		[[nodiscard]] Vector3 Vector3::GetNormalized() const
		{
			return Vector3(*this).Normalize();
		}

		[[nodiscard]] float Vector3::GetDistanceTo(const Vector3& other) const
		{
			Vector3 sub = Vector3(other);
			sub -= *this;

			return sub.GetLength();
		}

		[[nodiscard]] float Vector3::GetDotProduct(const Vector3& other) const
		{
			return x * other.x + y * other.y + z * other.z;
		}

		[[nodiscard]] Vector3 Vector3::GetCrossProduct(const Vector3& other) const
		{
			return Vector3(
				y * other.z - z * other.y,
				z * other.x - x * other.z,
				x * other.y - y * other.x);
		}

		[[nodiscard]] Vector3 Vector3::GetDirectionalTo(const Vector3& other) const
		{
			Vector3 sub = Vector3(other);
			sub -= *this;

			return sub.Normalize();
		}

		[[nodiscard]] Vector3 Vector3::GetMiddleFrom(const Vector3& other) const
		{
			Vector3 mid = Vector3(other);
			mid += *this;
			mid /= 2.f;

			return mid;
		}


		constexpr Vector3& Vector3::operator+=(const Vector3& other) noexcept
		{
			x += other.x;
			y += other.y;
			z += other.z;

			return *this;
		}

		constexpr Vector3& Vector3::operator-=(const Vector3& other) noexcept
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;

			return *this;
		}

		constexpr Vector3& Vector3::operator/=(const Vector3& other) noexcept
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;

			return *this;
		}

		constexpr Vector3& Vector3::operator*=(const Vector3& other) noexcept
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;

			return *this;
		}

		constexpr Vector3& Vector3::operator+=(float scalar) noexcept
		{
			x += scalar;
			y += scalar;
			z += scalar;

			return *this;
		}

		constexpr Vector3& Vector3::operator-=(float scalar) noexcept
		{
			x -= scalar;
			y -= scalar;
			z -= scalar;

			return *this;
		}

		constexpr Vector3& Vector3::operator/=(float scalar) noexcept
		{
			x /= scalar;
			y /= scalar;
			z /= scalar;

			return *this;
		}

		constexpr Vector3& Vector3::operator*=(float scalar) noexcept
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;

			return *this;
		}

		[[nodiscard]] constexpr Vector3 Vector3::operator+(const Vector3& other) const noexcept
		{
			return Vector3(*this) += other;
		}

		[[nodiscard]] constexpr Vector3 Vector3::operator-(const Vector3& other) const noexcept
		{
			return Vector3(*this) -= other;
		}

		[[nodiscard]] constexpr Vector3 Vector3::operator/(const Vector3& other) const noexcept
		{
			return Vector3(*this) /= other;
		}

		[[nodiscard]] constexpr Vector3 Vector3::operator*(const Vector3& other) const noexcept
		{
			return Vector3(*this) *= other;
		}

		[[nodiscard]] constexpr Vector3 Vector3::operator+(float scalar) const noexcept
		{
			return Vector3(*this) += scalar;
		}

		[[nodiscard]] constexpr Vector3 Vector3::operator-(float scalar) const noexcept
		{
			return Vector3(*this) -= scalar;
		}

		[[nodiscard]] constexpr Vector3 Vector3::operator/(float scalar) const noexcept
		{
			return Vector3(*this) /= scalar;
		}

		[[nodiscard]] constexpr Vector3 Vector3::operator*(float scalar) const noexcept
		{
			return Vector3(*this) *= scalar;
		}

		[[nodiscard]] constexpr bool Vector3::operator==(const Vector3& other) const noexcept
		{
			return (x == other.x && y == other.y && z == other.z);
		}

	}
}