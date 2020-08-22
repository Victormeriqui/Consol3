#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include "Vector3.hpp"

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

			constexpr Quaternion();
			constexpr Quaternion(float x, float y, float z, float w);
			constexpr Quaternion(const Vector3& axis, float w);

			Quaternion& Normalize();
			Quaternion& Conjugate();

			[[nodiscard]] float GetLength() const;
			[[nodiscard]] Quaternion GetNormalized() const;
			[[nodiscard]] Quaternion GetConjugate() const;

			constexpr Quaternion& operator*=(const Quaternion& other) noexcept;
			constexpr Quaternion& operator*=(const Vector3& vec) noexcept;

			[[nodiscard]] constexpr Quaternion operator*(const Quaternion& other) const noexcept;
			[[nodiscard]] constexpr Quaternion operator*(const Vector3& vec) const noexcept;

			[[nodiscard]] constexpr bool operator==(const Quaternion& other) const noexcept;
		};
	}
}

#endif