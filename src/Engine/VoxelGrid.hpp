#ifndef VOXELGRID_HPP
#define VOXELGRID_HPP

#include "Math/Util/MathUtil.hpp"
#include "VoxelTypes.hpp"

#include <array>
#include <cstdint>

#define VOXEL_GRID_WIDTH  100
#define VOXEL_GRID_HEIGHT 100
#define VOXEL_GRID_DEPTH  100

using namespace Math;
using namespace Math::Util;

namespace Engine
{
    struct VoxelData
    {
        VoxelType type;
        RGBColor color;

        VoxelData() : type(VoxelType::AIR)
        {
        }
        VoxelData(VoxelType type) : type(type)
        {
        }
        VoxelData(VoxelType type, const RGBColor& color) : type(type), color(color)
        {
        }
    };

    template<uint16_t WIDTH, uint16_t HEIGHT, uint16_t DEPTH>
    class VoxelGrid
    {
    private:
        std::array<VoxelData, WIDTH * HEIGHT * DEPTH> grid;

    public:
        VoxelGrid()
        {
            grid.fill({ VoxelType::AIR });
        }

        [[nodiscard]] VoxelData GetVoxelData(uint16_t x, uint16_t y, uint16_t z) const
        {
            return grid[x + WIDTH * (y + HEIGHT * z)];
        }

        [[nodiscard]] VoxelData GetVoxelData(const Vector3& pos) const
        {
            return grid[static_cast<uint16_t>(pos.x) + WIDTH * (static_cast<uint16_t>(pos.y) + HEIGHT * static_cast<uint16_t>(pos.z))];
        }

        [[nodiscard]] VoxelType GetVoxelType(uint16_t x, uint16_t y, uint16_t z) const
        {
            return grid[x + WIDTH * (y + HEIGHT * z)].type;
        }

        [[nodiscard]] VoxelType GetVoxelType(const Vector3& pos) const
        {
            return grid[static_cast<uint16_t>(pos.x) + WIDTH * (static_cast<uint16_t>(pos.y) + HEIGHT * static_cast<uint16_t>(pos.z))].type;
        }

        void SetVoxelData(uint16_t x, uint16_t y, uint16_t z, VoxelData voxel_data)
        {
            grid[x + WIDTH * (y + HEIGHT * z)] = voxel_data;
        }

        void SetVoxelData(const Vector3& pos, VoxelData voxel_data)
        {
            grid[static_cast<uint16_t>(pos.x) + WIDTH * (static_cast<uint16_t>(pos.y) + HEIGHT * static_cast<uint16_t>(pos.z))] = voxel_data;
        }

        [[nodiscard]] bool IsPositionInsideGrid(uint16_t x, uint16_t y, uint16_t z)
        {
            return IsInRange(x, 0, VOXEL_GRID_WIDTH - 1) && IsInRange(y, 0, VOXEL_GRID_HEIGHT - 1) && IsInRange(z, 0, VOXEL_GRID_DEPTH - 1);
        }

        [[nodiscard]] bool IsPositionInsideGrid(const Vector3& pos)
        {
            return IsPositionInsideGrid(static_cast<uint16_t>(pos.x), static_cast<uint16_t>(pos.y), static_cast<uint16_t>(pos.z));
        }

        void Fill(VoxelData voxel_data)
        {
            grid.fill(voxel_data);
        }
    };

}

#endif