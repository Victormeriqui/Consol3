#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include "Vector3.hpp"
#include "Angle.hpp"

namespace Engine
{
	namespace Math
	{
		class Quaternion
		{
		public:
			float x;
			float y;
			float z;
			float w;

			constexpr Quaternion() : x(0), y(0), z(0), w(0)
			{
			}

			constexpr Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
			{
			}

			constexpr Quaternion(const Vector3& axis, float w) : x(axis.x), y(axis.y), z(axis.z), w(w)
			{
			}

			Quaternion(const Angle& angle);

			Quaternion& Normalize();
			Quaternion& Conjugate();

			[[nodiscard]] float GetLength() const;
			[[nodiscard]] Quaternion GetNormalized() const;
			[[nodiscard]] Quaternion GetConjugate() const;

			constexpr Quaternion& operator*=(const Quaternion& other) noexcept
			{
				x = x * other.w + w * other.x + y * other.z - z * other.y;
				y = y * other.w + w * other.y + z * other.x - x * other.z;
				z = z * other.w + w * other.z + x * other.y - y * other.x;
				w = w * other.w - x * other.x - y * other.y - z * other.z;

				return *this;
			}

			constexpr Quaternion& operator*=(const Vector3& vec) noexcept
			{
				x = w * vec.x + y * vec.z - z * vec.y;
				y = w * vec.y + z * vec.x - x * vec.z;
				z = w * vec.z + x * vec.y - y * vec.x;
				w = -x * vec.x - y * vec.y - z * vec.z;

				return *this;
			}

			[[nodiscard]] constexpr Quaternion operator*(const Quaternion& other) const noexcept
			{
				return Quaternion(*this) *= other;
			}

			[[nodiscard]] constexpr Quaternion operator*(const Vector3& vec) const noexcept
			{
				return Quaternion(*this) *= vec;
			}

			[[nodiscard]] constexpr bool operator==(const Quaternion& other) const noexcept
			{
				return (x == other.x && y == other.y && z == other.z && w == other.w);
			}
		};
	}
}

#endif