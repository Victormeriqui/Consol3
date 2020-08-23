#include "Quaternion.hpp"

#include "Vector3.hpp"
#include "Matrix4.hpp"

#include <cmath>

namespace Engine
{
	namespace Math
	{
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
			return sqrtf(x*x + y*y + z*z + w*w);
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
