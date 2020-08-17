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

			Vector3D() : x(0), y(0), z(0) {}
			Vector3D(float x, float y, float z) : x(x), y(y), z(z) {};
			~Vector3D() {};

			Vector3D& Normalize();

			float GetLenght();
			Vector3D& GetNormalized();

			float GetDistanceTo(const Vector3D& other);
			float GetDotProduct(const Vector3D& other);
			float GetCrossProduct(const Vector3D& other);
			Vector3D& GetDirectionalTo(const Vector3D& other);
			Vector3D& GetMiddleFrom(const Vector3D& other);

			Vector3D& operator+= (const Vector3D& other)
			{
				x += other.x;
				y += other.y;
				z += other.z;

				return *this;
			}

			Vector3D& operator-= (const Vector3D& other)
			{
				x -= other.x;
				y -= other.y;
				z -= other.z;

				return *this;
			}

			Vector3D& operator/= (const Vector3D& other)
			{
				x /= other.x;
				y /= other.y;
				z /= other.z;

				return *this;
			}

			Vector3D& operator*= (const Vector3D& other)
			{
				x *= other.x;
				y *= other.y;
				z *= other.z;

				return *this;
			}

			Vector3D& operator+ (const Vector3D& other) const
			{
				return Vector3D(*this) += other;
			}

			Vector3D& operator- (const Vector3D& other) const
			{
				return Vector3D(*this) -= other;
			}

			Vector3D& operator/ (const Vector3D& other) const
			{
				return Vector3D(*this) /= other;
			}

			Vector3D& operator* (const Vector3D& other) const
			{
				return Vector3D(*this) *= other;
			}

			bool operator==(const Vector3D& other) const
			{
				return (x == other.x && y == other.y && z == other.z);
			}
		};
	}
}

#endif