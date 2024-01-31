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

        Vector3I VoxelSimulation::GetCandidateSwapPos(const Vector3I& from_coord, VoxelData& voxel_data)
        {
            voxel_data.velocity_threshold += voxel_data.velocity;
            Vector3I candidate_coord = from_coord + Vector3I(static_cast<int>(voxel_data.velocity_threshold.x), static_cast<int>(voxel_data.velocity_threshold.y), static_cast<int>(voxel_data.velocity_threshold.z));

            if (std::abs(voxel_data.velocity_threshold.x) >= 1.0f)
                voxel_data.velocity_threshold.x = 0.0f;

            if (std::abs(voxel_data.velocity_threshold.y) >= 1.0f)
                voxel_data.velocity_threshold.y = 0.0f;

            if (std::abs(voxel_data.velocity_threshold.z) >= 1.0f)
                voxel_data.velocity_threshold.z = 0.0f;

            voxel_grid->SetVoxelData(from_coord, voxel_data);

            return candidate_coord;
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
                        VoxelElementSettings cur_voxel_settings = voxel_element_settings_map[cur_voxel_data.type];

                        // non simulated types like air
                        if (cur_voxel_settings.skip_simulation)
                            continue;

                        // static types never move
                        if (cur_voxel_settings.movement_type == VoxelMovementType::STATIC)
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

                        // under our current pos
                        Vector3I cur_voxel_under_pos         = cur_voxel_pos + VoxelUtil::down;
                        VoxelElement cur_voxel_under_element = voxel_grid->GetVoxelElement(cur_voxel_under_pos);

                        // our candidate
                        Vector3I candidate_voxel_pos = GetCandidateSwapPos(cur_voxel_pos, cur_voxel_data);

                        VoxelElement candidate_voxel_element = voxel_grid->GetVoxelElement(candidate_voxel_pos);

                        // under our candidate
                        Vector3I candidate_voxel_under_pos         = candidate_voxel_pos + VoxelUtil::down;
                        VoxelElement candidate_voxel_under_element = voxel_grid->GetVoxelElement(candidate_voxel_under_pos);

                        // falling movement
                        if (cur_voxel_data.is_falling)
                        {
                            // swap if the candidate is air
                            if (candidate_voxel_element == VoxelElement::AIR)
                            {
                                VoxelUtil::SwapVoxels(voxel_grid, cur_voxel_pos, candidate_voxel_pos);
                                cur_voxel_pos = candidate_voxel_pos;
                                continue;
                            }

                            // we hit either a non air element or the grid wall

                            // under our poisiton is air, we can fall vertically
                            if (cur_voxel_under_element == VoxelElement::AIR)
                            {
                                // remove all horiontal velocity so we just fall vertically, keep original vertical velocity or a head start if there is none
                                cur_voxel_data.velocity   = Vector3(0.0f, -1.0f, 0.0f);
                                cur_voxel_data.is_falling = true;
                                voxel_grid->SetVoxelData(cur_voxel_pos, cur_voxel_data);
                                continue;
                            }

                            // under our position is either a non air element, or the grid floor, so we cant fall anymore
                            cur_voxel_data.is_falling = false;

                            // remove all vertical velocity as we cannot fall anymore
                            cur_voxel_data.velocity = Vector3(cur_voxel_data.velocity.x, 0.0f, cur_voxel_data.velocity.z);

                            // if we didn't have horizontal velocity before, add a little bit to slide off to the sides
                            if (cur_voxel_data.velocity.x == 0.0f && cur_voxel_data.velocity.z == 0.0f)
                            {
                                Vector3 rand_vel = VoxelUtil::GetRandomHorizontalVelocity();

                                // adapt the slide off velocity to the element dispersion
                                cur_voxel_data.velocity = rand_vel * cur_voxel_settings.dispersion;
                            }

                            voxel_grid->SetVoxelData(cur_voxel_pos, cur_voxel_data);
                        }
                        // floor sliding movement
                        else
                        {
                            // if we're on top of the grid bounds just sleep
                            if (candidate_voxel_under_element == VoxelElement::OUT_OF_BOUNDS)
                            {
                                cur_voxel_data.is_sleeping = true;
                                voxel_grid->SetVoxelData(cur_voxel_pos, cur_voxel_data);
                                continue;
                            }

                            VoxelData candidate_voxel_under_data                = voxel_grid->GetVoxelData(candidate_voxel_under_pos);
                            VoxelElementSettings candidate_voxel_under_settings = voxel_element_settings_map[candidate_voxel_under_data.type];

                            // apply friction if we're on top of a solid
                            if (candidate_voxel_under_settings.movement_type == VoxelMovementType::SOLID || candidate_voxel_under_settings.movement_type == VoxelMovementType::STATIC)
                            {
                                cur_voxel_data.velocity *= (1.0f - cur_voxel_settings.friction);
                                voxel_grid->SetVoxelData(cur_voxel_pos, cur_voxel_data);
                            }

                            bool should_sleep = std::abs(cur_voxel_data.velocity.x) <= 0.1f && std::abs(cur_voxel_data.velocity.z) <= 0.1f;

                            // try to sleep if we don't have enough velocity
                            if (should_sleep)
                            {
                                // dont sleep if we're a liquid on top of a liquid, push a little bit instead so it disperses
                                if (cur_voxel_settings.movement_type == VoxelMovementType::LIQUID && candidate_voxel_under_settings.movement_type == VoxelMovementType::LIQUID)
                                {
                                    Vector3 rand_vel = VoxelUtil::GetRandomHorizontalVelocity();

                                    // make sure the push  is significant enough to move it
                                    // if (rand_vel.GetLength() < 0.8f)
                                    //  rand_vel *= 6.0f;

                                    cur_voxel_data.velocity = rand_vel * cur_voxel_settings.dispersion;
                                    voxel_grid->SetVoxelData(cur_voxel_pos, cur_voxel_data);
                                    continue;
                                }

                                // cur_voxel_data.is_sleeping = true;
                                // cur_voxel_data.velocity    = Vector3(0.0f, 0.0f, 0.0f);
                                // voxel_grid->SetVoxelData(cur_voxel_pos, cur_voxel_data);
                                // continue;
                            }

                            // swap if the candidate is air or the same element
                            if (candidate_voxel_element == VoxelElement::AIR || (candidate_voxel_element == cur_voxel_data.type && cur_voxel_pos != candidate_voxel_pos))
                            {
                                VoxelUtil::SwapVoxels(voxel_grid, cur_voxel_pos, candidate_voxel_pos);
                                cur_voxel_pos = candidate_voxel_pos;
                            }

                            // under our poisiton is air, we can fall vertically
                            if (candidate_voxel_under_element == VoxelElement::AIR)
                            {
                                cur_voxel_data.velocity   = Vector3(0.0f, -1.0f, 0.0f);
                                cur_voxel_data.is_falling = true;
                                voxel_grid->SetVoxelData(cur_voxel_pos, cur_voxel_data);
                                continue;
                            }
                        }
                    }
                }
            }
        }
    }
}
