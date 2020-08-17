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

			Vector2D() : x(0), y(0) {};
			Vector2D(float x, float y) : x(x), y(y) {};
			~Vector2D() {};

			Vector2D& Normalize();

			float GetLenght();
			Vector2D& GetNormalized();

			float GetDistanceTo(const Vector2D& other);
			float GetDotProduct(const Vector2D& other);
			Vector2D& GetDirectionalTo(const Vector2D& other);
			Vector2D& GetMiddleFrom(const Vector2D& other);

			Vector2D& operator+= (const Vector2D& other)
			{
				x += other.x;
				y += other.y;

				return *this;
			}

			Vector2D& operator-= (const Vector2D& other)
			{
				x -= other.x;
				y -= other.y;

				return *this;
			}

			Vector2D& operator/= (const Vector2D& other)
			{
				x /= other.x;
				y /= other.y;

				return *this;
			}

			Vector2D& operator*= (const Vector2D& other)
			{
				x *= other.x;
				y *= other.y;

				return *this;
			}

			Vector2D& operator+ (const Vector2D& other) const
			{
				return Vector2D(*this) += other;
			}

			Vector2D& operator- (const Vector2D& other) const
			{
				return Vector2D(*this) -= other;
			}

			Vector2D& operator/ (const Vector2D& other) const
			{
				return Vector2D(*this) /= other;
			}

			Vector2D& operator* (const Vector2D& other) const
			{
				return Vector2D(*this) *= other;
			}

			bool operator==(const Vector2D& other) const
			{
				return (x == other.x && y == other.y);
			}
		};
	}
}

#endif