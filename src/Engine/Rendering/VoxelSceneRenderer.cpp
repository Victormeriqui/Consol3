#include "VoxelSceneRenderer.hpp"

#include "Display/RGBColorConstants.hpp"
#include "Engine/VoxelTypes.hpp"
#include "Math/Matrix4.hpp"
#include "Math/Util/MathUtil.hpp"
#include "Math/Vector3.hpp"

using namespace Math;
using namespace Math::Util;

namespace Engine
{
    namespace Rendering
    {
        VoxelSceneRenderer::VoxelSceneRenderer(std::shared_ptr<IFrameDrawer> frame_drawer, std::shared_ptr<ResourceManager> resource_manager, std::shared_ptr<Camera> camera, std::shared_ptr<VoxelGrid> voxel_grid) :
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
            Vector3 camera_pos      = camera->GetPosition();
            Vector3 camera_look     = camera->GetLookDirection();
            float camera_fov        = camera->GetFOV();
            Matrix4 view_matrix_inv = camera->GetViewMatrix().GetInverted();
            Matrix4 proj_matrix_inv = camera->GetProjectionMatrix().GetInverted();

            float step_size = 0.1f;

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

                    Vector3 ray_dir = (pixel_point - camera_pos);
                    ray_dir.Normalize();

                    float cur_step = 0.5f;
                    while (true)
                    {
                        Vector3 cur_march_pos = camera_pos + ray_dir * cur_step;

                        if (!voxel_grid->IsPositionInsideGrid(cur_march_pos))
                            break;

                        uint16_t grid_x = static_cast<uint16_t>(cur_march_pos.x);
                        uint16_t grid_y = static_cast<uint16_t>(cur_march_pos.y);
                        uint16_t grid_z = static_cast<uint16_t>(cur_march_pos.z);

                        VoxelData cur_voxel_data = voxel_grid->GetVoxelData(grid_x, grid_y, grid_z);

                        if (cur_voxel_data.type != VoxelType::AIR)
                        {
                            DrawPixel(x, y, cur_voxel_data.color);
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
