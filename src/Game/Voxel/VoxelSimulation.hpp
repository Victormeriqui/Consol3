#ifndef VOXELSIMULATION_HPP
#define VOXELSIMULATION_HPP

#include "Display/RGBColor.hpp"
#include "Engine/VoxelGrid.hpp"
#include "Engine/VoxelTypes.hpp"
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

            static void UpdateSimulation(std::shared_ptr<VoxelGrid> voxel_grid, uint64_t update_tick)
            {
                if (update_tick % 3 != 0)
                    return;

                for (uint16_t y = 0; y < VOXEL_GRID_HEIGHT; y++)
                {
                    for (uint16_t x = 0; x < VOXEL_GRID_WIDTH; x++)
                    {
                        for (uint16_t z = 0; z < VOXEL_GRID_DEPTH; z++)
                        {
                            Vector3 cur_voxel_pos    = Vector3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
                            VoxelData cur_voxel_data = voxel_grid->GetVoxelData(x, y, z);

                            if (cur_voxel_data.type == VoxelType::AIR)
                                continue;

                            bool is_liquid = voxel_liquid_types.find(cur_voxel_data.type) != voxel_liquid_types.end();
                            bool is_solid  = voxel_solid_types.find(cur_voxel_data.type) != voxel_solid_types.end();

                            // solids never move
                            if (is_solid)
                                continue;

                            // move down
                            if (voxel_grid->IsPositionInsideGrid(cur_voxel_pos - VoxelUtil::down))
                            {
                                VoxelType down_voxel_type = voxel_grid->GetVoxelType(cur_voxel_pos - VoxelUtil::down);

                                if (down_voxel_type == VoxelType::AIR)
                                {
                                    voxel_grid->SetVoxelData(x, y, z, { VoxelType::AIR });
                                    voxel_grid->SetVoxelData(cur_voxel_pos - VoxelUtil::down, cur_voxel_data);
                                    continue;
                                }
                            }

                            // move sideways down
                            std::array<Vector3, 4> rand_sides_down = VoxelUtil::sides_down;
                            std::shuffle(rand_sides_down.begin(), rand_sides_down.end(), VoxelUtil::random_generator);    // randomize the order so theres no spreading pattern

                            bool moved_down_side = false;
                            for (const Vector3& side_option : rand_sides_down)
                            {
                                Vector3 side_pos = cur_voxel_pos + side_option;

                                if (!voxel_grid->IsPositionInsideGrid(side_pos))
                                    continue;

                                VoxelType side_voxel_type = voxel_grid->GetVoxelType(side_pos);

                                if (side_voxel_type == VoxelType::AIR)
                                {
                                    voxel_grid->SetVoxelData(x, y, z, { VoxelType::AIR });
                                    voxel_grid->SetVoxelData(side_pos, cur_voxel_data);
                                    moved_down_side = true;
                                    break;
                                }
                            }
                            if (moved_down_side)
                                continue;

                            // move sideways
                            if (!is_liquid)
                                continue;

                            std::array<Vector3, 4> rand_sides = VoxelUtil::sides;
                            std::shuffle(rand_sides.begin(), rand_sides.end(), VoxelUtil::random_generator);    // randomize the order so theres no spreading pattern

                            for (const Vector3& side_option : rand_sides)
                            {
                                Vector3 side_pos = cur_voxel_pos + side_option;

                                if (!voxel_grid->IsPositionInsideGrid(side_pos))
                                    continue;

                                VoxelData side_voxel_data = voxel_grid->GetVoxelData(side_pos);

                                if (side_voxel_data.type == VoxelType::AIR || side_voxel_data.type == VoxelType::WATER)
                                {
                                    voxel_grid->SetVoxelData(x, y, z, side_voxel_data);
                                    voxel_grid->SetVoxelData(side_pos, cur_voxel_data);
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