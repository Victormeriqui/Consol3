#ifndef VOXELSIMULATION_HPP
#define VOXELSIMULATION_HPP

#include "Engine/VoxelGrid.hpp"
#include "Math/Vector3.hpp"
#include "Math/Vector3I.hpp"

#include <cstdint>
#include <memory>

namespace Game
{
    namespace Voxel
    {
        using namespace Engine;

        const static uint64_t update_frequency = 3;
        const static float gravity_accel       = 0.091f;
        const static float gas_rise_vel        = 0.5f;

        class VoxelSimulation
        {
        private:
            std::shared_ptr<VoxelGrid> voxel_grid;

            Vector3I GetCandidateSwapPos(const Vector3I& from_coord, VoxelData& voxel_data);

        public:
            VoxelSimulation(std::shared_ptr<VoxelGrid> voxel_grid);

            void UpdateSimulationTopDown(uint64_t update_tick);
            void UpdateSimulationDownTop(uint64_t update_tick);
        };
    }
}

#endif