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

        Ray RayMarcher::SetupRayPerspective(uint16_t x, uint16_t y, const Vector3& origin) const
        {
            float ndc_x = -1.0f + 2.0f * static_cast<float>(x) / static_cast<float>(frame_drawer->GetFrameBufferHeight() - 1);
            float ndc_y = 1.0f - 2.0f * static_cast<float>(y) / static_cast<float>(frame_drawer->GetFrameBufferHeight() - 1);

            Vector3 pixel_point = Vector3(ndc_x, ndc_y, 1.0f);
            // hack to do the 4d matrix mul and perspective divide (keeping the W component) without going for a concrete Vector4 class
            Vertex vert = Vertex(pixel_point);

            // remove the projection matrix, get view coords
            vert *= inverse_projection_mat;

            // remove the view matrix, get world coords
            vert *= inverse_view_mat;
            vert.PerspectiveDivide();

            return Ray(origin, origin.GetDirectionalTo(vert.GetPosition()));
        }

        Ray RayMarcher::SetupRayOrtho(uint16_t x, uint16_t y) const
        {
            float ndc_x = -1.0f + 2.0f * static_cast<float>(x) / static_cast<float>(frame_drawer->GetFrameBufferHeight() - 1);
            float ndc_y = 1.0f - 2.0f * static_cast<float>(y) / static_cast<float>(frame_drawer->GetFrameBufferHeight() - 1);

            Vector3 pixel_point      = Vector3(ndc_x, ndc_y, 1.0f);
            Vector3 pixel_point_back = Vector3(ndc_x, ndc_y, 0.0f);

            // ndc to view
            pixel_point *= inverse_projection_mat;
            pixel_point_back *= inverse_projection_mat;

            // view to world
            pixel_point *= inverse_view_mat;
            pixel_point_back *= inverse_view_mat;

            return Ray(pixel_point_back, pixel_point_back.GetDirectionalTo(pixel_point));
        }

        Vector3 RayMarcher::HitPositionToNDCPerspective(const Vector3& hit_pos) const
        {
            // hack to do the 4d matrix mul and perspective divide (keeping the W component) without going for a concrete Vector4 class
            Vertex vert = Vertex(hit_pos);
            vert *= view_mat;
            vert *= projection_mat;
            vert.PerspectiveDivide();

            return vert.GetPosition();
        }

        Vector3 RayMarcher::HitPositionToNDCOrtho(Vector3 hit_pos) const
        {
            hit_pos *= view_mat;
            hit_pos *= projection_mat;

            return hit_pos;
        }

        void RayMarcher::SetupLightSpacePerspective(const Vector3& hit_pos, const LightingSystem& lighting_system)
        {
            uint8_t hit_ndc_light_space_c = 0;

            for (std::shared_ptr<ILight> light : lighting_system.GetLights())
            {
                if (!light->IsShadowCaster())
                    continue;

                // hack to do the 4d matrix mul and perspective divide (keeping the W component) without going for a concrete Vector4 class
                Vertex vert = Vertex(hit_pos);
                vert *= light->GetViewMatrix().value().get();
                vert *= light->GetProjectionMatrix().value().get();
                vert.PerspectiveDivide();

                hit_ndc_light_space[hit_ndc_light_space_c++] = vert.GetPosition();
            }
        }

        void RayMarcher::SetupLightSpaceOrtho(const Vector3& hit_pos, const LightingSystem& lighting_system)
        {
            uint8_t hit_ndc_light_space_c = 0;

            for (std::shared_ptr<ILight> light : lighting_system.GetLights())
            {
                if (!light->IsShadowCaster())
                    continue;

                // hack to do the 4d matrix mul and perspective divide (keeping the W component) without going for a concrete Vector4 class
                Vector3 hit_in_light_space = hit_pos;
                hit_in_light_space *= light->GetViewMatrix().value().get();
                hit_in_light_space *= light->GetProjectionMatrix().value().get();

                hit_ndc_light_space[hit_ndc_light_space_c++] = hit_in_light_space;
            }
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
                    res.hit_normal   = Vector3(static_cast<float>(-step.x), 0.0f, 0.0f);
                    res.hit_position = ray.origin + ray.direction * t_max.x;

                    cur_grid_coords.x += step.x;
                    t_max.x += delta.x;
                }
                else if (t_max.y < t_max.z)
                {
                    res.hit_normal   = Vector3(0.0f, static_cast<float>(-step.y), 0.0f);
                    res.hit_position = ray.origin + ray.direction * t_max.y;

                    cur_grid_coords.y += step.y;
                    t_max.y += delta.y;
                }
                else
                {
                    res.hit_normal   = Vector3(0.0f, 0.0f, static_cast<float>(-step.z));
                    res.hit_position = ray.origin + ray.direction * t_max.z;

                    cur_grid_coords.z += step.z;
                    t_max.z += delta.z;
                }
            }

            return res;
        }

        void RayMarcher::DrawVoxelGridDepthOnlyPerspective(DepthBuffer& depthbuffer, const VoxelGrid& voxel_grid, const Vector3& origin)
        {
            for (uint16_t y = 0; y < frame_drawer->GetFrameBufferHeight(); y++)
            {
                for (uint16_t x = 0; x < frame_drawer->GetFrameBufferWidth(); x++)
                {
                    Ray ray = SetupRayPerspective(x, y, origin);

                    MarchResult march_res = MarchUntilHit(ray, voxel_grid, 1000);

                    if (!march_res.did_hit)
                        continue;

                    Vector3 hit_ndc = HitPositionToNDCPerspective(march_res.hit_position);

                    if (depthbuffer.GetValue(x, y) > hit_ndc.z)
                        depthbuffer.SetValue(x, y, hit_ndc.z);
                }
            }
        }

        void RayMarcher::DrawVoxelGridDepthOnlyOrtho(DepthBuffer& depthbuffer, const VoxelGrid& voxel_grid)
        {
            for (uint16_t y = 0; y < frame_drawer->GetFrameBufferHeight(); y++)
            {
                for (uint16_t x = 0; x < frame_drawer->GetFrameBufferWidth(); x++)
                {
                    Ray ray = SetupRayOrtho(x, y);

                    MarchResult march_res = MarchUntilHit(ray, voxel_grid, 1000);

                    if (!march_res.did_hit)
                        continue;

                    Vector3 hit_ndc = HitPositionToNDCOrtho(march_res.hit_position);

                    if (depthbuffer.GetValue(x, y) > hit_ndc.z)
                        depthbuffer.SetValue(x, y, hit_ndc.z);
                }
            }
        }

        void RayMarcher::DrawVoxelGridPerspective(DepthBuffer& depthbuffer, const VoxelGrid& voxel_grid, const Vector3& origin, const LightingSystem& lighting_system)
        {
            for (uint16_t y = 0; y < frame_drawer->GetFrameBufferHeight(); y++)
            {
                for (uint16_t x = 0; x < frame_drawer->GetFrameBufferWidth(); x++)
                {
                    Ray ray = SetupRayPerspective(x, y, origin);

                    MarchResult march_res = MarchUntilHit(ray, voxel_grid, 1000);

                    if (!march_res.did_hit)
                        continue;

                    Vector3 hit_ndc = HitPositionToNDCPerspective(march_res.hit_position);

                    if (depthbuffer.GetValue(x, y) < hit_ndc.z)
                        continue;

                    depthbuffer.SetValue(x, y, hit_ndc.z);

                    // base voxel color
                    RGBColor voxel_color = voxel_color_map[march_res.voxel_data_ptr->type][march_res.voxel_data_ptr->color_index];

                    SetupLightSpacePerspective(march_res.hit_position, lighting_system);

                    RGBColor lit_color = lighting_system.GetLitColorAt(march_res.hit_position, march_res.hit_normal, origin, hit_ndc_light_space, MaterialProperties());

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

        void RayMarcher::DrawPixel(uint16_t x, uint16_t y, const RGBColor& color)
        {
            frame_drawer->SetPixel(x, y, color);
        }

    }
}
