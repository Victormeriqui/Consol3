#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <cstdint>

#include "../../Math/Vector3.hpp"
#include "../../Math/Angle.hpp"
#include "../../Math/Quaternion.hpp"
#include "../../Math/Matrix4.hpp"
#include "Transform.hpp"
#include "DepthBuffer.hpp"

namespace Engine
{
	namespace Rendering
	{
		using namespace Math;

		class Camera
		{
		private:
			uint16_t width;
			uint16_t height;
			float znear;
			float zfar;
			float fov;

			Vector3 position;
			Quaternion rotation;

			Transform transform;

			Matrix4 projection_mat;

			DepthBuffer depthbuffer;

		public:
			Camera();
			Camera(uint16_t width, uint16_t height, float znear, float zfar, float fov);

			[[nodiscard]] const Matrix4& GetProjectionMatrix() const;
			[[nodiscard]] Matrix4 GetViewMatrix() const;

			[[nodiscard]] uint16_t GetWith() const;
			[[nodiscard]] uint16_t GetHeight() const;
			[[nodiscard]] float GetZNear() const;
			[[nodiscard]] float GetZFar() const;
			[[nodiscard]] float GetFOV() const;

			void SetPosition(const Vector3& position);
			[[nodiscard]] Vector3 GetPosition() const;

			[[nodiscard]] Vector3 GetLookDirection() const;

			void SetRotation(const Angle& rotation_ang);
			[[nodiscard]] Quaternion GetRotation() const;

			void Move(const Vector3& direction, float amount);
			void MoveX(float amount);
			void MoveY(float amount);
			void MoveZ(float amount);

			void RotatePitch(float amount);
			void RotateYaw(float amount);
			void RotateRoll(float amount);

			[[nodiscard]] DepthBuffer& GetDepthBuffer();
			void ClearDepthBuffer();
		};
	}
}

#endif