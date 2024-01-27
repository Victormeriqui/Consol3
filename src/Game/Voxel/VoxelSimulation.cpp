#include "VoxelSimulation.hpp"

#include "Engine/VoxelElements.hpp"
#include "Math/Ray.hpp"
#include "VoxelUtil.hpp"

#include <cmath>

namespace Game
{
    namespace Voxel
    {
        using namespace Math;

        VoxelSimulation::VoxelSimulation(std::shared_ptr<VoxelGrid> voxel_grid) : voxel_grid(std::move(voxel_grid))
        {
        }

        CandidateSwapInfo VoxelSimulation::GetCandidateSwap(const Vector3I& from_coord, VoxelData& voxel_data)
        {
            // Ray vel_ray            = Ray(Vector3(from_coord), voxel_data.);
            // Vector3I max_end_coord = from_coord + Vector3I(static_cast<int>(voxel_data.velocity.x), static_cast<int>(voxel_data.velocity.y), static_cast<int>(voxel_data.velocity.z));

            voxel_data.velocity_threshold += voxel_data.velocity;
            Vector3I candidate_coord = from_coord + Vector3I(static_cast<int>(voxel_data.velocity_threshold.x), static_cast<int>(voxel_data.velocity_threshold.y), static_cast<int>(voxel_data.velocity_threshold.z));

            if (std::abs(voxel_data.velocity_threshold.x) >= 1.0f)
                voxel_data.velocity_threshold.x = 0.0f;

            if (std::abs(voxel_data.velocity_threshold.y) >= 1.0f)
                voxel_data.velocity_threshold.y = 0.0f;

            if (std::abs(voxel_data.velocity_threshold.z) >= 1.0f)
                voxel_data.velocity_threshold.z = 0.0f;

            voxel_grid->SetVoxelData(from_coord, voxel_data);

            return { .hit_voxel_coord = candidate_coord, .before_hit_voxel_coord = from_coord };

            // MarchResult march_res = vel_ray.MarchUntilHitOrPosition(*voxel_grid, 100, max_end_coord);

            // the hit normal will be pointing towards the ray origin, so we can use it to go one voxel back
            // Vector3I before_hit_coord = march_res.hit_voxel_coord + voxel_grid->GetGridPosition(march_res.hit_normal);

            // return { .hit_voxel_coord = march_res.hit_voxel_coord, .before_hit_voxel_coord = before_hit_coord };
        }

        void VoxelSimulation::UpdateSimulationTopDown(uint64_t update_tick)
        {
            if (update_tick % update_frequency != 0)
                return;

            for (int y = VOXEL_GRID_UP; y > VOXEL_GRID_DOWN; y--)
            {
                for (int x = VOXEL_GRID_LEFT; x < VOXEL_GRID_RIGHT; x++)
                {
                    for (int z = VOXEL_GRID_BACKWARDS; z < VOXEL_GRID_FORWARDS; z++)
                    {
                        Vector3I cur_voxel_pos                  = Vector3I(x, y, z);
                        VoxelData cur_voxel_data                = voxel_grid->GetVoxelData(cur_voxel_pos);
                        VoxelElementSettings cur_voxel_settings = voxel_elements_map[cur_voxel_data.type];

                        // non simulated types like air
                        if (cur_voxel_settings.skip_simulation)
                            continue;

                        // only gas is handled top down
                        if (cur_voxel_settings.movement_type != VoxelMovementType::GAS)
                            continue;

                        // move up, sideways or sideways up
                        std::vector<Vector3I> rand_swap_gases = VoxelUtil::up_sides_and_sides_up;
                        std::shuffle(rand_swap_gases.begin(), rand_swap_gases.end(), VoxelUtil::random_generator);    // randomize the order so theres no spreading pattern

                        for (const Vector3I& move_option : rand_swap_gases)
                        {
                            Vector3I move_pos = cur_voxel_pos + move_option;

                            if (!voxel_grid->IsPositionInsideGrid(move_pos))
                                continue;

                            VoxelElement side_voxel_element = voxel_grid->GetVoxelElement(move_pos);

                            if (side_voxel_element == VoxelElement::AIR)
                            {
                                VoxelUtil::SwapVoxels(voxel_grid, cur_voxel_pos, move_pos);
                                break;
                            }
                        }
                    }
                }
            }
        }

