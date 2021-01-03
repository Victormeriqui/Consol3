#include "DirectionalLight.hpp"

#include "../../../Math/Matrix4.hpp"
#include "../../../Math/Util/MathUtil.hpp"
#include "../../../Math/Vector3.hpp"
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
			float ortho_size = 3;
			DirectionalLight::DirectionalLight() :
				intensity(1.0f),
				// TODO: figure out the best value for this
				depthbuffer(DepthBuffer(200, 200)),
				projection_mat(Matrix4().SetOrthographicProjection(-ortho_size, ortho_size, ortho_size, -ortho_size, -ortho_size, ortho_size))
			{
				// by calling the setter explicitly we also update the view matrix
				SetDirection(Vector3());
			}

			DirectionalLight::DirectionalLight(const Vector3& direction) :
				intensity(1.0f),
				// TODO: figure out the best value for this
				depthbuffer(DepthBuffer(200, 200)),
				projection_mat(Matrix4().SetOrthographicProjection(-ortho_size, ortho_size, ortho_size, -ortho_size, -ortho_size, ortho_size))
			{
				SetDirection(direction);
			}

			DirectionalLight::DirectionalLight(const Vector3& direction, float intensity) :
				intensity(intensity),
				// TODO: figure out the best value for this
				depthbuffer(DepthBuffer(200, 200)),
				projection_mat(Matrix4().SetOrthographicProjection(-ortho_size, ortho_size, ortho_size, -ortho_size, -ortho_size, ortho_size))
			{
				SetDirection(direction);
			}

			void DirectionalLight::UpdateViewMatrix()
			{
				Vector3 right = Util::yaxis.GetCrossProduct(direction);
				right.Normalize();

				Vector3 up = direction.GetCrossProduct(right);
				up.Normalize();

				Matrix4 rotation_mat = Matrix4().SetDirectionalRotation(right, up, direction);

				view_mat = rotation_mat;
			}

			Vector3 DirectionalLight::GetDirection() const
			{
				return direction;
			}

			void DirectionalLight::SetDirection(const Vector3& direction)
			{
				this->direction = direction;

				UpdateViewMatrix();
			}

			float DirectionalLight::GetIntensity() const
			{
				return intensity;
			}

			void DirectionalLight::SetIntensity(float intensity)
			{
				this->intensity = intensity;
			}

			float DirectionalLight::GetLightAmountAt(const Vector3& position, const Vector3& normal) const
			{
				float amount = normal.GetDotProduct(-direction);

				amount *= intensity;

				return std::max(0.0f, amount);
			}

			bool DirectionalLight::IsShadowCaster() const
			{
				return true;
			}

			std::optional<bool> DirectionalLight::IsLinearProjection() const
			{
				return std::optional(true);
			}

			std::optional<std::reference_wrapper<const Matrix4>> DirectionalLight::GetProjectionMatrix() const
			{
				return std::optional(std::reference_wrapper(projection_mat));
			}

			std::optional<std::reference_wrapper<const Matrix4>> DirectionalLight::GetViewMatrix() const
			{
				return std::optional(std::reference_wrapper(view_mat));
			}

			std::optional<std::reference_wrapper<DepthBuffer>> DirectionalLight::GetLightDepthBuffer()
			{
				return std::optional(std::reference_wrapper<DepthBuffer>(depthbuffer));
			}

			std::optional<float> DirectionalLight::GetBias() const
			{
				return 0.04f;
			}

			void DirectionalLight::ClearDepthBuffer()
			{
				depthbuffer.FillBuffer(std::numeric_limits<float>::max());
			}
		}
	}
}
