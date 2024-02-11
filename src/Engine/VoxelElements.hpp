#ifndef VOXELELEMENTS_HPP
#define VOXELELEMENTS_HPP

#include "Display/RGBColor.hpp"
#include "Display/RGBColorConstants.hpp"

#include <cstdint>
#include <map>
#include <string>
#include <vector>

using namespace Display;

namespace Engine
{

    enum class VoxelMovementType
    {
        NONE,
        STATIC,
        SOLID,
        LIQUID,
        GAS
    };

    struct VoxelMovementSettings
    {
        float friction       = 0.0f;
        float dispersion     = 0.0f;
        float aquaplaning    = 0.0f;
        float air_resistance = 0.0f;
    };

    struct VoxelTemperatureSettings
    {
        float first_spawn_temperature            = 0.0f;
        float phase_transition_temperature_boost = 0.0f;
        float heat_input_rate                    = 0.0f;
        float heat_output_rate                   = 0.0f;
        float heat_dissipation_rate              = 0.0f;
    };

    struct VoxelElementSettings
    {
        bool skip_simulation = false;

        VoxelMovementType movement_type;
        VoxelMovementSettings movement_settings;

        VoxelTemperatureSettings temperature_settings;
    };

    enum class VoxelElement : uint8_t
    {
        OUT_OF_BOUNDS = 0,
        AIR,
        CURSOR,
        STONE,
        STEEL,
        SAND,
        ICE,
        WATER,
        STEAM,
        LAVA,
    };

    static std::map<VoxelElement, std::string> voxel_element_name_map = {
        {VoxelElement::AIR, "Air"},
        {VoxelElement::CURSOR, "Cursor"},
        {VoxelElement::STEEL, "Steel"},
        {VoxelElement::SAND, "Sand"},
        {VoxelElement::ICE, "Ice"},
        {VoxelElement::WATER, "Water"},
        {VoxelElement::STEAM, "Steam"},
        {VoxelElement::STONE, "Stone"},
        {VoxelElement::LAVA, "Lava"},
    };

    static std::map<VoxelElement, VoxelElementSettings> voxel_element_settings_map = {
        {VoxelElement::OUT_OF_BOUNDS, {.skip_simulation = true, .movement_type = VoxelMovementType::NONE}},
        {VoxelElement::AIR, {.skip_simulation = true, .movement_type = VoxelMovementType::NONE}},
        {VoxelElement::CURSOR, {.skip_simulation = true, .movement_type = VoxelMovementType::NONE}},

        {VoxelElement::STEEL, {.movement_type = VoxelMovementType::STATIC}},

        {VoxelElement::SAND,
         {
             .movement_type = VoxelMovementType::SOLID,
             .movement_settings =
                 {
                     .friction       = 0.16f,
                     .dispersion     = 1.0f,
                     .air_resistance = 0.01f,
                 },
         }},

        {VoxelElement::ICE,
         {
             .movement_type = VoxelMovementType::STATIC,
             .temperature_settings =
                 {
                     .first_spawn_temperature            = -10.0f,
                     .phase_transition_temperature_boost = -1.0f,
                     .heat_input_rate                    = 2.0f,
                     .heat_output_rate                   = 10.0f,
                     .heat_dissipation_rate              = 0.0f,
                 },
         }},
        {VoxelElement::WATER,
         {
             .movement_type     = VoxelMovementType::LIQUID,
             .movement_settings = {.friction = 0.4f, .dispersion = 2.0f, .aquaplaning = 0.5f},
             .temperature_settings =
                 {
                     .first_spawn_temperature = 5.0f,
                     .heat_input_rate         = 40.0f,
                     .heat_output_rate        = 10.0f,
                     .heat_dissipation_rate   = 1.0f,
                 },
         }},
        {VoxelElement::STEAM,
         {
             .movement_type     = VoxelMovementType::GAS,
             .movement_settings = {.dispersion = 1.0f},
             .temperature_settings =
                 {
                     .first_spawn_temperature            = 110.0f,
                     .phase_transition_temperature_boost = 5.0f,
                     .heat_input_rate                    = 0.5f,
                     .heat_output_rate                   = 5.0f,
                     .heat_dissipation_rate              = 0.0f,
                 },
         }},

        {VoxelElement::STONE,
         {
             .movement_type = VoxelMovementType::STATIC,
             .temperature_settings =
                 {
                     .first_spawn_temperature = 25.0f,
                     .heat_input_rate         = 30.0f,
                     .heat_output_rate        = 1.0f,
                     .heat_dissipation_rate   = 0.1f,
                 },
         }},
        {VoxelElement::LAVA,
         {
             .movement_type     = VoxelMovementType::LIQUID,
             .movement_settings = {.friction = 1.1f, .dispersion = 0.5f, .aquaplaning = 0.3f},
             .temperature_settings =
                 {
                     .first_spawn_temperature = 1200.0f,
                     .heat_input_rate         = 5.0f,
                     .heat_output_rate        = 40.0f,
                     .heat_dissipation_rate   = 1.0f,
                 },
         }},
    };

