#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <cstdint>

#include "Vector3.hpp"
#include "Angle.hpp"
#include "Quaternion.hpp"
#include "Matrix4.hpp"
#include "Transform.hpp"

using namespace Engine::Math;

namespace Engine
{
	namespace Rendering
	{
		class Camera
		{
		private:
			uint16_t width;
			uint16_t height;
			float znear;
			float zfar;
			float fov;

			Vector3 position;
			Angle rotation;

			Transform transform;

			Matrix4 projection_mat;

		public:
			Camera();
			Camera(uint16_t width, uint16_t height, float znear, float zfar, float fov);

			[[nodiscard]] Matrix4 GetViewMatrix() const;

			[[nodiscard]] uint16_t GetWith() const;
			[[nodiscard]] uint16_t GetHeight() const;
			[[nodiscard]] float GetZNear() const;
			[[nodiscard]] float GetZFar() const;
			[[nodiscard]] float GetFOV() const;

			void SetPosition(const Vector3& position);
			[[nodiscard]] Vector3 GetPosition() const;
			
			void SetRotation(const Angle& rotation);

			[[nodiscard]] Angle GetRotation() const;

			void Move(const Vector3& direction, float amount);
		};


	}
}

#endif