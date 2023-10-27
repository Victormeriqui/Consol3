#ifndef VOXELGRID_HPP
#define VOXELGRID_HPP

#include <array>
#include <cstdint>

namespace Engine
{
    namespace Rendering
    {
        template<typename T, uint16_t WIDTH, uint16_t HEIGHT, uint16_t DEPTH>
        class VoxelGrid
        {
        private:
            std::array<T, WIDTH * HEIGHT * DEPTH> grid;

        public:
            VoxelGrid()
            {
            }

            [[nodiscard]] T GetVoxel(uint16_t x, uint16_t y, uint16_t z) const
            {
                return grid[x + WIDTH * (y + HEIGHT * z)];
            }

            void SetVoxel(uint16_t x, uint16_t y, uint16_t z, T voxel)
            {
                grid[x + WIDTH * (y + HEIGHT * z)] = voxel;
            }
        };
    }
}

#endif