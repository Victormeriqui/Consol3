
#ifndef VOXELSCENERENDERER_HPP
#define VOXELSCENERENDERER_HPP

#include "Camera.hpp"
#include "Display/IFrameDrawer.hpp"
#include "Display/RGBColor.hpp"
#include "Engine/Resources/ResourceManager.hpp"
#include "VoxelGrid.hpp"

#include <cstdint>
#include <memory>

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

            std::shared_ptr<VoxelGrid<VOXEL_GRID_WIDTH, VOXEL_GRID_HEIGHT, VOXEL_GRID_DEPTH>> voxel_grid;

        public:
            VoxelSceneRenderer(std::shared_ptr<IFrameDrawer> frame_drawer,
                               std::shared_ptr<ResourceManager> resource_manager,
                               std::shared_ptr<Camera> camera,
                               std::shared_ptr<VoxelGrid<VOXEL_GRID_WIDTH, VOXEL_GRID_HEIGHT, VOXEL_GRID_DEPTH>> voxel_grid);

            void SetVoxelPalette(std::unique_ptr<std::map<uint8_t, RGBColor>> voxel_palette);

            void DrawPixel(uint16_t x, uint16_t y, const RGBColor& color);

            void RenderScene(int64_t delta);
        };
    }
}

#endif
