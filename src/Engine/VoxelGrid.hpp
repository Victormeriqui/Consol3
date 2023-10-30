#ifndef VOXELGRID_HPP
#define VOXELGRID_HPP

#include "VoxelTypes.hpp"

#include <array>
#include <cstdint>

#define VOXEL_GRID_WIDTH  100
#define VOXEL_GRID_HEIGHT 100
#define VOXEL_GRID_DEPTH  100

namespace Engine
{
    template<uint16_t WIDTH, uint16_t HEIGHT, uint16_t DEPTH>
    class VoxelGrid
    {
    private:
        std::array<uint8_t, WIDTH * HEIGHT * DEPTH> grid;

        [[nodiscard]] inline uint8_t GetVoxelData(uint16_t x, uint16_t y, uint16_t z) const
        {
            return grid[x + WIDTH * (y + HEIGHT * z)];
        }

        inline void SetVoxelData(uint16_t x, uint16_t y, uint16_t z, uint8_t voxel)
        {
            grid[x + WIDTH * (y + HEIGHT * z)] = voxel;
        }

    public:
        VoxelGrid(uint8_t default)
        {
            grid.fill(default);
        }

        [[nodiscard]] VoxelType GetVoxel(const Vector3& pos) const
        {
            return static_cast<VoxelType>(this->GetVoxelData(static_cast<uint16_t>(pos.x), static_cast<uint16_t>(pos.y), static_cast<uint16_t>(pos.z)));
        }

        [[nodiscard]] VoxelType GetVoxel(uint16_t x, uint16_t y, uint16_t z) const
        {
            return static_cast<VoxelType>(this->GetVoxelData(x, y, z));
        }

        void SetVoxel(const Vector3& pos, VoxelType voxel)
        {
            this->SetVoxelData(static_cast<uint16_t>(pos.x), static_cast<uint16_t>(pos.y), static_cast<uint16_t>(pos.z), static_cast<uint8_t>(voxel));
        }

        void SetVoxel(uint16_t x, uint16_t y, uint16_t z, VoxelType voxel)
        {
            this->SetVoxelData(x, y, z, static_cast<uint8_t>(voxel));
        }

        void Fill(uint8_t voxel)
        {
            grid.fill(voxel);
        }
    };

}

#endif