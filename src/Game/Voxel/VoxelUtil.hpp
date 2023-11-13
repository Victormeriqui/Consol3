#ifndef VOXELUTIL_HPP
#define VOXELUTIL_HPP

#include "Display/RGBColor.hpp"
#include "Engine/VoxelElements.hpp"
#include "Engine/VoxelGrid.hpp"
#include "Math/Vector3.hpp"

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

            static Vector3 left     = Vector3(-1, 0, 0);
            static Vector3 right    = Vector3(1, 0, 0);
            static Vector3 forward  = Vector3(0, 0, 1);
            static Vector3 backward = Vector3(0, 0, -1);
            static Vector3 up       = Vector3(0, -1, 0);
            static Vector3 down     = Vector3(0, 1, 0);

            static std::array<Vector3, 4> sides                 = { left, right, forward, backward };
            static std::array<Vector3, 4> sides_down            = { left - down, right - down, forward - down, backward - down };
            static std::array<Vector3, 4> sides_up              = { left - up, right - up, forward - up, backward - up };
            static std::array<Vector3, 9> up_sides_and_sides_up = { up, left, right, forward, backward, left - up, right - up, forward - up, backward - up };

            static void SpawnVoxel(std::shared_ptr<VoxelGrid> voxel_grid, uint16_t x, uint16_t y, uint16_t z, VoxelElement voxel_type, uint8_t color_index)
            {
                voxel_grid->SetVoxelData(x, y, z, { voxel_type, color_index });
            }

            static void SpawnVoxel(std::shared_ptr<VoxelGrid> voxel_grid, uint16_t x, uint16_t y, uint16_t z, VoxelElement voxel_type)
            {
                uint8_t random_color_index = static_cast<uint8_t>(random_generator() % voxel_color_map[voxel_type].size());
                SpawnVoxel(voxel_grid, x, y, z, voxel_type, random_color_index);
            }

            static void SpawnVoxel(std::shared_ptr<VoxelGrid> voxel_grid, const Vector3& pos, VoxelElement voxel_type)
            {
                SpawnVoxel(voxel_grid, static_cast<uint16_t>(pos.x), static_cast<uint16_t>(pos.y), static_cast<uint16_t>(pos.z), voxel_type);
            }

            static void SwapVoxels(std::shared_ptr<VoxelGrid> voxel_grid, const Vector3& pos_a, const Vector3& pos_b)
            {
                VoxelData a_data = voxel_grid->GetVoxelData(pos_a);
                VoxelData b_data = voxel_grid->GetVoxelData(pos_b);

                voxel_grid->SetVoxelData(pos_a, b_data);
                voxel_grid->SetVoxelData(pos_b, a_data);
            }
        }
    }
}

#endif