#ifndef VOXELUTIL_HPP
#define VOXELUTIL_HPP

#include "Display/RGBColor.hpp"
#include "Engine/VoxelGrid.hpp"
#include "Engine/VoxelTypes.hpp"
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

            static std::array<Vector3, 4> sides      = { left, right, forward, backward };
            static std::array<Vector3, 4> sides_down = { left - down, right - down, forward - down, backward - down };
            static std::array<Vector3, 4> sides_up   = { left - down, right - down, forward - down, backward - down };

            static void SpawnVoxel(std::shared_ptr<VoxelGrid> voxel_grid, uint16_t x, uint16_t y, uint16_t z, VoxelType voxel_type, RGBColor color)
            {
                voxel_grid->SetVoxelData(x, y, z, { voxel_type, color });
            }

            static void SpawnVoxel(std::shared_ptr<VoxelGrid> voxel_grid, uint16_t x, uint16_t y, uint16_t z, VoxelType voxel_type)
            {
                const std::vector<RGBColor>& color_options = voxel_color_map.at(voxel_type);

                uint8_t random_color = static_cast<uint8_t>(random_generator() % color_options.size());
                SpawnVoxel(voxel_grid, x, y, z, voxel_type, color_options[random_color]);
            }

            static void SpawnVoxel(std::shared_ptr<VoxelGrid> voxel_grid, const Vector3& pos, VoxelType voxel_type)
            {
                SpawnVoxel(voxel_grid, static_cast<uint16_t>(pos.x), static_cast<uint16_t>(pos.y), static_cast<uint16_t>(pos.z), voxel_type);
            }
        }
    }
}

#endif