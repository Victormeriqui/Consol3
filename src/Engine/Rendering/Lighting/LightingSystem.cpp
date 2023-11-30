#include "LightingSystem.hpp"

#include "Engine/Rendering/Transform.hpp"
#include "Engine/Rendering/Vertex.hpp"
#include "Math/Util/MathUtil.hpp"
#include "Math/Vector3.hpp"

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

            LightingSystem::LightingSystem() : ambient_light_color(RGBColor(255, 255, 255)), lights(std::vector<std::shared_ptr<ILight>>())
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

            void LightingSystem::SetAmbientLightColor(RGBColor ambient_light_color)
            {
                this->ambient_light_color = ambient_light_color;
            }

            RGBColor LightingSystem::GetAmbientLightColor() const
            {
                return ambient_light_color;
            }

            RGBColor LightingSystem::GetLitColorAt(const Vertex& vertex, const Vector3& cam_pos, const Vector3 vertex_position_lights[], const MaterialProperties& material_properties) const
            {
                return GetLitColorAt(vertex.GetPosition(), vertex.GetNormal(), cam_pos, vertex_position_lights, material_properties);
            }

            RGBColor LightingSystem::GetLitColorAt(const Vector3& position, const Vector3& normal, const Vector3& cam_pos, const Vector3 position_lights[], const MaterialProperties& material_properties) const
            {
                RGBColor final_color;

                uint8_t i = 0;
                for (std::shared_ptr<ILight> light : lights)
                {
                    if (!light->IsShadowCaster())
                    {
                        final_color += light->GetColorAt(position, normal, cam_pos, material_properties);

                        continue;
                    }

                    const Vector3 position_light = position_lights[i++];

                    // out of light's view range (outside the NDC coords from its perspective)
                    if (!Util::IsInRange<float>(position_light.x, -1.0f, 1.0f) || !Util::IsInRange<float>(position_light.y, -1.0f, 1.0f))
                    {
                        final_color += light->GetColorAt(position, normal, cam_pos, material_properties);

                        continue;
                    }

                    uint16_t depthbuffer_x = (uint16_t)Util::Lerp(position_light.x, -1, 1, 0, 199);
                    uint16_t depthbuffer_y = (uint16_t)Util::Lerp(position_light.y, 1, -1, 0, 199);

                    float light_depth = light->GetLightDepthBuffer().value().get().GetValue(depthbuffer_x, depthbuffer_y);

                    if (position_light.z < (light_depth + light->GetBias().value()))
                        final_color += light->GetColorAt(position, normal, cam_pos, material_properties);
                }

                return final_color;
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
