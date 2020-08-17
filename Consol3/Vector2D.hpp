#ifndef VECTOR2D_HPP
#define VECTOR2D_HPP

namespace Engine
{
	namespace Math
	{
		class Vector2D
		{
		public:
			float x;
			float y;

			constexpr Vector2D();
			constexpr Vector2D(float x, float y);

			Vector2D& Normalize();

			[[nodiscard]] float GetLength() const;
			[[nodiscard]] Vector2D GetNormalized() const;

			[[nodiscard]] float GetDistanceTo(const Vector2D& other) const;
			[[nodiscard]] float GetDotProduct(const Vector2D& other) const;
			[[nodiscard]] Vector2D GetDirectionalTo(const Vector2D& other) const;
			[[nodiscard]] Vector2D GetMiddleFrom(const Vector2D& other) const;

			constexpr Vector2D& operator+=(const Vector2D& other) noexcept;
			constexpr Vector2D& operator-=(const Vector2D& other) noexcept;
			constexpr Vector2D& operator/=(const Vector2D& other) noexcept;
			constexpr Vector2D& operator*=(const Vector2D& other) noexcept;

			constexpr Vector2D& operator+=(float scalar) noexcept;
			constexpr Vector2D& operator-=(float scalar) noexcept;
			constexpr Vector2D& operator/=(float scalar) noexcept;
			constexpr Vector2D& operator*=(float scalar) noexcept;

			[[nodiscard]] constexpr Vector2D operator+(const Vector2D& other) const noexcept;
			[[nodiscard]] constexpr Vector2D operator-(const Vector2D& other) const noexcept;
			[[nodiscard]] constexpr Vector2D operator/(const Vector2D& other) const noexcept;
			[[nodiscard]] constexpr Vector2D operator*(const Vector2D& other) const noexcept;

			[[nodiscard]] constexpr Vector2D operator+(float scalar) const noexcept;
			[[nodiscard]] constexpr Vector2D operator-(float scalar) const noexcept;
			[[nodiscard]] constexpr Vector2D operator/(float scalar) const noexcept;
			[[nodiscard]] constexpr Vector2D operator*(float scalar) const noexcept;

			[[nodiscard]] constexpr bool operator==(const Vector2D& other) const noexcept;
		};
	}
}

#endif