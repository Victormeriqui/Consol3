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

        void RayMarcher::DrawVoxelGridDepthOnlyPerspective(DepthBuffer& depthbuffer, const VoxelGrid& voxel_grid, const Vector3& origin)
        {
            for (uint16_t y = 0; y < frame_drawer->GetFrameBufferHeight(); y++)
            {
                for (uint16_t x = 0; x < frame_drawer->GetFrameBufferWidth(); x++)
                {
                    Ray ray = SetupRayPerspective(x, y, origin);

                    MarchResult march_res = ray.MarchUntilHit(voxel_grid, 1000, 1000.0f);

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

                    MarchResult march_res = ray.MarchUntilHit(voxel_grid, 1000, 1000.0f);

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

                    MarchResult march_res = ray.MarchUntilHit(voxel_grid, 1000, 1000.0f);

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
