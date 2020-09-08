#include "Camera.hpp"

#include "MathUtil.hpp"

#include <cmath>

using namespace Engine::Math;

namespace Engine
{
	namespace Rendering
	{
		Camera::Camera() : width(100), height(100), znear(0.1f), zfar(1000.0f), fov(90.0f), position(Vector3()), rotation(Angle()), transform(Transform())
		{
		}

		Camera::Camera(uint16_t width, uint16_t height, float znear, float zfar, float fov) :
			width(100),
			height(100),
			znear(0.1f),
			zfar(1000.0),
			fov(90.0f),
			position(Vector3()),
			rotation(Angle()),
			transform(Transform())
		{
			// aspect ratio
			float ar = (float)width / (float)height;
			float fov_rad = Util::ToRadians(fov);
			float tanhf = std::tan(fov_rad / 2.0f);
			float zrange = znear - zfar;

			float mat[4][4];

			mat[0][0] = 1.0f / (tanhf * ar); mat[0][1] = 0;            mat[0][2] = 0;                        mat[0][3] = 0;
			mat[1][0] = 0;                   mat[1][1] = 1.0f / tanhf; mat[1][2] = 0;                        mat[1][3] = 0;
			mat[2][0] = 0;                   mat[2][1] = 0;            mat[2][2] = (-znear - zfar) / zrange; mat[2][3] = (2.0f * zfar * znear) / zrange;
			mat[3][0] = 0;                   mat[3][1] = 0;            mat[3][2] = 1;                        mat[3][3] = 0;

			projection_mat = Matrix4(mat);
		}

		Matrix4 Camera::GetViewMatrix() const
		{
			return transform.GetTransformationMatrix();
		}

		uint16_t Camera::GetWith() const
		{
			return uint16_t();
		}

		uint16_t Camera::GetHeight() const
		{
			return uint16_t();
		}

		float Camera::GetZNear() const
		{
			return 0.0f;
		}

		float Camera::GetZFar() const
		{
			return 0.0f;
		}

		float Camera::GetFOV() const
		{
			return 0.0f;
		}

		void Camera::SetPosition(const Vector3& position)
		{
			// we store the position for world coords reference
			this->position = position;

			// the translation is the operation to get the positions in camera space (subtraction of the camera world coords)
			Vector3 translation = Vector3(-position.x, -position.y, -position.z);
			
			transform.SetTranslation(translation);
		}

		Vector3 Camera::GetPosition() const
		{
			return position;
		}

		void Camera::SetRotation(const Angle& rotation)
		{
			// we store the rotation angle which is the camera rotation in the world
			this->rotation = rotation;

			// convert the angle rotation to quat representation
			// the rotation is the operation to get the rotation in camera space (rotate by the conjugate)
			Quaternion rotation_quat = Quaternion(rotation).Conjugate();

			transform.SetRotation(rotation_quat);
		}

		Angle Camera::GetRotation() const
		{
			return rotation;
		}

		void Camera::Move(const Vector3& direction, float amount)
		{
			SetPosition(direction * amount);
		}

	}
}
