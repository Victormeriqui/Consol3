#include "RayMarcher.hpp"

#include "Vertex.hpp"

namespace Engine
{
    namespace Rendering
    {
        using namespace Display;
        using namespace Math;

        RayMarcher::RayMarcher(std::shared_ptr<IFrameDrawer> frame_drawer) : frame_drawer(std::move(frame_drawer))
        {
        }

        void RayMarcher::DrawPixel(uint16_t x, uint16_t y, const RGBColor& color)
        {
            frame_drawer->SetPixel(x, y, color);
        }

        Vector3I RayMarcher::CalculateNearestVoxelGridCoords(const Vector3I& cur_grid_coords, const Vector3& direction) const
        {
            return Vector3I(cur_grid_coords.x + (direction.x > 0.0f ? 1 : 0), cur_grid_coords.y + (direction.y > 0.0f ? 1 : 0), cur_grid_coords.z + (direction.z > 0.0f ? 1 : 0));
        }

        Vector3 RayMarcher::CalculateTMax(const Vector3I& near_grid_coords, const Ray& ray) const
        {
            // grid coords to world coords
            Vector3 tmax = near_grid_coords;

            tmax -= ray.origin;
            tmax /= ray.direction;
            return tmax;
        }

        Vector3 RayMarcher::CalculateDelta(const Vector3& direction) const
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

        MarchResult RayMarcher::MarchUntilHit(const Ray& ray, const VoxelGrid& voxel_grid, uint16_t max_iterations) const
        {
            MarchResult res = { .did_hit = false, .hit_position = Vector3(), .hit_normal = Vector3(), .voxel_data_ptr = nullptr };

            Vector3I step             = ray.direction.GetSignVector();
            Vector3I cur_grid_coords  = voxel_grid.GetGridPosition(ray.origin);
            Vector3I near_grid_coords = CalculateNearestVoxelGridCoords(cur_grid_coords, ray.direction);
            Vector3 t_max             = CalculateTMax(near_grid_coords, ray);
            Vector3 delta             = CalculateDelta(ray.direction);

            uint16_t i = 0;
            while (i++ < max_iterations)
            {
                // ray shot off the grid
                if (!voxel_grid.IsPositionInsideGrid(cur_grid_coords))
                {
                    res.did_hit = false;
                    return res;
                }

                res.voxel_data_ptr = voxel_grid.GetVoxelDataPtr(cur_grid_coords);

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

        void RayMarcher::DrawVoxelGridLight(DepthBuffer& depthbuffer, const VoxelGrid& voxel_grid, const Vector3& origin)
        {
            for (uint16_t y = 0; y < frame_drawer->GetFrameBufferHeight(); y++)
            {
                for (uint16_t x = 0; x < frame_drawer->GetFrameBufferWidth(); x++)
                {
                    float ndc_x = -1.0f + 2.0f * static_cast<float>(x) / static_cast<float>(frame_drawer->GetFrameBufferHeight() - 1);
                    float ndc_y = 1.0f - 2.0f * static_cast<float>(y) / static_cast<float>(frame_drawer->GetFrameBufferHeight() - 1);

                    Vector3 pixel_point = Vector3(ndc_x, ndc_y, 1.0f);
                    // hack to do the 4d matrix mul and perspective divide without going for a concrete Vector4 class
                    Vertex vert = Vertex(pixel_point);
                    // ndc to view
                    // pixel_point *= proj_matrix_inv;
                    vert *= inverse_projection_mat;

                    // view to world
                    // pixel_point *= view_matrix_inv;
                    vert *= inverse_view_mat;
                    vert.PerspectiveDivide();

                    Ray ray = Ray(origin, origin.GetDirectionalTo(vert.GetPosition()));

                    MarchResult march_res = MarchUntilHit(ray, voxel_grid, 1000);

                    if (!march_res.did_hit)
                        continue;

                    Vector3 world_hit_point = march_res.hit_position;
                    world_hit_point *= view_mat;

                    depthbuffer.SetValue(x, y, world_hit_point.z);
                }
            }
        }

        void RayMarcher::DrawVoxelGridCamera(DepthBuffer& depthbuffer, const VoxelGrid& voxel_grid, const Vector3& origin, const LightingSystem& lighting_system)
        {
            for (uint16_t y = 0; y < frame_drawer->GetFrameBufferHeight(); y++)
            {
                for (uint16_t x = 0; x < frame_drawer->GetFrameBufferWidth(); x++)
                {
                    float ndc_x = -1.0f + 2.0f * static_cast<float>(x) / static_cast<float>(frame_drawer->GetFrameBufferHeight() - 1);
                    float ndc_y = 1.0f - 2.0f * static_cast<float>(y) / static_cast<float>(frame_drawer->GetFrameBufferHeight() - 1);

                    Vector3 pixel_point = Vector3(ndc_x, ndc_y, 1.0f);
                    // hack to do the 4d matrix mul and perspective divide without going for a concrete Vector4 class
                    Vertex vert = Vertex(pixel_point);
                    // ndc to view
                    // pixel_point *= proj_matrix_inv;
                    vert *= inverse_projection_mat;

                    // view to world
                    // pixel_point *= view_matrix_inv;
                    vert *= inverse_view_mat;
                    vert.PerspectiveDivide();

                    Ray ray = Ray(origin, origin.GetDirectionalTo(vert.GetPosition()));

                    MarchResult march_res = MarchUntilHit(ray, voxel_grid, 1000);

                    if (!march_res.did_hit)
                        continue;

                    Vector3 world_hit_point = march_res.hit_position;
                    world_hit_point *= view_mat;
                    depthbuffer.SetValue(x, y, world_hit_point.z);

                    RGBColor voxel_color = voxel_color_map[march_res.voxel_data_ptr->type][march_res.voxel_data_ptr->color_index];
                    RGBColor lit_color   = lighting_system.GetLitColorAt(march_res.hit_position, march_res.hit_normal, origin, MaterialProperties());
                    lit_color += lighting_system.GetAmbientLightColor();
                    voxel_color.BlendMultiply(lit_color);

                    frame_drawer->SetPixel(x, y, voxel_color);
                }
            }
        }

        void RayMarcher::SetViewMatrix(const Matrix4& view_matrix)
        {
            view_mat         = view_matrix;
            inverse_view_mat = view_mat.GetInverted();
        }

        void RayMarcher::SetProjectionMatrix(const Matrix4& projection_matrix)
        {
            projection_mat         = projection_matrix;
            inverse_projection_mat = projection_mat.GetInverted();
        }

    }
}
