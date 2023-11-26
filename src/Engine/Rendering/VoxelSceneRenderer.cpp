#include "VoxelSceneRenderer.hpp"

#include "Display/RGBColorConstants.hpp"
#include "Engine/VoxelElements.hpp"
#include "Math/Matrix4.hpp"
#include "Math/Util/MathUtil.hpp"
#include "Math/Vector3.hpp"

#include <cmath>
#include <limits>

using namespace Math::Util;

namespace Engine
{
    namespace Rendering
    {
        VoxelSceneRenderer::VoxelSceneRenderer(std::shared_ptr<IFrameDrawer> frame_drawer, std::shared_ptr<LightingSystem> lighting_system, std::shared_ptr<Camera> camera, std::shared_ptr<VoxelGrid> voxel_grid) :
            frame_drawer(std::move(frame_drawer)),
            null_frame_drawer(std::make_shared<NullFrameDrawer>()),
            ray_marcher(this->frame_drawer),
            shadowmap_ray_marcher(null_frame_drawer),
            lighting_system(std::move((lighting_system))),
            camera(std::move(camera)),
            voxel_grid(std::move(voxel_grid))
        {
            ray_marcher.SetProjectionMatrix(this->camera->GetProjectionMatrix());
        }

        void VoxelSceneRenderer::RenderShadowMapPass()
        {
            for (std::shared_ptr<ILight> light : lighting_system->GetLights())
            {
                if (!light->IsShadowCaster())
                    continue;

                shadowmap_ray_marcher.SetProjectionMatrix(light->GetProjectionMatrix().value());
                shadowmap_ray_marcher.SetViewMatrix(light->GetViewMatrix().value());

                static const RGBColor nocolor;

                shadowmap_ray_marcher.DrawVoxelGridLight(light->GetLightDepthBuffer().value(), *voxel_grid, light->GetPosition());
            }
        }

        void VoxelSceneRenderer::RenderScene(int64_t delta)
        {
            camera->ClearDepthBuffer();
            lighting_system->ClearDepthBuffers();

            RenderShadowMapPass();

            ray_marcher.SetViewMatrix(camera->GetViewMatrix());

            ray_marcher.DrawVoxelGridCamera(camera->GetDepthBuffer(), *voxel_grid, camera->GetPosition(), *lighting_system);

            for (uint16_t y = 0; y < frame_drawer->GetFrameBufferHeight(); y++)
            {
                for (uint16_t x = 0; x < frame_drawer->GetFrameBufferWidth(); x++)
                {
                    // float z     = camera->GetDepthBuffer().GetValue(x, y);
                    float z     = lighting_system->GetLights().at(0)->GetLightDepthBuffer().value().get().GetValue(x, y);
                    uint16_t nx = Util::LerpCast<uint16_t>(x / 200.0f, 0, 100);
                    uint16_t ny = Util::LerpCast<uint16_t>(y / 200.0f, 0, 100);
                    ray_marcher.DrawPixel(nx, ny, RGBColor(Util::Lerp(z, 1.0f, 0.0f), Util::Lerp(z, 1.0f, 0.0f), Util::Lerp(z, 1.0f, 0.0f)));
                }
            }
        }

        void VoxelSceneRenderer::DrawPixel(uint16_t x, uint16_t y, const RGBColor& color)
        {
            ray_marcher.DrawPixel(x, y, color);
        }

    }
}