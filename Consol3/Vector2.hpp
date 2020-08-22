#ifndef Vector2_HPP
#define Vector2_HPP

namespace Engine
{
	namespace Math
	{
		class Vector2
		{
		public:
			float x;
			float y;

			constexpr Vector2();
			constexpr Vector2(float x, float y);

			Vector2& Normalize();

			[[nodiscard]] float GetLength() const;
			[[nodiscard]] Vector2 GetNormalized() const;

			[[nodiscard]] float GetDistanceTo(const Vector2& other) const;
			[[nodiscard]] float GetDotProduct(const Vector2& other) const;
			[[nodiscard]] Vector2 GetDirectionalTo(const Vector2& other) const;
			[[nodiscard]] Vector2 GetMiddleFrom(const Vector2& other) const;

			constexpr Vector2& operator+=(const Vector2& other) noexcept;
			constexpr Vector2& operator-=(const Vector2& other) noexcept;
			constexpr Vector2& operator/=(const Vector2& other) noexcept;
			constexpr Vector2& operator*=(const Vector2& other) noexcept;

			constexpr Vector2& operator+=(float scalar) noexcept;
			constexpr Vector2& operator-=(float scalar) noexcept;
			constexpr Vector2& operator/=(float scalar) noexcept;
			constexpr Vector2& operator*=(float scalar) noexcept;

			[[nodiscard]] constexpr Vector2 operator+(const Vector2& other) const noexcept;
			[[nodiscard]] constexpr Vector2 operator-(const Vector2& other) const noexcept;
			[[nodiscard]] constexpr Vector2 operator/(const Vector2& other) const noexcept;
			[[nodiscard]] constexpr Vector2 operator*(const Vector2& other) const noexcept;

			[[nodiscard]] constexpr Vector2 operator+(float scalar) const noexcept;
			[[nodiscard]] constexpr Vector2 operator-(float scalar) const noexcept;
			[[nodiscard]] constexpr Vector2 operator/(float scalar) const noexcept;
			[[nodiscard]] constexpr Vector2 operator*(float scalar) const noexcept;

			[[nodiscard]] constexpr bool operator==(const Vector2& other) const noexcept;
		};
	}
}

#endif