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

    struct MarchState
    {
        Vector3I step;
        Vector3 delta;
        Vector3 tmax;
    };

    class Ray
    {
    private:
        MarchState GetMarchingPreCompute() const
        {
            MarchState res;

            float current_x_index = std::max(1.0f, std::ceil(origin.x - VOXEL_GRID_LEFT));
            float current_y_index = std::max(1.0f, std::ceil(origin.y - VOXEL_GRID_DOWN));
            float current_z_index = std::max(1.0f, std::ceil(origin.z - VOXEL_GRID_BACKWARDS));

            if (direction.x > 0.0f)
            {
                res.step.x  = 1;
                res.delta.x = VOXEL_SIZE / direction.x;
                res.tmax.x  = (VOXEL_GRID_LEFT + current_x_index - origin.x) / direction.x;
            }
            else if (direction.x < 0.0f)
            {
                res.step.x             = -1;
                res.delta.x            = VOXEL_SIZE / -direction.x;
                float previous_x_index = current_x_index - 1;
                res.tmax.x             = (VOXEL_GRID_LEFT + previous_x_index - origin.x) / direction.x;
            }
            else
            {
                res.step.x  = 0;
                res.delta.x = std::numeric_limits<float>::infinity();
                res.tmax.x  = std::numeric_limits<float>::infinity();
            }

            if (direction.y > 0.0f)
            {
                res.step.y  = 1;
                res.delta.y = VOXEL_SIZE / direction.y;
                res.tmax.y  = (VOXEL_GRID_LEFT + current_y_index - origin.y) / direction.y;
            }
            else if (direction.y < 0.0f)
            {
                res.step.y             = -1;
                res.delta.y            = VOXEL_SIZE / -direction.y;
                float previous_y_index = current_y_index - 1;
                res.tmax.y             = (VOXEL_GRID_LEFT + previous_y_index - origin.y) / direction.y;
            }
            else
            {
                res.step.y  = 0;
                res.delta.y = std::numeric_limits<float>::infinity();
                res.tmax.y  = std::numeric_limits<float>::infinity();
            }

            if (direction.z > 0.0f)
            {
                res.step.z  = 1;
                res.delta.z = VOXEL_SIZE / direction.z;
                res.tmax.z  = (VOXEL_GRID_LEFT + current_z_index - origin.z) / direction.z;
            }
            else if (direction.z < 0.0f)
            {
                res.step.z             = -1;
                res.delta.z            = VOXEL_SIZE / -direction.z;
                float previous_z_index = current_z_index - 1;
                res.tmax.z             = (VOXEL_GRID_LEFT + previous_z_index - origin.z) / direction.z;
            }
            else
            {
                res.step.z  = 0;
                res.delta.z = std::numeric_limits<float>::infinity();
                res.tmax.z  = std::numeric_limits<float>::infinity();
            }

            return res;
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

        MarchResult MarchUntilHit(const VoxelGrid& voxel_grid, uint16_t max_iterations) const
        {
            MarchResult res = { .did_hit = false, .hit_position = Vector3(), .hit_normal = Vector3(), .hit_voxel_coord = Vector3I(), .voxel_data_ptr = nullptr };

            MarchState march_state   = GetMarchingPreCompute();
            Vector3I cur_grid_coords = voxel_grid.GetGridPosition(origin);

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

                if (march_state.tmax.x < march_state.tmax.y && march_state.tmax.x < march_state.tmax.z)
                {
                    cur_grid_coords.x += march_state.step.x;

                    res.hit_normal      = Vector3(static_cast<float>(-march_state.step.x), 0.0f, 0.0f);
                    res.hit_position    = origin + direction * march_state.tmax.x;
                    res.hit_voxel_coord = cur_grid_coords;

                    march_state.tmax.x += march_state.delta.x;
                }
                else if (march_state.tmax.y < march_state.tmax.z)
                {
                    cur_grid_coords.y += march_state.step.y;

                    res.hit_normal      = Vector3(0.0f, static_cast<float>(-march_state.step.y), 0.0f);
                    res.hit_position    = origin + direction * march_state.tmax.y;
                    res.hit_voxel_coord = cur_grid_coords;

                    march_state.tmax.y += march_state.delta.y;
                }
                else
                {
                    cur_grid_coords.z += march_state.step.z;

                    res.hit_normal      = Vector3(0.0f, 0.0f, static_cast<float>(-march_state.step.z));
                    res.hit_position    = origin + direction * march_state.tmax.z;
                    res.hit_voxel_coord = cur_grid_coords;

                    march_state.tmax.z += march_state.delta.z;
                }
            }

            return res;
        }

        // used for non rendering marching, more accurate than the other method due to proper handling of cases where the direciton is diagonal
        // this method is slower than the one used for rendering
        MarchResult MarchUntilHitOrPosition(const VoxelGrid& voxel_grid, uint16_t max_iterations, const Vector3I& stop_position) const
        {
            MarchResult res = { .did_hit = false, .hit_position = Vector3(), .hit_normal = Vector3(), .hit_voxel_coord = Vector3I(), .voxel_data_ptr = nullptr };

            MarchState march_state = GetMarchingPreCompute();

            Vector3I cur_grid_coords = voxel_grid.GetGridPosition(origin);

            uint16_t i        = 0;
            int last_smallest = -1;

            while (i++ < max_iterations)
            {
                // ray shot off the grid
                if (!voxel_grid.IsPositionInsideGrid(cur_grid_coords))
                    return res;

                res.voxel_data_ptr = voxel_grid.GetVoxelDataPtr(cur_grid_coords);

                if (res.voxel_data_ptr->type != VoxelElement::AIR && origin != cur_grid_coords)
                {
                    res.did_hit = true;
                    return res;
                }

                bool x_smallest = march_state.tmax.x < march_state.tmax.y && march_state.tmax.x < march_state.tmax.z;
                bool x_equal    = march_state.tmax.x == march_state.tmax.y && march_state.tmax.x < march_state.tmax.z || march_state.tmax.x == march_state.tmax.z && march_state.tmax.x < march_state.tmax.y;
                bool y_smallest = march_state.tmax.y < march_state.tmax.x && march_state.tmax.y < march_state.tmax.z;
                bool y_equal    = march_state.tmax.y == march_state.tmax.x && march_state.tmax.y < march_state.tmax.z || march_state.tmax.y == march_state.tmax.z && march_state.tmax.y < march_state.tmax.x;
                bool z_smallest = march_state.tmax.z < march_state.tmax.x && march_state.tmax.z < march_state.tmax.y;
                bool z_equal    = march_state.tmax.z == march_state.tmax.x && march_state.tmax.z < march_state.tmax.y || march_state.tmax.z == march_state.tmax.y && march_state.tmax.z < march_state.tmax.x;

                if (x_smallest || (x_equal && last_smallest != 0))
                {
                    cur_grid_coords.x += march_state.step.x;

                    res.hit_normal      = Vector3(static_cast<float>(-march_state.step.x), 0.0f, 0.0f);
                    res.hit_position    = origin + direction * march_state.tmax.x;
                    res.hit_voxel_coord = cur_grid_coords;
                    last_smallest       = 0;

                    march_state.tmax.x += march_state.delta.x;
                }
                else if (y_smallest || (y_equal && last_smallest != 1))
                {
                    cur_grid_coords.y += march_state.step.y;

                    res.hit_normal      = Vector3(0.0f, static_cast<float>(-march_state.step.y), 0.0f);
                    res.hit_position    = origin + direction * march_state.tmax.y;
                    res.hit_voxel_coord = cur_grid_coords;
                    last_smallest       = 1;

                    march_state.tmax.y += march_state.delta.y;
                }
                else if (z_smallest || (z_equal && last_smallest != 2))
                {
                    cur_grid_coords.z += march_state.step.z;

                    res.hit_normal      = Vector3(0.0f, 0.0f, static_cast<float>(-march_state.step.z));
                    res.hit_position    = origin + direction * march_state.tmax.z;
                    res.hit_voxel_coord = cur_grid_coords;
                    last_smallest       = 2;

                    march_state.tmax.z += march_state.delta.z;
                }

                if (cur_grid_coords == stop_position)
                    return res;
            }

            return res;
        }
    };
}

#endif
