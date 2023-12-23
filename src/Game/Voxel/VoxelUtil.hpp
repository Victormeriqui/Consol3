#ifndef VOXELUTIL_HPP
#define VOXELUTIL_HPP

#include "Display/RGBColor.hpp"
#include "Engine/VoxelElements.hpp"
#include "Engine/VoxelGrid.hpp"
#include "Math/Vector3.hpp"
#include "Math/Vector3I.hpp"

#include <cmath>
#include <cstdint>
#include <memory>
#include <random>

namespace Game
{
    namespace Voxel
    {
        namespace VoxelUtil
        {
            static std::mt19937 random_generator(std::random_device {}());

            static Vector3I left           = Vector3I(-1, 0, 0);
            static Vector3I left_forward   = Vector3I(-1, 0, 1);
            static Vector3I forward        = Vector3I(0, 0, 1);
            static Vector3I right_forward  = Vector3I(1, 0, 1);
            static Vector3I right          = Vector3I(1, 0, 0);
            static Vector3I right_backward = Vector3I(1, 0, -1);
            static Vector3I backward       = Vector3I(0, 0, -1);
            static Vector3I left_backward  = Vector3I(-1, 0, -1);
            static Vector3I up             = Vector3I(0, 1, 0);
            static Vector3I down           = Vector3I(0, -1, 0);

            static std::array<Vector3I, 8> sides      = { left, left_forward, forward, right_forward, right, right_backward, backward, left_backward };
            static std::array<Vector3I, 8> sides_down = { left + down, left_forward + down, forward + down, right_forward + down, right + down, right_backward + down, backward + down, left_backward + down };
            static std::array<Vector3I, 8> sides_up   = { left + up, left_forward + up, forward + up, right_forward + up, right + up, right_backward + up, backward + up, left_backward + up };

            static std::array<Vector3I, 9> up_sides_and_sides_up = { up, left, right, forward, backward, left + up, right + up, forward + up, backward + up };

            static std::vector<std::vector<Vector3I>> sides_at_dist;

            static void InitializeSidesUntilMaxDist(uint8_t max_dist)
            {
                for (uint8_t dist = 0; dist <= max_dist; dist++)
                {
                    std::vector<Vector3I> cur_sides = std::vector<Vector3I>();

                    for (int side_option_z = -dist; side_option_z <= dist; side_option_z++)
                    {
                        for (int side_option_x = -dist; side_option_x <= dist; side_option_x++)
                        {
                            uint8_t cur_dist = static_cast<uint8_t>((static_cast<float>(side_option_z * side_option_z + side_option_x * side_option_x)));

                            if (cur_dist != dist)
                                continue;

                            cur_sides.emplace_back(Vector3I(side_option_x, 0, side_option_z));
                        }
                    }

                    sides_at_dist.emplace_back(cur_sides);
                }
            }

            static void SpawnVoxel(std::shared_ptr<VoxelGrid> voxel_grid, const Vector3I& grid_pos, VoxelElement voxel_type, uint8_t color_index)
            {
                voxel_grid->SetVoxelData(grid_pos, { voxel_type, color_index });
            }

            static void SpawnVoxel(std::shared_ptr<VoxelGrid> voxel_grid, const Vector3I& grid_pos, VoxelElement voxel_type)
            {
                uint8_t random_color_index = static_cast<uint8_t>(random_generator() % voxel_color_map[voxel_type].size());

                SpawnVoxel(voxel_grid, grid_pos, voxel_type, random_color_index);
            }

            static void SwapVoxels(std::shared_ptr<VoxelGrid> voxel_grid, const Vector3I& grid_pos_a, const Vector3I& grid_pos_b)
            {
                VoxelData a_data = voxel_grid->GetVoxelData(grid_pos_a);
                VoxelData b_data = voxel_grid->GetVoxelData(grid_pos_b);

                voxel_grid->SetVoxelData(grid_pos_a, b_data);
                voxel_grid->SetVoxelData(grid_pos_b, a_data);
            }
        }
    }
}

#endif