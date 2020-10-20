#include "LightingSystem.hpp"

#include "../Vertex.hpp"
#include "../../../Math/Vector3.hpp"

#include <vector>
#include <memory>

namespace Engine
{
	namespace Rendering
	{
		namespace Lighting
		{
			LightingSystem::LightingSystem() :
				lights(std::vector<std::shared_ptr<ILight>>())
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

			float LightingSystem::GetLightAmountAt(const Vertex& vertex) const
			{
				float light_amount = 0.0f;

				for (std::shared_ptr<ILight> light : lights)
					light_amount += light->GetLightAmountAt(vertex);

				return light_amount;
			}
		}
	}
}