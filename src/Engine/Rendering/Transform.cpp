#include "Transform.hpp"

#include "../../Math/Util/MathUtil.hpp"

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
			translation_mat = Matrix4().SetTranslation(translation);

			return *this;
		}

		Transform& Transform::SetRotation(const Angle& rotation)
		{
			return SetRotation(Quaternion(rotation));
		}

		Transform& Transform::SetRotation(const Quaternion& rotation)
		{
			rotation_mat = Matrix4().SetQuaternionRotation(rotation);

			return *this;
		}

		Transform& Transform::SetScale(const Vector3& scale)
		{
			scale_mat = Matrix4().SetScale(scale);

			return *this;
		}
	}
}