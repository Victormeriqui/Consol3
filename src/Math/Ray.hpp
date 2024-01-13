#ifndef RAY_HPP
#define RAY_HPP

#include "../Engine/VoxelGrid.hpp"
#include "Vector3.hpp"
#include "Vector3I.hpp"

#include <cstdint>

namespace Math
{
    using namespace Engine;

    struct MarchResult
    {
        bool did_hit;
        Vector3 hit_position;
        Vector3 hit_normal;
        Vector3I hit_voxel_coord;
        const VoxelData* voxel_data_ptr;
    };

    class Ray
    {
    private:
        constexpr Vector3I CalculateNearestVoxelGridCoords(const Vector3I& cur_grid_coords) const
        {
            return Vector3I(cur_grid_coords.x + (direction.x > 0.0f ? 1 : 0), cur_grid_coords.y + (direction.y > 0.0f ? 1 : 0), cur_grid_coords.z + (direction.z > 0.0f ? 1 : 0));
        }

        constexpr Vector3 CalculateTMax(const Vector3I& near_grid_coords) const
        {
            // grid coords to world coords
            Vector3 tmax = near_grid_coords;

            tmax -= origin;
            tmax /= direction;
            return tmax;
        }

        constexpr Vector3 CalculateDelta() const
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

    public:
        Vector3 origin;
        Vector3 direction;

        constexpr Ray(const Vector3& origin, const Vector3& direction) : origin(origin), direction(direction)
        {
        }

        constexpr Vector3 March(float step) const noexcept
        {
            return origin + direction * step;
        }

        MarchResult MarchUntilHit(const VoxelGrid& voxel_grid, uint16_t max_iterations, float max_dist) const
        {
            MarchResult res = { .did_hit = false, .hit_position = Vector3(), .hit_normal = Vector3(), .hit_voxel_coord = Vector3I(), .voxel_data_ptr = nullptr };

            Vector3I step             = direction.GetSignVector();
            Vector3I cur_grid_coords  = voxel_grid.GetGridPosition(origin);
            Vector3I near_grid_coords = CalculateNearestVoxelGridCoords(cur_grid_coords);
            Vector3 t_max             = CalculateTMax(near_grid_coords);
            Vector3 delta             = CalculateDelta();

            uint16_t i = 0;
            while (i++ < max_iterations)
            {
                // ray shot off the grid
                if (!voxel_grid.IsPositionInsideGrid(cur_grid_coords))
                    return res;

                res.voxel_data_ptr = voxel_grid.GetVoxelDataPtr(cur_grid_coords);

                if (res.voxel_data_ptr->type != VoxelElement::AIR)
                {
                    res.did_hit = true;
                    return res;
                }

                if (t_max.x < t_max.y && t_max.x < t_max.z)
                {
                    res.hit_normal      = Vector3(static_cast<float>(-step.x), 0.0f, 0.0f);
                    res.hit_position    = origin + direction * t_max.x;
                    res.hit_voxel_coord = cur_grid_coords;

                    cur_grid_coords.x += step.x;
                    t_max.x += delta.x;
                }
                else if (t_max.y < t_max.z)
                {
                    res.hit_normal      = Vector3(0.0f, static_cast<float>(-step.y), 0.0f);
                    res.hit_position    = origin + direction * t_max.y;
                    res.hit_voxel_coord = cur_grid_coords;

                    cur_grid_coords.y += step.y;
                    t_max.y += delta.y;
                }
                else
                {
                    res.hit_normal      = Vector3(0.0f, 0.0f, static_cast<float>(-step.z));
                    res.hit_position    = origin + direction * t_max.z;
                    res.hit_voxel_coord = cur_grid_coords;

                    cur_grid_coords.z += step.z;
                    t_max.z += delta.z;
                }

                // ray hit max dist
                if (origin.GetDistanceTo(res.hit_position) >= max_dist)
                    return res;
            }

            return res;
        }
    };
}

#endif
