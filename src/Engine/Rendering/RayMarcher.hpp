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

        class RayMarcher
        {
        private:
            Matrix4 view_mat;
            Matrix4 inverse_view_mat;
            Matrix4 projection_mat;
            Matrix4 inverse_projection_mat;

            std::shared_ptr<IFrameDrawer> frame_drawer;

            Vector3 hit_ndc_light_space[10];

            Ray SetupRayPerspective(uint16_t x, uint16_t y, const Vector3& origin) const;
            Ray SetupRayOrtho(uint16_t x, uint16_t y) const;
            Vector3 HitPositionToNDCPerspective(const Vector3& hit_pos) const;
            Vector3 HitPositionToNDCOrtho(Vector3 hit_pos) const;
            void SetupLightSpacePerspective(const Vector3& hit_pos, const LightingSystem& lighting_system);
            void SetupLightSpaceOrtho(const Vector3& hit_pos, const LightingSystem& lighting_system);

        public:
            RayMarcher();

            void SetFrameDrawer(std::shared_ptr<IFrameDrawer> frame_drawer);

            void SetViewMatrix(const Matrix4& view_matrix);
            void SetProjectionMatrix(const Matrix4& projection_matrix);

            void DrawVoxelGridDepthOnlyPerspective(DepthBuffer& depthbuffer, const VoxelGrid& voxel_grid, const Vector3& origin);
            void DrawVoxelGridDepthOnlyOrtho(DepthBuffer& depthbuffer, const VoxelGrid& voxel_grid);
            void DrawVoxelGridPerspective(DepthBuffer& depthbuffer, const VoxelGrid& voxel_grid, const Vector3& origin, const LightingSystem& lighting_system);
            void DrawPixel(uint16_t x, uint16_t y, const RGBColor& color);
        };
    }
}

#endif
