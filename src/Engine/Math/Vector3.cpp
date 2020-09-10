#include "Vector3.hpp"

#include "Quaternion.hpp"
#include "Util/MathUtil.hpp"

#include <cmath>

namespace Engine
{
	namespace Math
	{
		Vector3& Vector3::Normalize()
		{
			float len = GetLength();

			x /= len;
			y /= len;
			z /= len;

			return *this;
		}

		Vector3& Vector3::Rotate(const Vector3& axis, float amount)
		{
			Vector3 rotated = GetRotated(axis, amount);

			x = rotated.x;
			y = rotated.x;
			z = rotated.x;
			
			return *this;
		}

		Vector3 Vector3::GetRotated(const Vector3& axis, float amount) const
		{
			float amount_sin = std::sinf(-amount);
			float amount_cos = std::cosf(-amount);

			const Vector3 rx = GetCrossProduct(axis * amount_sin);
			const Vector3 ry = axis * GetDotProduct((axis * (1 - amount_cos)));
			const Vector3 rz = *this * amount_cos;

			return rx + rz + ry;
		}

		Vector3& Vector3::Rotate(const Quaternion& quat)
		{
			Quaternion rotated_quat = (quat * (*this)) *quat.GetConjugate();

			x = rotated_quat.x;
			y = rotated_quat.y;
			z = rotated_quat.z;
			
			return *this;
		}

		Vector3 Vector3::GetRotated(const Quaternion& quat) const
		{
			return Vector3(*this).Rotate(quat);
		}

		float Vector3::GetLength() const
		{
			return std::sqrtf(x*x + y*y + z*z);
		}

		Vector3 Vector3::GetNormalized() const
		{
			return Vector3(*this).Normalize();
		}

		float Vector3::GetDistanceTo(const Vector3& other) const
		{
			Vector3 sub = Vector3(other);
			sub -= *this;

			return sub.GetLength();
		}

		float Vector3::GetDotProduct(const Vector3& other) const
		{
			return x * other.x + y * other.y + z * other.z;
		}

		Vector3 Vector3::GetCrossProduct(const Vector3& other) const
		{
			return Vector3(
				y * other.z - z * other.y,
				z * other.x - x * other.z,
				x * other.y - y * other.x);
		}

		Vector3 Vector3::GetDirectionalTo(const Vector3& other) const
		{
			Vector3 sub = Vector3(other);
			sub -= *this;

			return sub.Normalize();
		}

		Vector3 Vector3::GetMiddleFrom(const Vector3& other) const
		{
			Vector3 mid = Vector3(other);
			mid += *this;
			mid /= 2.0f;

			return mid;
		}
	}
}