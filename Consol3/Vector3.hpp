#ifndef Vector3_HPP
#define Vector3_HPP

namespace Engine
{
	namespace Math
	{
		class Vector3
		{
		public:
			float x;
			float y;
			float z;
			float w;

			constexpr Vector3();
			constexpr Vector3(float x, float y, float z);
			constexpr Vector3(float x, float y, float z, float w);

			Vector3& Normalize();
			Vector3& PerspectiveDivide();
			Vector3& Rotate(const Vector3& axis, float amount);
			Vector3& Rotate(const Quaternion& quat);

			[[nodiscard]] float GetLength() const;
			[[nodiscard]] Vector3 GetNormalized() const;
			[[nodiscard]] Vector3 GetPerspectiveDivided() const;
			[[nodiscard]] Vector3 GetRotated(const Vector3& axis, float amount) const;
			[[nodiscard]] Vector3 GetRotated(const Quaternion& quat) const;

			[[nodiscard]] float GetDistanceTo(const Vector3& other) const;
			[[nodiscard]] float GetDotProduct(const Vector3& other) const;
			[[nodiscard]] Vector3 GetCrossProduct(const Vector3& other) const;
			[[nodiscard]] Vector3 GetDirectionalTo(const Vector3& other) const;
			[[nodiscard]] Vector3 GetMiddleFrom(const Vector3& other) const;

			constexpr Vector3& operator+=(const Vector3& other) noexcept;
			constexpr Vector3& operator-=(const Vector3& other) noexcept;
			constexpr Vector3& operator/=(const Vector3& other) noexcept;
			constexpr Vector3& operator*=(const Vector3& other) noexcept;

			constexpr Vector3& operator+=(float scalar) noexcept;
			constexpr Vector3& operator-=(float scalar) noexcept;
			constexpr Vector3& operator/=(float scalar) noexcept;
			constexpr Vector3& operator*=(float scalar) noexcept;

			[[nodiscard]] constexpr Vector3 operator+(const Vector3& other) const noexcept;
			[[nodiscard]] constexpr Vector3 operator-(const Vector3& other) const noexcept;
			[[nodiscard]] constexpr Vector3 operator/(const Vector3& other) const noexcept;
			[[nodiscard]] constexpr Vector3 operator*(const Vector3& other) const noexcept;

			[[nodiscard]] constexpr Vector3 operator+(float scalar) const noexcept;
			[[nodiscard]] constexpr Vector3 operator-(float scalar) const noexcept;
			[[nodiscard]] constexpr Vector3 operator/(float scalar) const noexcept;
			[[nodiscard]] constexpr Vector3 operator*(float scalar) const noexcept;

			[[nodiscard]] constexpr bool operator==(const Vector3& other) const noexcept;
		};
	}
}

#endif