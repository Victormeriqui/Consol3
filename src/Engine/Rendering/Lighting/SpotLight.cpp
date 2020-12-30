#include "SpotLight.hpp"

#include "../../../Math/Vector3.hpp"
#include "../Vertex.hpp"
#include "../../../Math/Util/MathUtil.hpp"
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
			SpotLight::SpotLight() :
				angle(90),
				range(1.0f),
				intensity(1.0f),
				attenuation({ 1.2f, 1, 1 }),
				transform(Transform()),
				// TODO: figure out the best value for this
				depthbuffer(DepthBuffer(200, 200)),
				// TODO: calculate width, height and fov with the light's angle
				projection_mat(Matrix4().SetPerspective(200, 200, 0.1f, 100.0f, 50))
			{
				// by calling the setters explicitly we also update the light matrix
				SetPosition(Vector3());
				SetDirection(Vector3());
			}

			SpotLight::SpotLight(const Vector3& position, const Vector3& direction) :
				angle(90),
				range(1.0f),
				intensity(1.0f),
				attenuation({ 1.2f, 1, 1 }),
				transform(Transform()),
				// TODO: figure out the best value for this
				depthbuffer(DepthBuffer(200, 200)),
				// TODO: calculate width, height and fov with the light's angle
				projection_mat(Matrix4().SetPerspective(200, 200, 0.1f, 100.0f, 50))
			{
				SetPosition(Vector3());
				SetDirection(Vector3());
			}

			SpotLight::SpotLight(const Vector3& position, const Vector3& direction, float range) :
				angle(90),
				range(range),
				intensity(1.0f),
				attenuation({ 1.2f, 1, 1 }),
				transform(Transform()),
				// TODO: figure out the best value for this
				depthbuffer(DepthBuffer(200, 200)),
				// TODO: calculate width, height and fov with the light's angle
				projection_mat(Matrix4().SetPerspective(200, 200, 0.1f, 100.0f, 50))
			{
				SetPosition(Vector3());
				SetDirection(Vector3());
			}

			void SpotLight::UpdateLightMatrix()
			{
				light_mat = projection_mat * (transform.GetRotationMatrix() * transform.GetTranslationMatrix());
			}

			Vector3 SpotLight::GetPosition() const
			{
				return position;
			}

			void SpotLight::SetPosition(const Vector3& position)
			{
				this->position = position;

				// recalculate the light_matrix
				transform.SetTranslation(position);

				UpdateLightMatrix();
			}

			Vector3 SpotLight::GetDirection() const
			{
				return direction;
			}

			void SpotLight::SetDirection(const Vector3& direction)
			{
				this->direction = direction;

				// recalculate the light_matrix
				float pitch = pitch = std::atan(direction.z / std::sqrt(direction.x * direction.x + direction.y * direction.y));
				float yaw = std::atan(direction.y / direction.x);

				transform.SetRotation(Angle(pitch, yaw, 0));

				UpdateLightMatrix();
			}

			float SpotLight::GetAngle() const
			{
				return angle;
			}

			void SpotLight::SetAngle(float angle)
			{
				this->angle = angle;
			}

			float SpotLight::GetRange() const
			{
				return range;
			}

			void SpotLight::SetRange(float range)
			{
				this->range = range;
			}

			float SpotLight::GetIntensity() const
			{
				return intensity;
			}

			void SpotLight::SetIntensity(float intensity)
			{
				this->intensity = intensity;
			}

			float SpotLight::GetLightAmountAt(const Vertex& vertex) const
			{
				Vector3 light_dir = vertex.GetPosition() - position;
				float light_dist = light_dir.GetLength();

				if (light_dist > range)
					return 0;

				light_dir.Normalize();

				float spotfactor = light_dir.GetDotProduct(direction);

				float ang = Math::Util::ToDegrees(std::acosf(spotfactor));

				if (ang > angle)
					return 0;

				float attenuation_amount = attenuation.c + attenuation.b * light_dist + attenuation.a * light_dist * light_dist + 0.0001f;

				float amount = vertex.GetNormal().GetDotProduct(-light_dir);

				amount = (amount * intensity) / attenuation_amount;

				return std::max(0.0f, amount);
			}

			bool SpotLight::IsShadowCaster() const
			{
				return true;
			}

			std::optional<std::reference_wrapper<const Matrix4>> SpotLight::GetLightMatrix() const
			{
				return std::optional(std::reference_wrapper(light_mat));
			}

			std::optional<std::reference_wrapper<DepthBuffer>> SpotLight::GetLightDepthBuffer()
			{
				return std::optional(std::reference_wrapper(depthbuffer));
			}

		}
	}
}
