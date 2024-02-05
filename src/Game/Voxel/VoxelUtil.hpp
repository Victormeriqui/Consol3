#ifndef VOXELUTIL_HPP
#define VOXELUTIL_HPP

#include "Display/RGBColor.hpp"
#include "Engine/VoxelElements.hpp"
#include "Engine/VoxelGrid.hpp"
#include "Math/Util/MathUtil.hpp"
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
        using namespace Engine;

        namespace VoxelUtil
        {
            static std::mt19937 random_generator(std::random_device {}());
            static std::uniform_real_distribution<float> rand_direction_component {-1.0f, 1.0f};

            static Vector3 GetRandomHorizontalVelocity()
            {
                return Vector3(rand_direction_component(random_generator), 0.0f, rand_direction_component(random_generator));
            }

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

            static std::vector<Vector3I> adjacent_positions;
            static std::vector<std::vector<Vector3I>> sides_at_dist;

            static void InitializeUtilPositions(uint8_t max_dist)
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

                for (int y = -1; y <= 1; y++)
                {
                    for (const Vector3I& pos : sides_at_dist[1])
                        adjacent_positions.emplace_back(Vector3I(pos.x, y, pos.z));
                    for (const Vector3I& pos : sides_at_dist[2])
                        adjacent_positions.emplace_back(Vector3I(pos.x, y, pos.z));
                }
                adjacent_positions.emplace_back(Vector3I(0, 1, 0));
                adjacent_positions.emplace_back(Vector3I(0, -1, 0));
            }

            static void SpawnVoxel(std::shared_ptr<VoxelGrid> voxel_grid, const Vector3I& grid_pos, VoxelElement voxel_type, uint8_t color_index, float temperature)
            {
                VoxelElementSettings element_settings = voxel_element_settings_map[voxel_type];

                Vector3 velocity = Vector3(0.0f, 0.0f, 0.0f);

                // give a downwards head start to solid and liquid spawns
                if (element_settings.movement_type == VoxelMovementType::SOLID || element_settings.movement_type == VoxelMovementType::LIQUID)
                    velocity.y = -1.0f;

                voxel_grid->SetVoxelData(grid_pos, {.type = voxel_type, .color_index = color_index, .velocity = velocity, .temperature = temperature});
            }

            static void SpawnVoxel(std::shared_ptr<VoxelGrid> voxel_grid, const Vector3I& grid_pos, VoxelElement voxel_type)
            {
                VoxelElementSettings element_settings = voxel_element_settings_map[voxel_type];

                uint8_t random_color_index = static_cast<uint8_t>(random_generator() % voxel_color_map[voxel_type].size());

                SpawnVoxel(voxel_grid, grid_pos, voxel_type, random_color_index, element_settings.temperature_settings.first_spawn_temperature);
            }

            static void PhaseTransitionVoxel(std::shared_ptr<VoxelGrid> voxel_grid, const Vector3I& grid_pos, VoxelElement voxel_type, float temperature)
            {
                VoxelElementSettings element_settings = voxel_element_settings_map[voxel_type];

                uint8_t random_color_index = static_cast<uint8_t>(random_generator() % voxel_color_map[voxel_type].size());

                SpawnVoxel(voxel_grid, grid_pos, voxel_type, random_color_index, temperature + element_settings.temperature_settings.phase_transition_temperature_boost);
            }

            static void SwapVoxels(std::shared_ptr<VoxelGrid> voxel_grid, const Vector3I& grid_pos_a, const Vector3I& grid_pos_b)
            {
                VoxelData a_data = voxel_grid->GetVoxelData(grid_pos_a);
                VoxelData b_data = voxel_grid->GetVoxelData(grid_pos_b);

                voxel_grid->SetVoxelData(grid_pos_a, b_data);
                voxel_grid->SetVoxelData(grid_pos_b, a_data);
            }

            static void SpawnBox(std::shared_ptr<VoxelGrid> voxel_grid, const Vector3I& grid_pos, uint8_t size, VoxelElement voxel_type)
            {
                uint8_t half_size = size / 2;
                int min_x         = grid_pos.x - half_size;
                int max_x         = grid_pos.x + half_size;
                int min_y         = grid_pos.y - half_size;
                int max_y         = grid_pos.y + half_size;
                int min_z         = grid_pos.z - half_size;
                int max_z         = grid_pos.z + half_size;

                for (int x = min_x; x <= max_x; x++)
                {
                    for (int y = min_y; y <= max_y; y++)
                    {
                        for (int z = min_z; z <= max_z; z++)
                        {
                            if (x == min_x || x == max_x || y == min_y || z == min_z || z == max_z)
                                SpawnVoxel(voxel_grid, Vector3I(x, y, z), voxel_type);
                        }
                    }
                }
            }

            static void SpawnCube(std::shared_ptr<VoxelGrid> voxel_grid, const Vector3I& grid_pos, uint8_t size, VoxelElement voxel_type)
            {
                uint8_t half_size = size / 2;
                int min_x         = grid_pos.x - half_size;
                int max_x         = grid_pos.x + half_size;
                int min_y         = grid_pos.y - half_size;
                int max_y         = grid_pos.y + half_size;
                int min_z         = grid_pos.z - half_size;
                int max_z         = grid_pos.z + half_size;

                for (int x = min_x; x <= max_x; x++)
                {
                    for (int y = min_y; y <= max_y; y++)
                    {
                        for (int z = min_z; z <= max_z; z++)
                        {
                            SpawnVoxel(voxel_grid, Vector3I(x, y, z), voxel_type);
                        }
                    }
                }
            }
        }
    }
}

#endif