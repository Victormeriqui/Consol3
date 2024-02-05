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
            VoxelUtil::InitializeUtilPositions(10);
        }

        Vector3I VoxelSimulation::GetCandidateSwapPos(const Vector3I& from_coord, VoxelData& voxel_data)
        {
            voxel_data.velocity_threshold += voxel_data.velocity;

            Vector3I position_offset     = Vector3I(static_cast<int>(voxel_data.velocity_threshold.x), static_cast<int>(voxel_data.velocity_threshold.y), static_cast<int>(voxel_data.velocity_threshold.z));
            Vector3I max_candidate_coord = from_coord + position_offset;

            // no movement this call
            if (max_candidate_coord == from_coord)
            {
                voxel_grid->SetVoxelData(from_coord, voxel_data);
                return from_coord;
            }

            if (std::abs(voxel_data.velocity_threshold.x) >= 1.0f)
                voxel_data.velocity_threshold.x = 0.0f;

            if (std::abs(voxel_data.velocity_threshold.y) >= 1.0f)
                voxel_data.velocity_threshold.y = 0.0f;

            if (std::abs(voxel_data.velocity_threshold.z) >= 1.0f)
                voxel_data.velocity_threshold.z = 0.0f;

            voxel_grid->SetVoxelData(from_coord, voxel_data);

            Ray vel_ray           = Ray(Vector3(from_coord), Vector3(position_offset).GetNormalized());
            MarchResult march_res = vel_ray.MarchUntilHitOrPosition(*voxel_grid, 10, max_candidate_coord);

            return march_res.hit_voxel_coord;
        }

        void VoxelSimulation::ConductHeat(const Vector3I& from_coord, VoxelData& voxel_data, const VoxelElementSettings& voxel_settings)
        {
            // exchange heat with adjacent elements
            for (const Vector3I& adjacent_offset : VoxelUtil::adjacent_positions)
            {
                Vector3I adjacent_voxel_pos         = from_coord + adjacent_offset;
                VoxelElement adjacent_voxel_element = voxel_grid->GetVoxelElement(adjacent_voxel_pos);

                if (adjacent_voxel_element == VoxelElement::AIR || adjacent_voxel_element == VoxelElement::OUT_OF_BOUNDS)
                    continue;

                VoxelData adjacent_voxel_data                = voxel_grid->GetVoxelData(adjacent_voxel_pos);
                VoxelElementSettings adjacent_voxel_settings = voxel_element_settings_map[adjacent_voxel_element];

                float temp_diff = voxel_data.temperature - adjacent_voxel_data.temperature;
                // their temperature is lower than ours, so we can give them heat
                if (temp_diff > 0)
                {
                    float available_to_exchange = std::min(voxel_settings.temperature_settings.heat_output_rate, temp_diff);
                    float exchange              = std::min(available_to_exchange, adjacent_voxel_settings.temperature_settings.heat_input_rate);

                    voxel_data.temperature -= exchange;
                    adjacent_voxel_data.temperature += exchange;

                    voxel_grid->SetVoxelData(from_coord, voxel_data);
                    voxel_grid->SetVoxelData(adjacent_voxel_pos, adjacent_voxel_data);
                }
            }

            // dissipate heat
            if (voxel_data.temperature > voxel_settings.temperature_settings.first_spawn_temperature)
            {
                voxel_data.temperature -= voxel_settings.temperature_settings.heat_dissipation_rate;
                voxel_grid->SetVoxelData(from_coord, voxel_data);
            }
        }

        bool VoxelSimulation::HandlePhaseTransition(const Vector3I& from_coord, VoxelData& voxel_data)
        {
            VoxelPhaseTransitionSettings phase_transition_settings = voxel_element_phase_transition_map[voxel_data.type];

            // boil
            if (voxel_data.temperature > phase_transition_settings.boil_temperature_threshold && voxel_data.type == phase_transition_settings.liquid_element)
            {
                VoxelUtil::PhaseTransitionVoxel(voxel_grid, from_coord, phase_transition_settings.gas_element, voxel_data.temperature);
                return true;
            }
            // condense
            if (voxel_data.temperature <= phase_transition_settings.boil_temperature_threshold && voxel_data.type == phase_transition_settings.gas_element)
            {
                VoxelUtil::PhaseTransitionVoxel(voxel_grid, from_coord, phase_transition_settings.liquid_element, voxel_data.temperature);
                return true;
            }

            // freeze
            if (voxel_data.temperature <= phase_transition_settings.freeze_temperature_threshold && voxel_data.type == phase_transition_settings.liquid_element)
            {
                VoxelUtil::PhaseTransitionVoxel(voxel_grid, from_coord, phase_transition_settings.solid_element, voxel_data.temperature);
                return true;
            }
            // melt
            if (voxel_data.temperature > phase_transition_settings.freeze_temperature_threshold && voxel_data.type == phase_transition_settings.solid_element)
            {
                VoxelUtil::PhaseTransitionVoxel(voxel_grid, from_coord, phase_transition_settings.liquid_element, voxel_data.temperature);
                return true;
            }

            return false;
        }

        void VoxelSimulation::UpdateSimulationDownTop(uint64_t update_tick)
        {
            if (update_tick % update_frequency != 0)
                return;

            bool current_flip_flop = update_tick % 2 != 0;

            for (int y = VOXEL_GRID_DOWN; y <= VOXEL_GRID_UP; y++)
            {
                for (int x = VOXEL_GRID_LEFT; x <= VOXEL_GRID_RIGHT; x++)
                {
                    for (int z = VOXEL_GRID_BACKWARDS; z <= VOXEL_GRID_FORWARDS; z++)
                    {
                        Vector3I cur_voxel_pos                  = Vector3I(x, y, z);
                        VoxelData cur_voxel_data                = voxel_grid->GetVoxelData(cur_voxel_pos);
                        VoxelElementSettings cur_voxel_settings = voxel_element_settings_map[cur_voxel_data.type];

                        // non simulated types like air
                        if (cur_voxel_settings.skip_simulation)
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

                        // temperature handling
                        ConductHeat(cur_voxel_pos, cur_voxel_data, cur_voxel_settings);
                        bool transitioned_phase = HandlePhaseTransition(cur_voxel_pos, cur_voxel_data);

                        if (transitioned_phase)
                            continue;

                        // static types never move
                        if (cur_voxel_settings.movement_type == VoxelMovementType::STATIC)
                            continue;

                        // movement

                        // under our current pos
                        Vector3I cur_voxel_under_pos         = cur_voxel_pos + VoxelUtil::down;
                        VoxelElement cur_voxel_under_element = voxel_grid->GetVoxelElement(cur_voxel_under_pos);

                        // our candidate
                        Vector3I candidate_voxel_pos = GetCandidateSwapPos(cur_voxel_pos, cur_voxel_data);

                        VoxelElement candidate_voxel_element          = voxel_grid->GetVoxelElement(candidate_voxel_pos);
                        VoxelElementSettings candidate_voxel_settings = voxel_element_settings_map[candidate_voxel_element];

                        // under our candidate
                        Vector3I candidate_voxel_under_pos         = candidate_voxel_pos + VoxelUtil::down;
                        VoxelElement candidate_voxel_under_element = voxel_grid->GetVoxelElement(candidate_voxel_under_pos);

                        // falling movement
                        if (cur_voxel_data.is_falling)
                        {
                            // apply gravity for solid and liquid types
                            if (cur_voxel_settings.movement_type == VoxelMovementType::SOLID || cur_voxel_settings.movement_type == VoxelMovementType::LIQUID)
                            {
                                cur_voxel_data.velocity.y -= gravity_accel - cur_voxel_settings.movement_settings.air_resistance;
                                voxel_grid->SetVoxelData(cur_voxel_pos, cur_voxel_data);
                            }

                            // set constant rise velocity for gases (no acceleration)
                            if (cur_voxel_settings.movement_type == VoxelMovementType::GAS)
                            {
                                // random horizonal dispersion
                                Vector3 rand_vel        = VoxelUtil::GetRandomHorizontalVelocity();
                                cur_voxel_data.velocity = rand_vel * cur_voxel_settings.movement_settings.dispersion;
                                // rise up
                                cur_voxel_data.velocity.y = gas_rise_vel - cur_voxel_settings.movement_settings.air_resistance;

                                voxel_grid->SetVoxelData(cur_voxel_pos, cur_voxel_data);
                            }

                            // swap if the candidate is air
                            if (candidate_voxel_element == VoxelElement::AIR || candidate_voxel_settings.movement_type == VoxelMovementType::GAS)
                            {
                                VoxelUtil::SwapVoxels(voxel_grid, cur_voxel_pos, candidate_voxel_pos);
                                continue;
                            }

                            // if we're under the grid bounds just delete
                            if (candidate_voxel_element == VoxelElement::OUT_OF_BOUNDS)
                            {
                                voxel_grid->SetVoxelData(cur_voxel_pos, {.type = VoxelElement::AIR});
                                continue;
                            }

                            // no movement this iteration
                            if (cur_voxel_pos == candidate_voxel_pos || cur_voxel_settings.movement_type == VoxelMovementType::GAS)
                                continue;

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
                                cur_voxel_data.velocity = rand_vel * cur_voxel_settings.movement_settings.dispersion;
                            }

                            voxel_grid->SetVoxelData(cur_voxel_pos, cur_voxel_data);
                        }
                        // floor sliding movement
                        else
                        {
                            // if we're on top of the grid bounds just delete
                            if (cur_voxel_under_element == VoxelElement::OUT_OF_BOUNDS)
                            {
                                voxel_grid->SetVoxelData(cur_voxel_pos, {.type = VoxelElement::AIR});
                                continue;
                            }

                            VoxelData candidate_voxel_under_data                = voxel_grid->GetVoxelData(candidate_voxel_under_pos);
                            VoxelElementSettings candidate_voxel_under_settings = voxel_element_settings_map[candidate_voxel_under_data.type];

                            // apply friction if we're on top of a solid
                            if (candidate_voxel_under_settings.movement_type == VoxelMovementType::SOLID || candidate_voxel_under_settings.movement_type == VoxelMovementType::STATIC)
                            {
                                cur_voxel_data.velocity *= (1.0f - cur_voxel_settings.movement_settings.friction);
                                voxel_grid->SetVoxelData(cur_voxel_pos, cur_voxel_data);
                            }

                            bool should_sleep = std::abs(cur_voxel_data.velocity.x) <= 0.1f && std::abs(cur_voxel_data.velocity.z) <= 0.1f;

                            // try to sleep if we don't have enough velocity
                            if (should_sleep)
                            {
                                // dont sleep if we're a liquid on top of a liquid, push a little bit instead so it disperses
                                if (cur_voxel_settings.movement_type == VoxelMovementType::LIQUID && candidate_voxel_under_settings.movement_type == VoxelMovementType::LIQUID)
                                {
                                    Vector3 rand_vel        = VoxelUtil::GetRandomHorizontalVelocity();
                                    cur_voxel_data.velocity = rand_vel * cur_voxel_settings.movement_settings.aquaplaning;
                                    // make sure the push  is significant enough to move it
                                    // if (rand_vel.GetLength() < 0.8f)
                                    //  rand_vel *= 6.0f;

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
                                cur_voxel_data.velocity += Vector3(0.0f, -1.0f, 0.0f);
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
