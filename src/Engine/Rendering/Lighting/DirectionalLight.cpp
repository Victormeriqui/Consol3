#include "DirectionalLight.hpp"

#include "../../../Math/Vector3.hpp"
#include "../Vertex.hpp"
#include "../Transform.hpp"

#include <algorithm>
#include <cmath>
#include <functional>

namespace Engine
{
	namespace Rendering
	{
		namespace Lighting
		{
			DirectionalLight::DirectionalLight() :
				intensity(1.0f),
				transform(Transform()),
				// TODO: figure out the best value for this
				depthbuffer(DepthBuffer(200, 200)),
				projection_mat(Matrix4().SetOrthographic(-5, 5, 5, -5, -5, 5))
			{
				// by calling the setter explicitly we also update the light matrix
				SetDirection(Vector3());
			}

			DirectionalLight::DirectionalLight(const Vector3& direction) :
				intensity(1.0f),
				transform(Transform()),
				// TODO: figure out the best value for this
				depthbuffer(DepthBuffer(200, 200)),
				projection_mat(Matrix4().SetOrthographic(-5, 5, 5, -5, -5, 5))
			{
				SetDirection(direction);
			}

			DirectionalLight::DirectionalLight(const Vector3& direction, float intensity) :
				intensity(intensity),
				transform(Transform()),
				// TODO: figure out the best value for this
				depthbuffer(DepthBuffer(200, 200)),
				projection_mat(Matrix4().SetOrthographic(-5, 5, 5, -5, -5, 5))
			{
				SetDirection(direction);
			}

			void DirectionalLight::UpdateLightMatrix()
			{
				light_mat = projection_mat * transform.GetRotationMatrix();
			}

			Vector3 DirectionalLight::GetDirection() const
			{
				return direction;
			}

			void DirectionalLight::SetDirection(const Vector3& direction)
			{
				this->direction = direction;

				// recalculate the light_matrix
				float pitch = pitch = std::atan(direction.z / std::sqrt(direction.x * direction.x + direction.y * direction.y));
				float yaw = std::atan(direction.y / direction.x);

				transform.SetRotation(Angle(pitch, yaw, 0));
				
				UpdateLightMatrix();
			}

			float DirectionalLight::GetIntensity() const
			{
				return intensity;
			}

			void DirectionalLight::SetIntensity(float intensity)
			{
				this->intensity = intensity;
			}
			
			float DirectionalLight::GetLightAmountAt(const Vertex& vertex) const
			{
				float amount = vertex.GetNormal().GetDotProduct(-direction);

				amount *= intensity;

				return std::max(0.0f, amount);
			}

			bool DirectionalLight::IsShadowCaster() const
			{
				return true;
			}

			std::optional<std::reference_wrapper<const Matrix4>> DirectionalLight::GetLightMatrix() const
			{
				return std::optional(std::reference_wrapper(light_mat));
			}

			std::optional<std::reference_wrapper<DepthBuffer>> DirectionalLight::GetLightDepthBuffer()
			{
				return std::optional(std::reference_wrapper(depthbuffer));
			}

		}
	}
}
