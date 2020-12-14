#ifndef POINT_HPP
#define POINT_HPP

#include "Vector2.hpp"
#include "Vector3.hpp"

#include <cstdint>

namespace Math
{
	class Point2
	{
	public:
		int32_t x;
		int32_t y;

		constexpr Point2(int32_t x, int32_t y) : x(x), y(y) {}
		constexpr Point2(const Vector2& vector) : x((int32_t)vector.x), y((int32_t)vector.y) {}
		constexpr Point2(const Vector3& vector) : x((int32_t)vector.x), y((int32_t)vector.y) {}

		constexpr Point2& operator+=(const Point2& other) noexcept
		{
			x += other.x;
			y += other.y;

			return *this;
		}

		constexpr Point2& operator-=(const Point2& other) noexcept
		{
			x -= other.x;
			y -= other.y;

			return *this;
		}

		constexpr Point2& operator/=(const Point2& other) noexcept
		{
			x /= other.x;
			y /= other.y;

			return *this;
		}

		constexpr Point2& operator*=(const Point2& other) noexcept
		{
			x *= other.x;
			y *= other.y;

			return *this;
		}

		constexpr Point2& operator+=(uint16_t scalar) noexcept
		{
			x += scalar;
			y += scalar;

			return *this;
		}

		constexpr Point2& operator-=(uint16_t scalar) noexcept
		{
			x -= scalar;
			y -= scalar;

			return *this;
		}

		constexpr Point2& operator/=(uint16_t scalar) noexcept
		{
			x /= scalar;
			y /= scalar;

			return *this;
		}

		constexpr Point2& operator*=(uint16_t scalar) noexcept
		{
			x *= scalar;
			y *= scalar;

			return *this;
		}

		[[nodiscard]] constexpr Point2 operator+(const Point2& other) const noexcept
		{
			return  Point2(*this) += other;
		}

		[[nodiscard]] constexpr Point2 operator-(const Point2& other) const noexcept
		{
			return Point2(*this) -= other;
		}

		[[nodiscard]] constexpr Point2 operator/(const Point2& other) const noexcept
		{
			return Point2(*this) /= other;
		}

		[[nodiscard]] constexpr Point2 operator*(const Point2& other) const noexcept
		{
			return Point2(*this) *= other;
		}

		[[nodiscard]] constexpr Point2 operator+(uint16_t scalar) const noexcept
		{
			return Point2(*this) += scalar;
		}

		[[nodiscard]] constexpr Point2 operator-(uint16_t scalar) const noexcept
		{
			return Point2(*this) -= scalar;
		}

		[[nodiscard]] constexpr Point2 operator/(uint16_t scalar) const noexcept
		{
			return Point2(*this) /= scalar;
		}

		[[nodiscard]] constexpr Point2 operator*(uint16_t scalar) const noexcept
		{
			return Point2(*this) *= scalar;
		}

		[[nodiscard]] constexpr bool operator==(const Point2& other) const noexcept
		{
			return (x == other.x && y == other.y);
		}
	};
}

#endif