    struct VoxelPhaseTransitionSettings
    {
        float freeze_temperature_threshold;
        float boil_temperature_threshold;
        VoxelElement solid_element;
        VoxelElement liquid_element;
        VoxelElement gas_element;
    };

    static VoxelPhaseTransitionSettings water_phase_transition_settings = {
        .freeze_temperature_threshold = 0.0f,
        .boil_temperature_threshold   = 100.0f,
        .solid_element                = VoxelElement::ICE,
        .liquid_element               = VoxelElement::WATER,
        .gas_element                  = VoxelElement::STEAM,
    };

    static VoxelPhaseTransitionSettings lava_phase_transition_settings = {
        .freeze_temperature_threshold = 500.0f,
        .boil_temperature_threshold   = 9999.0f,
        .solid_element                = VoxelElement::STONE,
        .liquid_element               = VoxelElement::LAVA,
        .gas_element                  = VoxelElement::AIR,
    };

    static std::map<VoxelElement, VoxelPhaseTransitionSettings> voxel_element_phase_transition_map = {
        {VoxelElement::ICE, water_phase_transition_settings},
        {VoxelElement::WATER, water_phase_transition_settings},
        {VoxelElement::STEAM, water_phase_transition_settings},
        {VoxelElement::STONE, lava_phase_transition_settings},
        {VoxelElement::LAVA, lava_phase_transition_settings},
    };

    static std::map<VoxelElement, std::vector<RGBColor>> voxel_color_map = {
        {VoxelElement::AIR, {RGBConstants::Black()}},
        {VoxelElement::CURSOR, {RGBConstants::White()}},
        {VoxelElement::STEEL, {RGBColor(161, 161, 161)}},

        {VoxelElement::SAND,
         {
             RGBColor(254, 236, 207),
             RGBColor(227, 206, 178),
             RGBColor(240, 221, 171),
             RGBColor(240, 206, 158),
             RGBColor(235, 233, 197),
         }},
        {VoxelElement::ICE,
         {
             RGBColor(221, 235, 247),
             RGBColor(216, 236, 250),
         }},
        {VoxelElement::WATER,
         {
             RGBColor(46, 152, 245),
             RGBColor(106, 168, 245),
             RGBColor(62, 176, 245),
             RGBColor(25, 182, 245),
             RGBColor(79, 145, 245),

         }},
        {VoxelElement::STEAM,
         {
             RGBColor(200, 200, 200),
         }},
        {VoxelElement::STONE,
         {
             RGBColor(145, 142, 133),
         }},
        {VoxelElement::LAVA,
         {
             RGBColor(184, 84, 7),
             RGBColor(227, 111, 39),
         }},

    };

}

#endif