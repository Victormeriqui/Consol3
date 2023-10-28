#ifndef VOXELGRID_HPP
#define VOXELGRID_HPP

#include <array>
#include <cstdint>

#define VOXEL_GRID_WIDTH  100
#define VOXEL_GRID_HEIGHT 100
#define VOXEL_GRID_DEPTH  100

namespace Engine
{
    namespace Rendering
    {
        template<uint16_t WIDTH, uint16_t HEIGHT, uint16_t DEPTH>
        class VoxelGrid
        {
        private:
            std::array<uint8_t, WIDTH * HEIGHT * DEPTH> grid;

        public:
            VoxelGrid(uint8_t default)
            {
                grid.fill(default);
            }

            [[nodiscard]] uint8_t GetVoxel(uint16_t x, uint16_t y, uint16_t z) const
            {
                return grid[x + WIDTH * (y + HEIGHT * z)];
            }

            void SetVoxel(uint16_t x, uint16_t y, uint16_t z, uint8_t voxel)
            {
                grid[x + WIDTH * (y + HEIGHT * z)] = voxel;
            }

            void Fill(uint8_t voxel)
            {
                grid.fill(voxel);
            }
        };
    }
}

#endif