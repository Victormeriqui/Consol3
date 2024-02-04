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

    struct VoxelElementSettings
    {
        bool skip_simulation = false;
        VoxelMovementType movement_type;
        float friction       = 0.0f;
        float dispersion     = 0.0f;
        float air_resistance = 0.0f;
    };

    enum class VoxelElement : uint8_t
    {
        OUT_OF_BOUNDS = 0,
        AIR,
        CURSOR,
        STONE,
        STEEL,
        SAND,
        WATER,
        STEAM,
        LAVA,
    };

    static std::map<VoxelElement, std::string> voxel_element_name_map = {
        { VoxelElement::AIR, "Air" },   { VoxelElement::CURSOR, "Cursor" }, { VoxelElement::STONE, "Stone" }, { VoxelElement::STEEL, "Steel" },
        { VoxelElement::SAND, "Sand" }, { VoxelElement::WATER, "Water" },   { VoxelElement::STEAM, "Steam" }, { VoxelElement::LAVA, "Lava" },
    };

    static std::map<VoxelElement, VoxelElementSettings> voxel_element_settings_map = {
        { VoxelElement::AIR, { .skip_simulation = true, .movement_type = VoxelMovementType::NONE } },
        { VoxelElement::CURSOR, { .skip_simulation = true, .movement_type = VoxelMovementType::NONE } },
        { VoxelElement::STONE, { .movement_type = VoxelMovementType::STATIC } },
        { VoxelElement::STEEL, { .movement_type = VoxelMovementType::STATIC } },
        { VoxelElement::SAND, { .movement_type = VoxelMovementType::SOLID, .friction = 0.16f, .dispersion = 1.0f, .air_resistance = 0.01f } },
        { VoxelElement::WATER, { .movement_type = VoxelMovementType::LIQUID, .friction = 0.4f, .dispersion = 1.0f } },
        { VoxelElement::STEAM, { .movement_type = VoxelMovementType::GAS, .dispersion = 1.0f } },
        { VoxelElement::LAVA, { .movement_type = VoxelMovementType::LIQUID, .friction = 1.1f, .dispersion = 0.2f } },
    };

    static std::map<VoxelElement, std::vector<RGBColor>> voxel_color_map = {
        { VoxelElement::AIR, { RGBConstants::Black() } },
        { VoxelElement::CURSOR, { RGBConstants::White() } },
        { VoxelElement::STONE, { RGBColor(145, 142, 133) } },
        { VoxelElement::STEEL, { RGBColor(161, 161, 161) } },

        { VoxelElement::SAND,
          {
              RGBColor(254, 236, 207),
              RGBColor(227, 206, 178),
              RGBColor(240, 221, 171),
              RGBColor(240, 206, 158),
              RGBColor(235, 233, 197),
          } },
        { VoxelElement::WATER,
          {
              RGBColor(46, 152, 245),
              RGBColor(106, 168, 245),
              RGBColor(62, 176, 245),
              RGBColor(25, 182, 245),
              RGBColor(79, 145, 245),

          } },
        { VoxelElement::STEAM, { RGBColor(200, 200, 200) } },
        { VoxelElement::LAVA,
          {
              RGBColor(184, 84, 7),
              RGBColor(227, 111, 39),
          } },

    };

}

#endif