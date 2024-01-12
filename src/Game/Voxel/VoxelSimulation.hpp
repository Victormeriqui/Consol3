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

            constexpr uint64_t update_frequency = 3;

            static void UpdateSimulationTopDown(std::shared_ptr<VoxelGrid> voxel_grid, uint64_t update_tick)
            {
                if (update_tick % update_frequency != 0)
                    return;

                for (int y = VOXEL_GRID_UP; y > VOXEL_GRID_DOWN; y--)
                {
                    for (int x = VOXEL_GRID_LEFT; x < VOXEL_GRID_RIGHT; x++)
                    {
                        for (int z = VOXEL_GRID_BACKWARDS; z < VOXEL_GRID_FORWARDS; z++)
                        {
                            Vector3I cur_voxel_pos                  = Vector3I(x, y, z);
                            VoxelData cur_voxel_data                = voxel_grid->GetVoxelData(cur_voxel_pos);
                            VoxelElementSettings cur_voxel_settings = voxel_elements_map[cur_voxel_data.type];

                            // non simulated types like air
                            if (cur_voxel_settings.skip_simulation)
                                continue;

                            // only gas is handled top down
                            if (cur_voxel_settings.movement_type != VoxelMovementType::GAS)
                                continue;

                            // move up, sideways or sideways up
                            std::vector<Vector3I> rand_swap_gases = VoxelUtil::up_sides_and_sides_up;
                            std::shuffle(rand_swap_gases.begin(), rand_swap_gases.end(), VoxelUtil::random_generator);    // randomize the order so theres no spreading pattern

                            for (const Vector3I& move_option : rand_swap_gases)
                            {
                                Vector3I move_pos = cur_voxel_pos + move_option;

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
                if (update_tick % update_frequency != 0)
                    return;

                for (int y = VOXEL_GRID_DOWN; y < VOXEL_GRID_UP; y++)
                {
                    for (int x = VOXEL_GRID_LEFT; x < VOXEL_GRID_RIGHT; x++)
                    {
                        for (int z = VOXEL_GRID_BACKWARDS; z < VOXEL_GRID_FORWARDS; z++)
                        {
                            Vector3I cur_voxel_pos                  = Vector3I(x, y, z);
                            VoxelData cur_voxel_data                = voxel_grid->GetVoxelData(cur_voxel_pos);
                            VoxelElementSettings cur_voxel_settings = voxel_elements_map[cur_voxel_data.type];

                            // non simulated types like air
                            if (cur_voxel_settings.skip_simulation)
                                continue;

                            // static types never move, and gas types are not simulated down top
                            if (cur_voxel_settings.movement_type == VoxelMovementType::STATIC || cur_voxel_settings.movement_type == VoxelMovementType::GAS)
                                continue;

                            // potential swap voxel, down in the case of liquids and solids
                            Vector3I swap_voxel_pos = cur_voxel_pos + VoxelUtil::down;

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
                            std::vector<Vector3I> rand_swap_sides = VoxelUtil::sides_down;
                            std::shuffle(rand_swap_sides.begin(), rand_swap_sides.end(), VoxelUtil::random_generator);    // randomize the order so theres no spreading pattern

                            bool moved_down_side = false;
                            for (const Vector3I& side_option : rand_swap_sides)
                            {
                                swap_voxel_pos = cur_voxel_pos + side_option;

                                if (!voxel_grid->IsPositionInsideGrid(swap_voxel_pos))
                                    continue;

                                VoxelElement side_voxel_element    = voxel_grid->GetVoxelElement(swap_voxel_pos);
                                VoxelElement side_voxel_element_up = voxel_grid->GetVoxelElement(swap_voxel_pos - VoxelUtil::up);

                                // only move diagonally down if the above voxel on the side is also clear, otherwise we'd teleport diagonally
                                if (side_voxel_element == VoxelElement::AIR && side_voxel_element_up == VoxelElement::AIR)
                                {
                                    VoxelUtil::SwapVoxels(voxel_grid, cur_voxel_pos, swap_voxel_pos);
                                    moved_down_side = true;
                                    break;
                                }
                            }
                            if (moved_down_side)
                                continue;

                            // only liquids move sideways
                            if (cur_voxel_settings.movement_type != VoxelMovementType::LIQUID)
                                continue;
                        }
                    }
                }
            }
        }
    }
}

#endif