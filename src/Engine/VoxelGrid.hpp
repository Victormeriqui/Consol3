#ifndef VOXELGRID_HPP
#define VOXELGRID_HPP

#include "Math/Util/MathUtil.hpp"
#include "Vector3.hpp"
#include "Vector3I.hpp"
#include "VoxelElements.hpp"

#include <array>
#include <cstdint>

#define VOXEL_GRID_WIDTH  100
#define VOXEL_GRID_HEIGHT 100
#define VOXEL_GRID_DEPTH  100

#define VOXEL_GRID_LEFT      -VOXEL_GRID_WIDTH / 2
#define VOXEL_GRID_RIGHT     VOXEL_GRID_WIDTH / 2 - 1
#define VOXEL_GRID_DOWN      -VOXEL_GRID_HEIGHT / 2
#define VOXEL_GRID_UP        VOXEL_GRID_HEIGHT / 2 - 1
#define VOXEL_GRID_BACKWARDS -VOXEL_GRID_DEPTH / 2
#define VOXEL_GRID_FORWARDS  VOXEL_GRID_DEPTH / 2 - 1

using namespace Math;
using namespace Math::Util;

namespace Engine
{

    struct VoxelData
    {
        VoxelElement type;
        bool processed_flip_flop   = false;
        uint8_t color_index        = 0;
        Vector3 velocity           = Vector3();
        Vector3 velocity_threshold = Vector3();
        bool is_falling            = true;
        bool is_sleeping           = false;

        VoxelData() : type(VoxelElement::AIR)
        {
        }
        VoxelData(VoxelElement type) : type(type)
        {
        }
        VoxelData(VoxelElement type, uint8_t color_index) : type(type), color_index(color_index)
        {
        }
        VoxelData(VoxelElement type, uint8_t color_index, const Vector3& velocity) : type(type), color_index(color_index), velocity(velocity)
        {
        }
    };

    class VoxelGrid
    {
    private:
        std::array<VoxelData, VOXEL_GRID_WIDTH * VOXEL_GRID_HEIGHT * VOXEL_GRID_DEPTH> grid;

        constexpr uint32_t GetIndexFromCoords(Vector3I pos) const
        {
            pos.x += VOXEL_GRID_WIDTH / 2;
            pos.y += VOXEL_GRID_HEIGHT / 2;
            pos.z += VOXEL_GRID_DEPTH / 2;

            return pos.x + VOXEL_GRID_WIDTH * (pos.y + VOXEL_GRID_HEIGHT * pos.z);
        }

    public:
        VoxelGrid()
        {
            grid.fill({ VoxelElement::AIR });
        }

        [[nodiscard]] Vector3I GetGridPosition(const Vector3& world_pos) const
        {
            return Vector3I(static_cast<int>(std::floor(world_pos.x)), static_cast<int>(std::floor(world_pos.y)), static_cast<int>(std::floor(world_pos.z)));
        }

        [[nodiscard]] const VoxelData* GetVoxelDataPtr(const Vector3I& pos) const
        {
            return &grid[GetIndexFromCoords(pos)];
        }

        [[nodiscard]] VoxelData GetVoxelData(const Vector3I& pos) const
        {
            return grid[GetIndexFromCoords(pos)];
        }

        [[nodiscard]] VoxelElement GetVoxelElement(const Vector3I& pos) const
        {
            return grid[GetIndexFromCoords(pos)].type;
        }

        void SetVoxelData(const Vector3I& pos, const VoxelData& voxel_data)
        {
            grid[GetIndexFromCoords(pos)] = voxel_data;
        }

        [[nodiscard]] constexpr bool IsPositionInsideGrid(int x, int y, int z) const
        {
            return IsInRange<int>(x, VOXEL_GRID_LEFT, VOXEL_GRID_RIGHT) && IsInRange<int>(y, VOXEL_GRID_DOWN, VOXEL_GRID_UP) && IsInRange<int>(z, VOXEL_GRID_BACKWARDS, VOXEL_GRID_FORWARDS);
        }

        [[nodiscard]] constexpr bool IsPositionInsideGrid(const Vector3& pos) const
        {
            return IsPositionInsideGrid(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(pos.z));
        }

        [[nodiscard]] constexpr bool IsPositionInsideGrid(const Vector3I& pos) const
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