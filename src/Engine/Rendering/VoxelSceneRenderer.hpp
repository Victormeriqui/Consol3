
#ifndef VOXELSCENERENDERER_HPP
#define VOXELSCENERENDERER_HPP

#include "Camera.hpp"
#include "Display/IFrameDrawer.hpp"
#include "Display/NullFrameDrawer.hpp"
#include "Display/RGBColor.hpp"
#include "Engine/Resources/ResourceManager.hpp"
#include "Lighting/LightingSystem.hpp"
#include "Ray.hpp"
#include "RayMarcher.hpp"
#include "Vector3I.hpp"
#include "VoxelGrid.hpp"

#include <cstdint>
#include <memory>

namespace Engine
{
    namespace Rendering
    {
        using namespace Display;
        using namespace Resources;
        using namespace Lighting;
        using namespace Math;

        class VoxelSceneRenderer
        {
        private:
            std::shared_ptr<IFrameDrawer> frame_drawer;
            // a non functioning frame drawer for the shadowmap ray marcher
            std::shared_ptr<NullFrameDrawer> null_frame_drawer;

            RayMarcher ray_marcher;
            // a secondary ray marcher that writes to the light's depthbuffer, this is prefered so the main  ray marcher doesn't need to be affected
            RayMarcher shadowmap_ray_marcher;

            std::shared_ptr<LightingSystem> lighting_system;
            std::shared_ptr<Camera> camera;

            std::shared_ptr<VoxelGrid> voxel_grid;

            void RenderShadowMapPass();

        public:
            VoxelSceneRenderer(std::shared_ptr<LightingSystem> lighting_system, std::shared_ptr<Camera> camera, std::shared_ptr<VoxelGrid> voxel_grid);

            void SetFrameDrawer(std::shared_ptr<IFrameDrawer> frame_drawer);

            void DrawPixel(uint16_t x, uint16_t y, const RGBColor& color);

            void RenderScene(int64_t delta);
            void RenderSceneShared(int64_t delta);
        };
    }
}

#endif
