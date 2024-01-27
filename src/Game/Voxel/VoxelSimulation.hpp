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

        struct CandidateSwapInfo
        {
            Vector3I hit_voxel_coord;
            // the voxel immediately before the hit voxel
            Vector3I before_hit_voxel_coord;
        };

        class VoxelSimulation
        {
        private:
            const static uint64_t update_frequency = 3;

            std::shared_ptr<VoxelGrid> voxel_grid;

            CandidateSwapInfo GetCandidateSwap(const Vector3I& from_coord, VoxelData& voxel_data);

        public:
            VoxelSimulation(std::shared_ptr<VoxelGrid> voxel_grid);

            void UpdateSimulationTopDown(uint64_t update_tick);
            void UpdateSimulationDownTop(uint64_t update_tick);
        };
    }
}

#endif