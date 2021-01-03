#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "../../Math/Angle.hpp"
#include "../../Math/Matrix4.hpp"
#include "../../Math/Quaternion.hpp"
#include "../../Math/Vector3.hpp"

namespace Engine
{
	using namespace Math;

	namespace Rendering
	{
		class Transform
		{
		private:
			Matrix4 translation_mat;
			Matrix4 rotation_mat;
			Matrix4 scale_mat;

		public:
			Transform();

			Transform& SetTranslation(const Vector3& translation);
			Transform& SetRotation(const Angle& rotation);
			Transform& SetRotation(const Quaternion& rotation);

			Transform& SetScale(const Vector3& scale);

			[[nodiscard]] const Matrix4& GetTranslationMatrix() const;
			[[nodiscard]] const Matrix4& GetRotationMatrix() const;
			[[nodiscard]] const Matrix4& GetScaleMatrix() const;
		};
	}
}

#endif
