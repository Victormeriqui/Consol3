#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include "Vector3D.hpp"

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

			Quaternion() : x(0), y(0), z(0), w(0) {}
			Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};
			Quaternion(const Vector3D& axis, float w) : x(axis.x), y(axis.y), z(axis.z), w(w) {};
			~Quaternion() {};

			Quaternion& Normalize();
			Quaternion& Conjugate();

			float GetLenght();
			Quaternion& GetNormalized();
			Quaternion& GetConjugate();

			Quaternion& operator*= (const Quaternion& other)
			{
				x = x * other.w + w * other.x + y * other.z - z * other.y;
				y = y * other.w + w * other.y + z * other.x - x * other.z;
				z = z * other.w + w * other.z + x * other.y - y * other.x;
				w = w * other.w - x * other.x - y * other.y - z * other.z;

				return *this;
			}

			Quaternion& operator*= (const Vector3D& vec)
			{
				x = w * vec.x + y * vec.z - z * vec.y;
				y = w * vec.y + z * vec.x - x * vec.z;
				z = w * vec.z + x * vec.y - y * vec.x;
				w = -x * vec.x - y * vec.y - z * vec.z;

				return *this;
			}


			Quaternion& operator* (const Quaternion& other) const
			{
				return Quaternion(*this) *= other;
			}

			Quaternion& operator* (const Vector3D& vec) const
			{
				return Quaternion(*this) *= vec;
			}

			bool operator==(const Quaternion& other) const
			{
				return (x == other.x && y == other.y && z == other.z && w == other.w);
			}
		};
	}
}

#endif