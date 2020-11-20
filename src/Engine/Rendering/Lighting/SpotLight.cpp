#include "SpotLight.hpp"

#include "../../../Math/Vector3.hpp"
#include "../Vertex.hpp"
#include "../../../Math/Util/MathUtil.hpp"

#include <algorithm>
#include <cmath>

namespace Engine
{
	namespace Rendering
	{
		namespace Lighting
		{
			SpotLight::SpotLight() : position(Vector3()), direction(Vector3()), angle(90), range(1.0f), intensity(1.0f), attenuation({ 1.2f, 1, 1 })
			{
			}

			SpotLight::SpotLight(const Vector3& position, const Vector3& direction) : position(position), direction(direction), angle(90), range(1.0f), intensity(1.0f), attenuation({ 1.2f, 1, 1 })
			{
			}

			SpotLight::SpotLight(const Vector3& position, const Vector3& direction, float range) : position(position), direction(direction), angle(90), range(range), intensity(1.0f), attenuation({ 1.2f, 1, 1 })
			{
			}

			Vector3 SpotLight::GetPosition() const
			{
				return position;
			}

			void SpotLight::SetPosition(const Vector3& position)
			{
				this->position = position;
			}

			Vector3 SpotLight::GetDirection() const
			{
				return direction;
			}

			void SpotLight::SetDirection(const Vector3& direction)
			{
				this->direction = direction;
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

				float amount = -light_dir.GetDotProduct(vertex.GetNormal());

				amount = (amount * intensity) / attenuation_amount;

				return std::max(0.0f, amount);
			}
		}
	}
}
