#include "Camera.hpp"

#include "../../Math/Util/MathUtil.hpp"

#include <cmath>

namespace Engine
{
	namespace Rendering
	{
		using namespace Math;

		Camera::Camera() :
			width(100),
			height(100),
			znear(0.1f),
			zfar(100.0f),
			fov(90.0f),
			position(Vector3()),
			rotation(Angle()),
			transform(Transform()),
			depthbuffer(DepthBuffer(100, 100))
		{
		}

		Camera::Camera(uint16_t width, uint16_t height, float znear, float zfar, float fov) :
			width(width),
			height(height),
			znear(znear),
			zfar(zfar),
			fov(fov),
			position(Vector3()),
			rotation(Quaternion()),
			transform(Transform()),
			depthbuffer(DepthBuffer(width, height))
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

		const Matrix4& Camera::GetProjectionMatrix() const
		{
			return projection_mat;
		}

		Matrix4 Camera::GetViewMatrix() const
		{
			return transform.GetRotationMatrix() * transform.GetTranslationMatrix();
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

		Vector3 Camera::GetLookDirection() const
		{
			return rotation.GetForwardVector();
		}

		void Camera::SetRotation(const Angle& rotation_ang)
		{
			// we store the rotation quaternion which is the camera rotation in the world
			// convert the angle rotation to quat representation
			// the rotation is the operation to get the rotation in camera space (rotate by the conjugate)
			rotation = Quaternion(rotation_ang);

			transform.SetRotation(rotation.GetConjugate());
		}

		Quaternion Camera::GetRotation() const
		{
			return rotation;
		}

		void Camera::Move(const Vector3& direction, float amount)
		{
			SetPosition(position + (direction * amount));
		}

		void Camera::MoveX(float amount)
		{
			Move(rotation.GetRightVector(), amount);
		}

		void Camera::MoveY(float amount)
		{
			Move(rotation.GetUpVector(), amount);
		}

		void Camera::MoveZ(float amount)
		{
			Move(rotation.GetForwardVector(), amount);
		}

		void Camera::RotatePitch(float amount)
		{
			// to rotate the pitch we rotate over the camera's right axis
			rotation = Quaternion(rotation.GetRightVector(), Util::ToRadians(amount)) * rotation;
			transform.SetRotation(rotation.GetConjugate());
		}

		void Camera::RotateYaw(float amount)
		{
			// to rotate the yaw we rotate over the camera's up axis
			rotation = Quaternion(Vector3(0, 1, 0), Util::ToRadians(amount)) * rotation;
			transform.SetRotation(rotation.GetConjugate());
		}

		void Camera::RotateRoll(float amount)
		{
			// to rotate the roll we rotate over the camera's front axis
			rotation = Quaternion(rotation.GetForwardVector(), Util::ToRadians(amount)) * rotation;
			transform.SetRotation(rotation.GetConjugate());
		}

		[[nodiscard]] DepthBuffer& Camera::GetDepthBuffer()
		{
			return depthbuffer;
		}
	}
}