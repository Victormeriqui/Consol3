#include "VoxelSceneRenderer.hpp"

#include "Display/RGBColorConstants.hpp"
#include "Engine/VoxelElements.hpp"
#include "Math/Matrix4.hpp"
#include "Math/Util/MathUtil.hpp"
#include "Math/Vector3.hpp"

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

        MarchResult VoxelSceneRenderer::MarchUntilHit(const Ray& ray, float step_size, float max_step) const
        {
            Ray ret_ray     = ray;
            float cur_delta = 0.0f;
            while (true)
            {
                // ret_ray.origin = ret_ray.March(cur_delta);

                if (!voxel_grid->IsPositionInsideGrid(ret_ray.origin))
                    return { .ray = ret_ray, .did_hit = false, .voxel_data_ptr = nullptr };

                VoxelData* cur_voxel_data_ptr = voxel_grid->GetVoxelDataPtr(ret_ray.origin);

                if (cur_voxel_data_ptr->type != VoxelElement::AIR)
                    return { .ray = ret_ray, .did_hit = true, .voxel_data_ptr = cur_voxel_data_ptr };

                Vector3 dir_sign = ret_ray.direction.GetSignVector();
                Vector3 pos      = ret_ray.origin * dir_sign;
                float temp       = 0.0f;
                pos.x            = 1.0f - std::modf(pos.x, &temp) + 1e-5f;
                pos.y            = 1.0f - std::modf(pos.y, &temp) + 1e-5f;
                pos.z            = 1.0f - std::modf(pos.z, &temp) + 1e-5f;

                Vector3 dir_abs = ret_ray.direction.GetAbsoluteValue();

                Vector3 delta_vec = pos / dir_abs;

                float min_delta       = std::min({ delta_vec.x, delta_vec.y, delta_vec.z });
                Vector3 min_delta_vec = Vector3(min_delta, min_delta, min_delta);
                float x_hit           = min_delta_vec.x == delta_vec.x ? 1.0f : 0.0f;
                float y_hit           = min_delta_vec.y == delta_vec.y ? 1.0f : 0.0f;
                float z_hit           = min_delta_vec.z == delta_vec.z ? 1.0f : 0.0f;
                Vector3 hitMask       = Vector3(x_hit, y_hit, z_hit);

                ret_ray.origin = ret_ray.origin + ret_ray.direction * min_delta;

                ret_ray.direction = -hitMask * dir_sign;
            }
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

                    Ray ray = Ray(camera_pos, (pixel_point - camera_pos).GetNormalized());

                    MarchResult march_res = MarchUntilHit(ray, step_size, 1000.0f);

                    if (!march_res.did_hit)
                        continue;

                    RGBColor voxel_color = voxel_color_map[march_res.voxel_data_ptr->type][march_res.voxel_data_ptr->color_index];

                    DrawPixel(x, y, voxel_color);
                }
            }
        }
    }
}