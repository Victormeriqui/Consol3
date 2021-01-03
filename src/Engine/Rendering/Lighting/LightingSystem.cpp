#include "LightingSystem.hpp"

#include "../../../Math/Util/MathUtil.hpp"
#include "../../../Math/Vector3.hpp"
#include "../Model.hpp"
#include "../Transform.hpp"
#include "../Vertex.hpp"

#include <cstdint>
#include <memory>
#include <vector>

namespace Engine
{
	namespace Rendering
	{
		namespace Lighting
		{
			using namespace Math;

			LightingSystem::LightingSystem() : lights(std::vector<std::shared_ptr<ILight>>()), ambient_light(0.01f)
			{
			}

			void LightingSystem::AddLight(std::shared_ptr<ILight> light)
			{
				lights.push_back(std::move(light));
			}

			void LightingSystem::RemoveLight(int index)
			{
				lights.erase(lights.begin() + index);
			}

			const std::vector<std::shared_ptr<ILight>> LightingSystem::GetLights() const
			{
				return lights;
			}

			void LightingSystem::SetAmbientLight(float ambient_light)
			{
				this->ambient_light = ambient_light;
			}

			float LightingSystem::GetAmbientLight() const
			{
				return ambient_light;
			}

			float LightingSystem::GetLightAmountAt(const Vertex& vertex, const Vector3 vertex_position_lights[]) const
			{
				return GetLightAmountAt(vertex.GetPosition(), vertex.GetNormal(), vertex_position_lights);
			}

			float LightingSystem::GetLightAmountAt(const Vector3& position, const Vector3& normal, const Vector3 position_lights[]) const
			{
				float light_amount = 0.0f;

				uint8_t i = 0;
				for (std::shared_ptr<ILight> light : lights)
				{
					if (!light->IsShadowCaster())
					{
						light_amount += light->GetLightAmountAt(position, normal);
						continue;
					}

					const Vector3 position_light = position_lights[i++];

					if (!Util::IsInRange(position_light.x, -1.0f, 1.0f) || !Util::IsInRange(position_light.y, -1.0f, 1.0f))
					{
						light_amount += light->GetLightAmountAt(position, normal);
						continue;
					}

					uint16_t depthbuffer_x = (uint16_t)Util::Lerp(position_light.x, -1, 1, 0, 199);
					uint16_t depthbuffer_y = (uint16_t)Util::Lerp(position_light.y, 1, -1, 0, 199);

					float light_depth = light->GetLightDepthBuffer().value().get().GetValue(depthbuffer_x, depthbuffer_y);

					if (position_light.z < (light_depth + light->GetBias().value()))
						light_amount += light->GetLightAmountAt(position, normal);
				}

				return light_amount;
			}

			void LightingSystem::ClearDepthBuffers()
			{
				for (std::shared_ptr<ILight> light : lights)
				{
					if (!light->IsShadowCaster() || !light->GetLightDepthBuffer().has_value())
						continue;

					light->ClearDepthBuffer();
				}
			}
		}
	}
}
