#ifndef VECTOR3D_HPP
#define VECTOR3D_HPP

namespace Engine
{
	namespace Math
	{
		class Vector3D
		{
		public:
			float x;
			float y;
			float z;

			constexpr Vector3D();
			constexpr Vector3D(float x, float y, float z);

			Vector3D& Normalize();

			[[nodiscard]] float GetLength() const;
			[[nodiscard]] Vector3D GetNormalized() const;

			[[nodiscard]] float GetDistanceTo(const Vector3D& other) const;
			[[nodiscard]] float GetDotProduct(const Vector3D& other) const;
			[[nodiscard]] Vector3D GetCrossProduct(const Vector3D& other) const;
			[[nodiscard]] Vector3D GetDirectionalTo(const Vector3D& other) const;
			[[nodiscard]] Vector3D GetMiddleFrom(const Vector3D& other) const;

			constexpr Vector3D& operator+=(const Vector3D& other) noexcept;
			constexpr Vector3D& operator-=(const Vector3D& other) noexcept;
			constexpr Vector3D& operator/=(const Vector3D& other) noexcept;
			constexpr Vector3D& operator*=(const Vector3D& other) noexcept;

			constexpr Vector3D& operator+=(float scalar) noexcept;
			constexpr Vector3D& operator-=(float scalar) noexcept;
			constexpr Vector3D& operator/=(float scalar) noexcept;
			constexpr Vector3D& operator*=(float scalar) noexcept;

			[[nodiscard]] constexpr Vector3D operator+(const Vector3D& other) const noexcept;
			[[nodiscard]] constexpr Vector3D operator-(const Vector3D& other) const noexcept;
			[[nodiscard]] constexpr Vector3D operator/(const Vector3D& other) const noexcept;
			[[nodiscard]] constexpr Vector3D operator*(const Vector3D& other) const noexcept;

			[[nodiscard]] constexpr Vector3D operator+(float scalar) const noexcept;
			[[nodiscard]] constexpr Vector3D operator-(float scalar) const noexcept;
			[[nodiscard]] constexpr Vector3D operator/(float scalar) const noexcept;
			[[nodiscard]] constexpr Vector3D operator*(float scalar) const noexcept;

			[[nodiscard]] constexpr bool operator==(const Vector3D& other) const noexcept;
		};
	}
}

#endif