#include "Vector2.hpp"

#include <cmath>

namespace Engine
{
	namespace Math
	{

		Vector2& Vector2::Normalize()
		{
			float len = GetLength();

			x /= len;
			y /= len;

			return *this;
		}

		[[nodiscard]] float Vector2::GetLength() const
		{
			return sqrtf(x*x + y*y);
		}

		[[nodiscard]] Vector2 Vector2::GetNormalized() const
		{
			return Vector2(*this).Normalize();
		}

		[[nodiscard]] float Vector2::GetDistanceTo(const Vector2& other) const
		{
			Vector2 sub = Vector2(other);
			sub -= *this; 

			return sub.GetLength();
		}

		[[nodiscard]] float Vector2::GetDotProduct(const Vector2& other) const
		{
			return x * other.x + y * other.y;
		}

		[[nodiscard]] Vector2 Vector2::GetDirectionalTo(const Vector2& other) const
		{
			Vector2 sub = Vector2(other);
			sub -= *this;

			return sub.Normalize();
		}

		[[nodiscard]] Vector2 Vector2::GetMiddleFrom(const Vector2& other) const
		{
			Vector2 mid = Vector2(other);
			mid += *this;
			mid /= 2.f;

			return mid;
		}
	}
}