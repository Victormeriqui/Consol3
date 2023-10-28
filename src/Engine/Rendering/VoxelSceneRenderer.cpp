#include "VoxelSceneRenderer.hpp"

#include "Display/RGBColorConstants.hpp"
#include "Math/Matrix4.hpp"
#include "Math/Quaternion.hpp"
#include "Math/Util/MathUtil.hpp"
#include "Math/Vector3.hpp"

using namespace Math;
using namespace Math::Util;

namespace Engine
{
    namespace Rendering
    {
        VoxelSceneRenderer::VoxelSceneRenderer(std::shared_ptr<IFrameDrawer> frame_drawer,
                                               std::shared_ptr<ResourceManager> resource_manager,
                                               std::shared_ptr<Camera> camera,
                                               std::shared_ptr<VoxelGrid<VOXEL_GRID_WIDTH, VOXEL_GRID_HEIGHT, VOXEL_GRID_DEPTH>> voxel_grid) :
            frame_drawer(std::move(frame_drawer)),
            resource_manager(std::move(resource_manager)),
            camera(std::move(camera)),
            voxel_grid(std::move(voxel_grid))
        {
        }

        void VoxelSceneRenderer::DrawPixel(uint16_t x, uint16_t y, const RGBColor& color)
        {
            frame_drawer->SetPixel(x, y, color);
        }

        void VoxelSceneRenderer::RenderScene(int64_t delta)
        {
            Vector3 ray_origin  = camera->GetPosition();
            Vector3 camera_look = camera->GetLookDirection();
            float camera_fov    = camera->GetFOV();
            Matrix4 view_matrix = camera->GetViewMatrix();
            Matrix4 proj_matrix = camera->GetProjectionMatrix();
            Matrix4 inv_vp      = proj_matrix * view_matrix;
            inv_vp.Invert();

            float step_size = 0.5f;

            for (uint16_t y = 0; y < frame_drawer->GetFrameBufferHeight(); y++)
            {
                for (uint16_t x = 0; x < frame_drawer->GetFrameBufferWidth(); x++)
                {
                    Vector3 camera_space_ray;
                    // NDC coordinates are the same as the camera space ray from center to the pixel
                    camera_space_ray.x = 2.0f * (static_cast<float>(x) / frame_drawer->GetFrameBufferWidth()) - 1.0f;
                    camera_space_ray.y = 1.0f - 2.0f * (static_cast<float>(y) / frame_drawer->GetFrameBufferHeight());
                    camera_space_ray.z = 1.0f;

                    // trasnform with inverse of view projection to rotate our camera space ray to world space like the camera look vector
                    Vector3 world_space_ray = camera_space_ray * inv_vp;
                    world_space_ray.Normalize();

                    float cur_step = 0.1f;
                    while (true)
                    {
                        Vector3 cur_march_pos = ray_origin + world_space_ray * cur_step;

                        bool inside_voxel_grid = IsInRange(cur_march_pos.x, 0, VOXEL_GRID_WIDTH - 1) && IsInRange(cur_march_pos.y, 0, VOXEL_GRID_HEIGHT - 1) && IsInRange(cur_march_pos.z, 0, VOXEL_GRID_DEPTH - 1);

                        if (!inside_voxel_grid)
                            break;

                        uint16_t grid_x = static_cast<uint16_t>(cur_march_pos.x);
                        uint16_t grid_y = static_cast<uint16_t>(cur_march_pos.y);
                        uint16_t grid_z = static_cast<uint16_t>(cur_march_pos.z);

                        uint8_t cur_voxel = voxel_grid->GetVoxel(grid_x, grid_y, grid_z);

                        if (cur_voxel == 1)
                        {
                            DrawPixel(x, y, RGBColor(grid_x * 10, grid_y * 10, grid_z * 10));
                            break;
                        }
                        cur_step += step_size;

                        if (cur_step > 1000)
                            break;
                    }
                }
            }
        }
    }
}
