#include "VoxelSceneRenderer.hpp"

#include "Display/RGBColorConstants.hpp"
#include "Math/Matrix4.hpp"
#include "Math/Quaternion.hpp"
#include "Math/Vector3.hpp"

using namespace Math;

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
                    camera_space_ray.x = 2.0f * (static_cast<float>(x) / frame_drawer->GetFrameBufferWidth()) - 1.0f;
                    camera_space_ray.y = 1.0f - 2.0f * (static_cast<float>(y) / frame_drawer->GetFrameBufferHeight());
                    camera_space_ray.z = 1.0f;

                    Vector3 world_space_ray = camera_space_ray * inv_vp;
                    world_space_ray.Normalize();

                    // Vector3 ray_dir = camera->GetLookDirection() * camera_space_dir;
                    float t = 0.1f;
                    while (true)
                    {
                        Vector3 cur_march_pos = ray_origin + world_space_ray * t;

                        if (cur_march_pos.x >= 0 && cur_march_pos.x < VOXEL_GRID_WIDTH && cur_march_pos.y >= 0 && cur_march_pos.y < VOXEL_GRID_HEIGHT && cur_march_pos.z >= 0 && cur_march_pos.z < VOXEL_GRID_DEPTH)
                        {
                            uint16_t grid_x = static_cast<uint16_t>(cur_march_pos.x);
                            uint16_t grid_y = static_cast<uint16_t>(cur_march_pos.y);
                            uint16_t grid_z = static_cast<uint16_t>(cur_march_pos.z);

                            uint8_t cur_voxel = voxel_grid->GetVoxel(grid_x, grid_y, grid_z);

                            if (cur_voxel == 1)
                            {
                                DrawPixel(x, y, RGBColor(grid_x * 10, grid_y * 10, grid_z * 10));
                                break;
                            }
                            t += step_size;

                            if (t > 1000)
                                break;
                        }
                        else
                            break;
                    }
                }
            }
        }
    }
}
