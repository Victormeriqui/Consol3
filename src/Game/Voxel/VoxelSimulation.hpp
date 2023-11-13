#ifndef VOXELSIMULATION_HPP
#define VOXELSIMULATION_HPP

#include "Display/RGBColor.hpp"
#include "Engine/VoxelElements.hpp"
#include "Engine/VoxelGrid.hpp"
#include "Math/Vector3.hpp"
#include "VoxelUtil.hpp"

#include <cstdint>
#include <memory>

namespace Game
{
    namespace Voxel
    {
        namespace VoxelSimulation
        {
            static void UpdateSimulationTopDown(std::shared_ptr<VoxelGrid> voxel_grid, uint64_t update_tick)
            {
                if (update_tick % 3 != 0)
                    return;

                for (uint16_t y = VOXEL_GRID_HEIGHT - 1; y > 0; y--)
                {
                    for (uint16_t x = 0; x < VOXEL_GRID_WIDTH; x++)
                    {
                        for (uint16_t z = 0; z < VOXEL_GRID_DEPTH; z++)
                        {
                            Vector3 cur_voxel_pos                   = Vector3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
                            VoxelData cur_voxel_data                = voxel_grid->GetVoxelData(x, y, z);
                            VoxelElementSettings cur_voxel_settings = voxel_elements_map[cur_voxel_data.type];

                            // non simulated types like air
                            if (cur_voxel_settings.skip_simulation)
                                continue;

                            // only gas is handled top down
                            if (cur_voxel_settings.movement_type != VoxelMovementType::GAS)
                                continue;

                            // move up, sideways or sideways up
                            std::array<Vector3, 9> rand_swap_gases = VoxelUtil::up_sides_and_sides_up;
                            std::shuffle(rand_swap_gases.begin(), rand_swap_gases.end(), VoxelUtil::random_generator);    // randomize the order so theres no spreading pattern

                            for (const Vector3& move_option : rand_swap_gases)
                            {
                                Vector3 move_pos = cur_voxel_pos + move_option;

                                if (!voxel_grid->IsPositionInsideGrid(move_pos))
                                    continue;

                                VoxelElement side_voxel_element = voxel_grid->GetVoxelElement(move_pos);

                                if (side_voxel_element == VoxelElement::AIR)
                                {
                                    VoxelUtil::SwapVoxels(voxel_grid, cur_voxel_pos, move_pos);
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            static void UpdateSimulationDownTop(std::shared_ptr<VoxelGrid> voxel_grid, uint64_t update_tick)
            {
                if (update_tick % 3 != 0)
                    return;

                for (uint16_t y = 0; y < VOXEL_GRID_HEIGHT; y++)
                {
                    for (uint16_t x = 0; x < VOXEL_GRID_WIDTH; x++)
                    {
                        for (uint16_t z = 0; z < VOXEL_GRID_DEPTH; z++)
                        {
                            Vector3 cur_voxel_pos                   = Vector3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
                            VoxelData cur_voxel_data                = voxel_grid->GetVoxelData(x, y, z);
                            VoxelElementSettings cur_voxel_settings = voxel_elements_map[cur_voxel_data.type];

                            // non simulated types like air
                            if (cur_voxel_settings.skip_simulation)
                                continue;

                            // static types never move, and gas types are not simulated down top
                            if (cur_voxel_settings.movement_type == VoxelMovementType::STATIC || cur_voxel_settings.movement_type == VoxelMovementType::GAS)
                                continue;

                            // potential swap voxel, down in the case of liquids and solids
                            Vector3 swap_voxel_pos = cur_voxel_pos - VoxelUtil::down;

                            if (voxel_grid->IsPositionInsideGrid(swap_voxel_pos))
                            {
                                VoxelElement swap_voxel_element = voxel_grid->GetVoxelElement(swap_voxel_pos);

                                if (swap_voxel_element == VoxelElement::AIR)
                                {
                                    VoxelUtil::SwapVoxels(voxel_grid, cur_voxel_pos, swap_voxel_pos);
                                    continue;
                                }
                            }

                            // move sideways down
                            std::array<Vector3, 4> rand_swap_sides = VoxelUtil::sides_down;
                            std::shuffle(rand_swap_sides.begin(), rand_swap_sides.end(), VoxelUtil::random_generator);    // randomize the order so theres no spreading pattern

                            bool moved_down_side = false;
                            for (const Vector3& side_option : rand_swap_sides)
                            {
                                Vector3 side_pos = cur_voxel_pos + side_option;

                                if (!voxel_grid->IsPositionInsideGrid(side_pos))
                                    continue;

                                VoxelElement side_voxel_element = voxel_grid->GetVoxelElement(side_pos);

                                if (side_voxel_element == VoxelElement::AIR)
                                {
                                    VoxelUtil::SwapVoxels(voxel_grid, cur_voxel_pos, side_pos);
                                    moved_down_side = true;
                                    break;
                                }
                            }
                            if (moved_down_side)
                                continue;

                            // only liquids move sideways
                            if (cur_voxel_settings.movement_type != VoxelMovementType::LIQUID)
                                continue;

                            std::array<Vector3, 4> rand_sides = VoxelUtil::sides;
                            std::shuffle(rand_sides.begin(), rand_sides.end(), VoxelUtil::random_generator);    // randomize the order so theres no spreading pattern

                            for (const Vector3& side_option : rand_sides)
                            {
                                Vector3 side_pos = cur_voxel_pos + side_option;

                                if (!voxel_grid->IsPositionInsideGrid(side_pos))
                                    continue;

                                VoxelElement side_voxel_element = voxel_grid->GetVoxelElement(side_pos);

                                if (side_voxel_element == VoxelElement::AIR)
                                {
                                    VoxelUtil::SwapVoxels(voxel_grid, cur_voxel_pos, side_pos);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

#endif