#include "LightingSystem.hpp"

#include "../Vertex.hpp"
#include "../../../Math/Vector3.hpp"
#include "LightRenderer.hpp"
#include "../Transform.hpp"
#include "../Model.hpp"

#include <vector>
#include <memory>

namespace Engine
{
	namespace Rendering
	{
		namespace Lighting
		{
			LightingSystem::LightingSystem() :
				lights(std::vector<std::shared_ptr<ILight>>()),
				lights_renderer(std::make_shared<LightRenderer>()),
				lights_rasterizer(Rasterizer(lights_renderer))
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
				return GetLightAmountAt(vertex.GetPosition(), vertex.GetNormal());
			}

			float LightingSystem::GetLightAmountAt(const Vector3& position, const Vector3& normal) const
			{
				float light_amount = 0.0f;

				for (std::shared_ptr<ILight> light : lights)
					light_amount += light->GetLightAmountAt(position, normal);

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

			void LightingSystem::RenderToDepthBuffers(const Model& model, const Transform transform)
			{
				for (std::shared_ptr<ILight> light : lights)
				{
					if (!light->IsShadowCaster())
						continue;

					if (!light->GetProjectionMatrix().has_value() || !light->GetViewMatrix().has_value() || !light->GetLightDepthBuffer().has_value())
						continue;

					lights_rasterizer.SetProjectionMatrix(light->GetProjectionMatrix().value());
					lights_rasterizer.SetViewMatrix(light->GetViewMatrix().value());

					// draw the model to the light's depthbuffer
					model.DrawModel(transform, light->GetLightDepthBuffer().value(), lights_rasterizer, HSVColor());
				}
			}
		}
	}
}