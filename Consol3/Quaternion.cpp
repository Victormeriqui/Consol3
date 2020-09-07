#include "Quaternion.hpp"

#include "Vector3.hpp"
#include "Matrix4.hpp"

#include <cmath>

namespace Engine
{
	namespace Math
	{

		Quaternion::Quaternion(const Angle& angle)
		{
			float cy = std::cosf(angle.yaw * 0.5f);
			float sy = std::sinf(angle.yaw * 0.5f);
			float cp = std::cosf(angle.pitch * 0.5f);
			float sp = std::sinf(angle.pitch * 0.5f);
			float cr = std::cosf(angle.roll * 0.5f);
			float sr = std::sinf(angle.roll * 0.5f);

			w = cr * cp * cy + sr * sp * sy;
			x = sr * cp * cy - cr * sp * sy;
			y = cr * sp * cy + sr * cp * sy;
			z = cr * cp * sy - sr * sp * cy;
		}

		Quaternion& Quaternion::Normalize()
		{
			float len = GetLength();

			x /= len;
			y /= len;
			z /= len;
			w /= len;

			return *this;
		}

		Quaternion& Quaternion::Conjugate()
		{
			x = -x;
			y = -y;
			z = -z;
			w = -w;

			return *this;
		}

		float Quaternion::GetLength() const
		{
			return std::sqrtf(x*x + y*y + z*z + w*w);
		}

		Quaternion Quaternion::GetNormalized() const
		{
			return Quaternion(*this).Normalize();
		}

		Quaternion Quaternion::GetConjugate() const
		{
			return Quaternion(*this).Conjugate();
		}
	}
}
