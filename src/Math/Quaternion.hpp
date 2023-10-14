	#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include "Angle.hpp"
#include "Vector3.hpp"

#include <cmath>

namespace Math
{
	class Quaternion
	{
	public:
		float x;
		float y;
		float z;
		float w;

		constexpr Quaternion() : x(0), y(0), z(0), w(1)
		{
		}

		constexpr Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
		{
		}

		Quaternion(const Vector3& axis, float w)
		{
			float half_sinw = std::sin(static_cast<float>(w / 2.0f));
			float half_cosw = std::cos(static_cast<float>(w / 2.0f));

			x		= axis.x * half_sinw;
			y		= axis.y * half_sinw;
			z		= axis.z * half_sinw;
			this->w = half_cosw;
		}

		Quaternion(const Angle& angle);

		Quaternion& Normalize();
		Quaternion& Conjugate();

		[[nodiscard]] float GetLength() const;
		[[nodiscard]] Quaternion GetNormalized() const;
		[[nodiscard]] Quaternion GetConjugate() const;

		[[nodiscard]] Vector3 GetForwardVector() const;
		[[nodiscard]] Vector3 GetBackVector() const;
		[[nodiscard]] Vector3 GetUpVector() const;
		[[nodiscard]] Vector3 GetDownVector() const;
		[[nodiscard]] Vector3 GetRightVector() const;
		[[nodiscard]] Vector3 GetLeftVector() const;

		constexpr Quaternion& operator*=(const Quaternion& other) noexcept
		{
			float x_new = x * other.w + w * other.x + y * other.z - z * other.y;
			float y_new = y * other.w + w * other.y + z * other.x - x * other.z;
			float z_new = z * other.w + w * other.z + x * other.y - y * other.x;
			float w_new = w * other.w - x * other.x - y * other.y - z * other.z;

			x = x_new;
			y = y_new;
			z = z_new;
			w = w_new;

			return *this;
		}

		constexpr Quaternion& operator*=(const Vector3& vec) noexcept
		{
			float x_new = w * vec.x + y * vec.z - z * vec.y;
			float y_new = w * vec.y + z * vec.x - x * vec.z;
			float z_new = w * vec.z + x * vec.y - y * vec.x;
			float w_new = -x * vec.x - y * vec.y - z * vec.z;

			x = x_new;
			y = y_new;
			z = z_new;
			w = w_new;

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

#endif
