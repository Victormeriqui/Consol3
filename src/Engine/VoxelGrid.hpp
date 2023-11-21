#ifndef VOXELGRID_HPP
#define VOXELGRID_HPP

#include "Math/Util/MathUtil.hpp"
#include "Vector3.hpp"
#include "Vector3i.hpp"
#include "VoxelElements.hpp"

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
        VoxelElement type;
        uint8_t color_index = 0;

        VoxelData() : type(VoxelElement::AIR)
        {
        }
        VoxelData(VoxelElement type) : type(type)
        {
        }
        VoxelData(VoxelElement type, uint8_t color_index) : type(type), color_index(color_index)
        {
        }
    };

    class VoxelGrid
    {
    private:
        std::array<VoxelData, VOXEL_GRID_WIDTH * VOXEL_GRID_HEIGHT * VOXEL_GRID_DEPTH> grid;

    public:
        VoxelGrid()
        {
            grid.fill({ VoxelElement::AIR });
        }

        [[nodiscard]] VoxelData* GetVoxelDataPtr(uint16_t x, uint16_t y, uint16_t z)
        {
            return &grid[x + VOXEL_GRID_WIDTH * (y + VOXEL_GRID_HEIGHT * z)];
        }

        [[nodiscard]] VoxelData GetVoxelData(uint16_t x, uint16_t y, uint16_t z) const
        {
            return grid[x + VOXEL_GRID_WIDTH * (y + VOXEL_GRID_HEIGHT * z)];
        }

        [[nodiscard]] VoxelData* GetVoxelDataPtr(const Vector3I& pos)
        {
            return &grid[pos.x + VOXEL_GRID_WIDTH * (pos.y + VOXEL_GRID_HEIGHT * pos.z)];
        }

        [[nodiscard]] VoxelData* GetVoxelDataPtr(const Vector3& pos)
        {
            return &grid[static_cast<uint16_t>(pos.x) + VOXEL_GRID_WIDTH * (static_cast<uint16_t>(pos.y) + VOXEL_GRID_HEIGHT * static_cast<uint16_t>(pos.z))];
        }

        [[nodiscard]] VoxelData GetVoxelData(const Vector3& pos) const
        {
            return grid[static_cast<uint16_t>(pos.x) + VOXEL_GRID_WIDTH * (static_cast<uint16_t>(pos.y) + VOXEL_GRID_HEIGHT * static_cast<uint16_t>(pos.z))];
        }

        [[nodiscard]] VoxelElement GetVoxelElement(uint16_t x, uint16_t y, uint16_t z) const
        {
            return grid[x + VOXEL_GRID_WIDTH * (y + VOXEL_GRID_HEIGHT * z)].type;
        }

        [[nodiscard]] VoxelElement GetVoxelElement(const Vector3& pos) const
        {
            return grid[static_cast<uint16_t>(pos.x) + VOXEL_GRID_WIDTH * (static_cast<uint16_t>(pos.y) + VOXEL_GRID_HEIGHT * static_cast<uint16_t>(pos.z))].type;
        }

        void SetVoxelData(uint16_t x, uint16_t y, uint16_t z, const VoxelData& voxel_data)
        {
            grid[x + VOXEL_GRID_WIDTH * (y + VOXEL_GRID_HEIGHT * z)] = voxel_data;
        }

        void SetVoxelData(const Vector3& pos, const VoxelData& voxel_data)
        {
            grid[static_cast<uint16_t>(pos.x) + VOXEL_GRID_WIDTH * (static_cast<uint16_t>(pos.y) + VOXEL_GRID_HEIGHT * static_cast<uint16_t>(pos.z))] = voxel_data;
        }

        [[nodiscard]] bool IsPositionInsideGrid(uint16_t x, uint16_t y, uint16_t z)
        {
            return IsInRange(x, 0, VOXEL_GRID_WIDTH - 1) && IsInRange(y, 0, VOXEL_GRID_HEIGHT - 1) && IsInRange(z, 0, VOXEL_GRID_DEPTH - 1);
        }

        [[nodiscard]] bool IsPositionInsideGrid(const Vector3& pos)
        {
            return IsPositionInsideGrid(static_cast<uint16_t>(pos.x), static_cast<uint16_t>(pos.y), static_cast<uint16_t>(pos.z));
        }

        [[nodiscard]] bool IsPositionInsideGrid(const Vector3I& pos)
        {
            return IsPositionInsideGrid(pos.x, pos.y, pos.z);
        }

        void Fill(VoxelData voxel_data)
        {
            grid.fill(voxel_data);
        }
    };

}

#endif