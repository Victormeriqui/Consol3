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

		float Vector2::GetLength() const
		{
			return std::sqrtf(x*x + y*y);
		}

		Vector2 Vector2::GetNormalized() const
		{
			return Vector2(*this).Normalize();
		}

		float Vector2::GetDistanceTo(const Vector2& other) const
		{
			Vector2 sub = Vector2(other);
			sub -= *this; 

			return sub.GetLength();
		}

		float Vector2::GetDotProduct(const Vector2& other) const
		{
			return x * other.x + y * other.y;
		}

		Vector2 Vector2::GetDirectionalTo(const Vector2& other) const
		{
			Vector2 sub = Vector2(other);
			sub -= *this;

			return sub.Normalize();
		}

		Vector2 Vector2::GetMiddleFrom(const Vector2& other) const
		{
			Vector2 mid = Vector2(other);
			mid += *this;
			mid /= 2.0f;

			return mid;
		}
	}
}