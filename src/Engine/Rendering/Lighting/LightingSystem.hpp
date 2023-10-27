#ifndef LIGHTINGSYSTEM_HPP
#define LIGHTINGSYSTEM_HPP

#include "ILight.hpp"

#include "Display/RGBColor.hpp"
#include "Engine/Rendering/DepthBuffer.hpp"
#include "Engine/Rendering/Rasterizer.hpp"
#include "Engine/Rendering/Transform.hpp"
#include "Engine/Rendering/Vertex.hpp"
#include "MaterialProperties.hpp"
#include "Math/Vector3.hpp"

#include <memory>
#include <vector>

namespace Engine
{
    namespace Rendering
    {
        namespace Lighting
        {
            using namespace Display;

            class LightingSystem
            {
            private:
                RGBColor ambient_light_color;
                std::vector<std::shared_ptr<ILight>> lights;

            public:
                LightingSystem();

                void AddLight(std::shared_ptr<ILight> light);
                void RemoveLight(int index);

                void SetAmbientLightColor(RGBColor color);

                [[nodiscard]] RGBColor GetAmbientLightColor() const;

                const std::vector<std::shared_ptr<ILight>> GetLights() const;

                [[nodiscard]] RGBColor GetLitColorAt(const Vertex& vertex, const Vector3& cam_pos, const Vector3 vertex_position_lights[], const MaterialProperties& material_properties) const;
                [[nodiscard]] RGBColor GetLitColorAt(const Vector3& position, const Vector3& normal, const Vector3& cam_pos, const Vector3 position_lights[], const MaterialProperties& material_properties) const;

                void ClearDepthBuffers();
            };
        }
    }
}

#endif
