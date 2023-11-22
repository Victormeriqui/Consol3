
#ifndef VOXELSCENERENDERER_HPP
#define VOXELSCENERENDERER_HPP

#include "Camera.hpp"
#include "Display/IFrameDrawer.hpp"
#include "Display/RGBColor.hpp"
#include "Engine/Resources/ResourceManager.hpp"
#include "Lighting/LightingSystem.hpp"
#include "Ray.hpp"
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

        struct MarchResult
        {
            Ray ray;
            bool did_hit;
            VoxelData* voxel_data_ptr;
        };

        class VoxelSceneRenderer
        {
        private:
            std::shared_ptr<IFrameDrawer> frame_drawer;
            std::shared_ptr<ResourceManager> resource_manager;

            std::shared_ptr<LightingSystem> lighting_system;

            std::shared_ptr<Camera> camera;

            std::shared_ptr<VoxelGrid> voxel_grid;

            [[nodiscard]] Vector3I CalculateNearestVoxelGridCoords(const Vector3I& cur_grid_coords, const Vector3& direction) const;
            [[nodiscard]] Vector3 CalculateTMax(const Vector3I& near_grid_coords, const Ray& ray) const;
            [[nodiscard]] Vector3 CalculateDelta(const Vector3& direction) const;

            [[nodiscard]] int CalculateTMax(float direction_component) const;

            MarchResult MarchUntilHit(const Ray& ray, float step_size, float max_step) const;

        public:
            VoxelSceneRenderer(std::shared_ptr<IFrameDrawer> frame_drawer,
                               std::shared_ptr<ResourceManager> resource_manager,
                               std::shared_ptr<LightingSystem> lighting_system,
                               std::shared_ptr<Camera> camera,
                               std::shared_ptr<VoxelGrid> voxel_grid);

            void SetVoxelPalette(std::unique_ptr<std::map<uint8_t, RGBColor>> voxel_palette);

            void DrawPixel(uint16_t x, uint16_t y, const RGBColor& color);

            void RenderScene(int64_t delta);
        };
    }
}

#endif
