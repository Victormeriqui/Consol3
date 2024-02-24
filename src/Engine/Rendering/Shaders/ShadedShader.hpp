#ifndef SHADEDSHADER_HPP
#define SHADEDSHADER_HPP

#include "IShader.hpp"

#include "Display/RGBColor.hpp"
#include "Engine/Rendering/DepthBuffer.hpp"
#include "Engine/Rendering/Lighting/LightingSystem.hpp"
#include "Engine/Rendering/Lighting/MaterialProperties.hpp"
#include "Engine/Rendering/Texture.hpp"
#include "Engine/Rendering/Vertex.hpp"
#include "Math/Matrix4.hpp"

#include <cstdint>
#include <functional>
#include <memory>

namespace Engine
{
    namespace Rendering
    {
        namespace Shaders
        {
            using namespace Display;
            using namespace Lighting;

            class ShadedShader : public IShader
            {
            private:
                std::shared_ptr<LightingSystem> lighting_system;
                Vector3 camera_position;

                std::shared_ptr<Texture> texture;
                std::shared_ptr<Texture> normal_map;

                // a pointer here is prefered to avoid calling make_shared on every triangle
                DepthBuffer* vert_light_depthbuffer[10];

                // non linear projections need to account for perspective
                bool vert_light_islinearprojection[10];

                Vector3 frag_position_lights[10];

                bool has_normal_map;
                MaterialProperties material_properties;

            public:
                virtual size_t GetFragmentContextSize() const override;
                virtual bool VertexShader(Vertex& v0, Vertex& v1, Vertex& v2, const MVPTransform& mvp_mats, void* context) override;
                virtual RGBColor FragmentShader(RGBColor color, const Triangle& triangle, float barcoord0, float barcoord1, float barcoord2, const void* context) override;

                void SetLightingSystem(std::shared_ptr<LightingSystem> lighting_system);

                void SetTexture(std::shared_ptr<Texture> texture);
                void SetNormalMap(std::shared_ptr<Texture> normal_map);
                void DisableNormalMap();
                void SetCameraPosition(const Vector3& cam_pos);
                void SetMaterialProperties(const MaterialProperties& material_properties);
            };
        }
    }
}

#endif
