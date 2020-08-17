#include "Quaternion.hpp"

#include <cmath>

namespace Engine
{
	namespace Math
	{

		constexpr Quaternion::Quaternion() : x(0), y(0), z(0), w(0)
		{
		}

		constexpr Quaternion::Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
		{
		}

		constexpr Quaternion::Quaternion(const Vector3D& axis, float w) : x(axis.x), y(axis.y), z(axis.z), w(w)
		{
		}

		Quaternion& Quaternion::Normalize()
		{
			float len = GetLength();

			x /= len;
			y /= len;
			z /= len;
			w /= len;

			return *this;
		}

		Quaternion& Quaternion::Conjugate()
		{
			x = -x;
			y = -y;
			z = -z;
			w = -w;

			return *this;
		}

		float Quaternion::GetLength() const
		{
			return sqrtf(x*x + y*y + z*z + w*w);
		}

		Quaternion Quaternion::GetNormalized() const
		{
			return Quaternion(*this).Normalize();
		}

		Quaternion Quaternion::GetConjugate() const
		{
			return Quaternion(*this).Conjugate();
		}

		constexpr Quaternion& Quaternion::operator*=(const Quaternion& other) noexcept
		{
			x = x * other.w + w * other.x + y * other.z - z * other.y;
			y = y * other.w + w * other.y + z * other.x - x * other.z;
			z = z * other.w + w * other.z + x * other.y - y * other.x;
			w = w * other.w - x * other.x - y * other.y - z * other.z;

			return *this;
		}

		constexpr Quaternion& Quaternion::operator*=(const Vector3D& vec) noexcept
		{
			x = w * vec.x + y * vec.z - z * vec.y;
			y = w * vec.y + z * vec.x - x * vec.z;
			z = w * vec.z + x * vec.y - y * vec.x;
			w = -x * vec.x - y * vec.y - z * vec.z;

			return *this;
		}

		[[nodiscard]] constexpr Quaternion Quaternion::operator*(const Quaternion& other) const noexcept
		{
			return Quaternion(*this) *= other;
		}

		[[nodiscard]] constexpr Quaternion Quaternion::operator*(const Vector3D& vec) const noexcept
		{
			return Quaternion(*this) *= vec;
		}

		[[nodiscard]] constexpr bool Quaternion::operator==(const Quaternion& other) const noexcept
		{
			return (x == other.x && y == other.y && z == other.z && w == other.w);
		}

	}
}
