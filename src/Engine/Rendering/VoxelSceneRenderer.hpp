
#ifndef VOXELSCENERENDERER_HPP
#define VOXELSCENERENDERER_HPP

#include "Camera.hpp"
#include "Display/IFrameDrawer.hpp"
#include "Display/RGBColor.hpp"
#include "Engine/Resources/ResourceManager.hpp"
#include "VoxelGrid.hpp"

#include <cstdint>
#include <memory>

#define GRID_WIDTH  100
#define GRID_HEIGHT 100
#define GRID_DEPTH  100

namespace Engine
{
    namespace Rendering
    {
        using namespace Display;
        using namespace Resources;

        class VoxelSceneRenderer
        {
        private:
            std::shared_ptr<IFrameDrawer> frame_drawer;

            std::shared_ptr<ResourceManager> resource_manager;

            std::shared_ptr<Camera> camera;

            VoxelGrid<uint8_t, GRID_WIDTH, GRID_HEIGHT, GRID_DEPTH> voxel_grid;

        public:
            VoxelSceneRenderer(std::shared_ptr<IFrameDrawer> frame_drawer, std::shared_ptr<ResourceManager> resource_manager, std::shared_ptr<Camera> camera);

            void DrawPixel(uint16_t x, uint16_t y, const RGBColor& color);

            void RenderScene(int64_t delta);
        };
    }
}

#endif
