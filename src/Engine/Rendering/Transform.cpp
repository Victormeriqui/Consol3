#include "Transform.hpp"

#include "../../Math/Util/MathUtil.hpp"
#include "../../Math/Matrix4.hpp"
#include "../../Math/Vector3.hpp"
#include "../../Math/Angle.hpp"

namespace Engine
{
	namespace Rendering
	{
		using namespace Math;

		Transform::Transform()
		{
			translation_mat = Matrix4();
			rotation_mat = Matrix4();
			scale_mat = Matrix4();

			translation_mat.SetIdentity();
			rotation_mat.SetIdentity();
			scale_mat.SetIdentity();
		}

		const Matrix4& Transform::GetTranslationMatrix() const
		{
			return translation_mat;
		}

		const Matrix4& Transform::GetRotationMatrix() const
		{
			return rotation_mat;
		}

		const Matrix4& Transform::GetScaleMatrix() const
		{
			return scale_mat;
		}

		Transform& Transform::SetTranslation(const Vector3& translation)
		{
			float mat[4][4];

			mat[0][0] = 1;	mat[0][1] = 0;	mat[0][2] = 0;	mat[0][3] = translation.x;
			mat[1][0] = 0;	mat[1][1] = 1;	mat[1][2] = 0;	mat[1][3] = translation.y;
			mat[2][0] = 0;	mat[2][1] = 0;	mat[2][2] = 1;	mat[2][3] = translation.z;
			mat[3][0] = 0;	mat[3][1] = 0;	mat[3][2] = 0;	mat[3][3] = 1;

			translation_mat = Matrix4(mat);

			return *this;
		}

		Transform& Transform::SetRotation(const Angle& rotation)
		{
			return SetRotation(Quaternion(rotation));
		}

		Transform& Transform::SetRotation(const Quaternion& rotation)
		{
			float mat[4][4];

			float xx = rotation.x * rotation.x;
			float yy = rotation.y * rotation.y;
			float zz = rotation.z * rotation.z;
			float ww = rotation.w * rotation.w;
			float x2 = rotation.x * 2.0f;
			float y2 = rotation.y * 2.0f;
			float z2 = rotation.z * 2.0f;
			float w2 = rotation.w * 2.0f;
			float xy = x2 * rotation.y;
			float xz = x2 * rotation.z;
			float yz = y2 * rotation.z;
			float wx = w2 * rotation.x;
			float wy = w2 * rotation.y;
			float wz = w2 * rotation.z;

			mat[0][0] = ww + xx - yy - zz; mat[0][1] = xy - wz;           mat[0][2] = xz + wy;           mat[0][3] = 0;
			mat[1][0] = xy + wz;           mat[1][1] = ww - xx + yy - zz; mat[1][2] = yz - wx;           mat[1][3] = 0;
			mat[2][0] = xz - wy;           mat[2][1] = yz + wx;           mat[2][2] = ww - xx - yy + zz; mat[2][3] = 0;
			mat[3][0] = 0;                 mat[3][1] = 0;                 mat[3][2] = 0;                 mat[3][3] = 1;

			rotation_mat = Matrix4(mat);

			return *this;
		}

		Transform& Transform::SetScale(const Vector3& scale)
		{
			float mat[4][4];

			mat[0][0] = scale.x; mat[0][1] = 0;       mat[0][2] = 0;       mat[0][3] = 0;
			mat[1][0] = 0;       mat[1][1] = scale.y; mat[1][2] = 0;       mat[1][3] = 0;
			mat[2][0] = 0;       mat[2][1] = 0;       mat[2][2] = scale.z; mat[2][3] = 0;
			mat[3][0] = 0;       mat[3][1] = 0;       mat[3][2] = 0;       mat[3][3] = 1;

			scale_mat = Matrix4(mat);

			return *this;
		}
	}
}