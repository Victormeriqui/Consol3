#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Matrix4.hpp"
#include "Quaternion.hpp"
#include "Angle.hpp"
#include "Vector3.hpp"

using namespace Engine::Math;

namespace Engine
{
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

			[[nodiscard]] Matrix4 GetTransformationMatrix() const;

		};
	}
}

#endif