        void VoxelSimulation::UpdateSimulationDownTop(uint64_t update_tick)
        {
            if (update_tick % update_frequency != 0)
                return;

            bool current_flip_flop = update_tick % 2 != 0;

            for (int y = VOXEL_GRID_DOWN; y < VOXEL_GRID_UP; y++)
            {
                for (int x = VOXEL_GRID_LEFT; x < VOXEL_GRID_RIGHT; x++)
                {
                    for (int z = VOXEL_GRID_BACKWARDS; z < VOXEL_GRID_FORWARDS; z++)
                    {
                        Vector3I cur_voxel_pos                  = Vector3I(x, y, z);
                        VoxelData cur_voxel_data                = voxel_grid->GetVoxelData(cur_voxel_pos);
                        VoxelElementSettings cur_voxel_settings = voxel_elements_map[cur_voxel_data.type];

                        // non simulated types like air
                        if (cur_voxel_settings.skip_simulation)
                            continue;

                        // static types never move, and gas types are not simulated down top
                        if (cur_voxel_settings.movement_type == VoxelMovementType::STATIC || cur_voxel_settings.movement_type == VoxelMovementType::GAS)
                            continue;

                        // needs a wakeup from another voxel
                        if (cur_voxel_data.is_sleeping)
                            continue;

                        // we've already processed this voxel
                        if (cur_voxel_data.processed_flip_flop == current_flip_flop)
                            continue;

                        // mark as processed as we will process it next
                        cur_voxel_data.processed_flip_flop = !cur_voxel_data.processed_flip_flop;
                        voxel_grid->SetVoxelData(cur_voxel_pos, cur_voxel_data);

                        // sleep if we have little or no velocity
                        if (std::abs(cur_voxel_data.velocity.x) <= 0.01f && std::abs(cur_voxel_data.velocity.y) <= 0.01f && std::abs(cur_voxel_data.velocity.z) <= 0.01f)
                        {
                            cur_voxel_data.is_sleeping = true;
                            cur_voxel_data.velocity    = Vector3();
                            voxel_grid->SetVoxelData(cur_voxel_pos, cur_voxel_data);
                            continue;
                        }

                        CandidateSwapInfo candidate_voxel = GetCandidateSwap(cur_voxel_pos, cur_voxel_data);
                        Vector3I candidate_voxel_pos      = candidate_voxel.hit_voxel_coord;

                        bool cadidate_is_inside_grid = voxel_grid->IsPositionInsideGrid(candidate_voxel_pos);

                        // if we're not falling we are in contact with the ground, apply friction
                        if (!cur_voxel_data.is_falling)
                        {
                            cur_voxel_data.velocity *= (1.0f - cur_voxel_settings.friction);
                            voxel_grid->SetVoxelData(cur_voxel_pos, cur_voxel_data);
                        }

                        // simple case the is inside the grid and is air, continue in the velocity's vector
                        if (cadidate_is_inside_grid && voxel_grid->GetVoxelElement(candidate_voxel_pos) == VoxelElement::AIR)
                        {
                            // swap with the candidate
                            VoxelUtil::SwapVoxels(voxel_grid, cur_voxel_pos, candidate_voxel_pos);
                            cur_voxel_pos = candidate_voxel_pos;

                            continue;
                        }

                        // only relevant if velocity _mag > 1
                        // we hit either a non air element or the grid wall
                        // swap with hit - 1, so we're just touching the hit voxel
                        // candidate_voxel_pos = candidate_voxel.before_hit_voxel_coord;
                        // VoxelUtil::SwapVoxels(voxel_grid, cur_voxel_pos, candidate_voxel_pos);
                        // cur_voxel_pos = candidate_voxel_pos;

                        // look directly under the new position
                        Vector3I candidate_voxel_pos_under = candidate_voxel_pos + Vector3I(0, -1, 0);

                        // under our new poisiton is air, we can fall vertically
                        if (voxel_grid->IsPositionInsideGrid(candidate_voxel_pos_under) && voxel_grid->GetVoxelElement(candidate_voxel_pos_under) == VoxelElement::AIR)
                        {
                            // remove all horiontal velocity so we just fall vertically, keep original vertical velocity or a head start if there is none
                            cur_voxel_data.velocity   = Vector3(0.0f, cur_voxel_data.velocity.y != 0.0f ? cur_voxel_data.velocity.y : -1.0f, 0.0f);
                            cur_voxel_data.is_falling = true;
                            voxel_grid->SetVoxelData(cur_voxel_pos, cur_voxel_data);
                            continue;
                        }

                        // under our new position is either a non air element, or the grid floor

                        // if we weren't falling when we hit it means we were sliding and hit another voxel
                        if (!cur_voxel_data.is_falling)
                        {
                            // TODO bounce off maybe
                        }

                        cur_voxel_data.is_falling = false;

                        // remove all vertical velocity as we cannot fall anymore
                        cur_voxel_data.velocity = Vector3(cur_voxel_data.velocity.x, 0.0f, cur_voxel_data.velocity.z);

                        // if we didn't have horizontal velocity before, add a little bit
                        if (cur_voxel_data.velocity.x == 0.0f && cur_voxel_data.velocity.z == 0.0f)
                        {
                            Vector3 rand_dir = Vector3(VoxelUtil::rand_direction_component(VoxelUtil::random_generator), 0.0f, VoxelUtil::rand_direction_component(VoxelUtil::random_generator));
                            float rand_mag   = 1.0f;    // VoxelUtil::rand_magnitude_component(VoxelUtil::random_generator);

                            cur_voxel_data.velocity = rand_dir * rand_mag;
                            voxel_grid->SetVoxelData(cur_voxel_pos, cur_voxel_data);
                        }
                    }
                }
            }
        }
    }

}
