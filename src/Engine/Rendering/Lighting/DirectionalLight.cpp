#include "DirectionalLight.hpp"

#include "../../../Math/Vector3.hpp"
#include "../Vertex.hpp"

#include <algorithm>

namespace Engine
{
	namespace Rendering
	{
		namespace Lighting
		{
			DirectionalLight::DirectionalLight() : direction(Vector3()), intensity(1.0f)
			{
			}

			DirectionalLight::DirectionalLight(const Vector3& direction) : direction(direction), intensity(1.0f)
			{
			}

			DirectionalLight::DirectionalLight(const Vector3& direction, float intensity) : direction(direction), intensity(intensity)
			{

			}

			Vector3 DirectionalLight::GetDirection() const
			{
				return direction;
			}

			void DirectionalLight::SetDirection(const Vector3& direction)
			{
				this->direction = direction;
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
				float value = -direction.GetDotProduct(vertex.GetNormal()) * intensity;

				return std::max(0.0f, value);
			}
		}
	}
}
