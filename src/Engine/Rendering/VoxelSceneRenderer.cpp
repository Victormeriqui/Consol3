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
        VoxelSceneRenderer::VoxelSceneRenderer(
            std::shared_ptr<IFrameDrawer> frame_drawer, std::shared_ptr<ResourceManager> resource_manager, std::shared_ptr<LightingSystem> lighting_system, std::shared_ptr<Camera> camera, std::shared_ptr<VoxelGrid> voxel_grid) :
            frame_drawer(std::move(frame_drawer)),
            resource_manager(std::move(resource_manager)),
            lighting_system(std::move((lighting_system))),
            camera(std::move(camera)),
            voxel_grid(std::move(voxel_grid))
        {
        }

        void VoxelSceneRenderer::DrawPixel(uint16_t x, uint16_t y, const RGBColor& color)
        {
            frame_drawer->SetPixel(x, y, color);
        }

        Vector3I VoxelSceneRenderer::CalculateNearestVoxelGridCoords(const Vector3I& cur_grid_coords, const Vector3& direction) const
        {
            return Vector3I(cur_grid_coords.x + (direction.x > 0.0f ? 1 : 0), cur_grid_coords.y + (direction.y > 0.0f ? 1 : 0), cur_grid_coords.z + (direction.z > 0.0f ? 1 : 0));
        }

        Vector3 VoxelSceneRenderer::CalculateTMax(const Vector3I& near_grid_coords, const Ray& ray) const
        {
            // grid coords to world coords
            Vector3 tmax = near_grid_coords;

            tmax -= ray.origin;
            tmax /= ray.direction;
            return tmax;
        }

        Vector3 VoxelSceneRenderer::CalculateDelta(const Vector3& direction) const
        {
            Vector3 delta;
            Vector3 abs_dir = direction.GetAbsoluteValue();

            if (abs_dir.x > 0.0f)
                delta.x = 1.0f / abs_dir.x;
            if (abs_dir.y > 0.0f)
                delta.y = 1.0f / abs_dir.y;
            if (abs_dir.z > 0.0f)
                delta.z = 1.0f / abs_dir.z;

            return delta;
        }

        MarchResult VoxelSceneRenderer::MarchUntilHit(const Ray& ray, uint16_t max_iterations) const
        {
            MarchResult res = { .did_hit = false, .hit_position = Vector3(), .hit_normal = Vector3(), .voxel_data_ptr = nullptr };

            Vector3I step             = ray.direction.GetSignVector();
            Vector3I cur_grid_coords  = voxel_grid->GetGridPosition(ray.origin);
            Vector3I near_grid_coords = CalculateNearestVoxelGridCoords(cur_grid_coords, ray.direction);
            Vector3 t_max             = CalculateTMax(near_grid_coords, ray);
            Vector3 delta             = CalculateDelta(ray.direction);

            uint16_t i = 0;
            while (i++ < max_iterations)
            {
                // ray shot off the grid
                if (!voxel_grid->IsPositionInsideGrid(cur_grid_coords))
                {
                    res.did_hit = false;
                    return res;
                }

                res.voxel_data_ptr = voxel_grid->GetVoxelDataPtr(cur_grid_coords);

                if (res.voxel_data_ptr->type != VoxelElement::AIR)
                {
                    res.did_hit = true;
                    return res;
                }

                if (t_max.x < t_max.y && t_max.x < t_max.z)
                {
                    cur_grid_coords.x += step.x;
                    t_max.x += delta.x;
                    res.hit_normal   = Vector3(static_cast<float>(-step.x), 0.0f, 0.0f);
                    res.hit_position = ray.origin + ray.direction * t_max.x;
                }
                else if (t_max.y < t_max.z)
                {
                    cur_grid_coords.y += step.y;
                    t_max.y += delta.y;

                    res.hit_normal   = Vector3(0.0f, static_cast<float>(-step.y), 0.0f);
                    res.hit_position = ray.origin + ray.direction * t_max.y;
                }
                else
                {
                    cur_grid_coords.z += step.z;
                    t_max.z += delta.z;

                    res.hit_normal   = Vector3(0.0f, 0.0f, static_cast<float>(-step.z));
                    res.hit_position = ray.origin + ray.direction * t_max.z;
                }
            }

            return res;
        }

        void VoxelSceneRenderer::RenderScene(int64_t delta)
        {
            Vector3 camera_pos = camera->GetPosition();

            Vector3 camera_look     = camera->GetLookDirection();
            float camera_fov        = camera->GetFOV();
            Matrix4 view_matrix_inv = camera->GetViewMatrix().GetInverted();
            Matrix4 proj_matrix_inv = camera->GetProjectionMatrix().GetInverted();

            for (uint16_t y = 0; y < frame_drawer->GetFrameBufferHeight(); y++)
            {
                for (uint16_t x = 0; x < frame_drawer->GetFrameBufferWidth(); x++)
                {
                    float ndc_x = -1.0f + 2.0f * static_cast<float>(x) / static_cast<float>(frame_drawer->GetFrameBufferHeight() - 1);
                    float ndc_y = 1.0f - 2.0f * static_cast<float>(y) / static_cast<float>(frame_drawer->GetFrameBufferHeight() - 1);

                    Vector3 pixel_point = Vector3(ndc_x, ndc_y, 1.0f);
                    // ndc to world
                    pixel_point *= proj_matrix_inv;
                    // camera transform
                    pixel_point *= view_matrix_inv;

                    Ray ray = Ray(camera_pos, camera_pos.GetDirectionalTo(pixel_point));

                    MarchResult march_res = MarchUntilHit(ray, 1000);

                    if (!march_res.did_hit)
                        continue;

                    RGBColor voxel_color = voxel_color_map[march_res.voxel_data_ptr->type][march_res.voxel_data_ptr->color_index];
                    RGBColor lit_color   = lighting_system->GetLitColorAt(march_res.hit_position, march_res.hit_normal, camera_pos, MaterialProperties());

                    voxel_color.BlendMultiply(lit_color);

                    DrawPixel(x, y, voxel_color);
                }
            }
        }
    }
}