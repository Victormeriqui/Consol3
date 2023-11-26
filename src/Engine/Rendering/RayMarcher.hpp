#ifndef RAYMARCHER_HPP
#define RAYMARCHER_HPP

#include "DepthBuffer.hpp"
#include "Display/IFrameDrawer.hpp"
#include "Display/RGBColor.hpp"
#include "Lighting/LightingSystem.hpp"
#include "Math/Matrix4.hpp"
#include "Ray.hpp"
#include "Transform.hpp"
#include "Vector3I.hpp"
#include "VoxelGrid.hpp"

#include <cstdint>
#include <memory>

namespace Engine
{
    namespace Rendering
    {
        using namespace Display;
        using namespace Math;
        using namespace Lighting;

        struct MarchResult
        {
            bool did_hit;
            Vector3 hit_position;
            Vector3 hit_normal;
            const VoxelData* voxel_data_ptr;
        };

        class RayMarcher
        {
        private:
            Matrix4 view_mat;
            Matrix4 inverse_view_mat;
            Matrix4 projection_mat;
            Matrix4 inverse_projection_mat;

            std::shared_ptr<IFrameDrawer> frame_drawer;

            Vector3I CalculateNearestVoxelGridCoords(const Vector3I& cur_grid_coords, const Vector3& direction) const;
            Vector3 CalculateTMax(const Vector3I& near_grid_coords, const Ray& ray) const;
            Vector3 CalculateDelta(const Vector3& direction) const;

            int CalculateTMax(float direction_component) const;

            MarchResult MarchUntilHit(const Ray& ray, const VoxelGrid& voxel_grid, uint16_t max_iterations) const;

        public:
            RayMarcher(std::shared_ptr<IFrameDrawer> frame_drawer);

            void SetViewMatrix(const Matrix4& view_matrix);
            void SetProjectionMatrix(const Matrix4& projection_matrix);

            void DrawVoxelGridLight(DepthBuffer& depthbuffer, const VoxelGrid& voxel_grid, const Vector3& origin);
            void DrawVoxelGridCamera(DepthBuffer& depthbuffer, const VoxelGrid& voxel_grid, const Vector3& origin, const LightingSystem& lighting_system);
            void DrawPixel(uint16_t x, uint16_t y, const RGBColor& color);
        };
    }
}

#endif
