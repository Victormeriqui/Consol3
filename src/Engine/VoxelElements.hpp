#ifndef VOXELELEMENTS_HPP
#define VOXELELEMENTS_HPP

#include "Display/RGBColor.hpp"
#include "Display/RGBColorConstants.hpp"

#include <cstdint>
#include <map>
#include <string_view>
#include <vector>

using namespace Display;

namespace Engine
{

    enum class VoxelMovementType
    {
        STATIC,
        SOLID,
        LIQUID,
        GAS
    };

    struct VoxelElementSettings
    {
        bool skip_simulation = false;
        VoxelMovementType movement_type;
    };

    enum class VoxelElement : uint8_t
    {
        AIR = 0,
        CURSOR,
        STONE,
        SAND,
        WATER,
        STEAM
    };

    static std::map<VoxelElement, VoxelElementSettings> voxel_elements_map = {
        { VoxelElement::AIR, { .skip_simulation = true } },
        { VoxelElement::CURSOR, { .skip_simulation = true } },
        { VoxelElement::STONE, { .movement_type = VoxelMovementType::STATIC } },
        { VoxelElement::SAND, { .movement_type = VoxelMovementType::SOLID } },
        { VoxelElement::WATER, { .movement_type = VoxelMovementType::LIQUID } },
        { VoxelElement::STEAM, { .movement_type = VoxelMovementType::GAS } },
    };

    static std::map<VoxelElement, std::vector<RGBColor>> voxel_color_map = { { VoxelElement::AIR, { RGBConstants::Black() } },
                                                                             { VoxelElement::CURSOR, { RGBConstants::White() } },
                                                                             { VoxelElement::STONE, { RGBColor(158, 158, 158), RGBColor(135, 135, 135), RGBColor(120, 120, 120), RGBColor(107, 107, 107), RGBColor(92, 92, 92) } },
                                                                             { VoxelElement::SAND,
                                                                               { RGBColor(245, 219, 178),
                                                                                 RGBColor(235, 195, 130),
                                                                                 RGBColor(221, 184, 121),
                                                                                 RGBColor(204, 167, 110),
                                                                                 RGBColor(187, 157, 102),
                                                                                 RGBColor(176, 145, 94),
                                                                                 RGBColor(162, 134, 86),
                                                                                 RGBColor(148, 124, 78),
                                                                                 RGBColor(134, 113, 70),
                                                                                 RGBColor(121, 103, 63) } },
                                                                             { VoxelElement::WATER, { RGBColor(0, 0, 255) } },
                                                                             { VoxelElement::STEAM, { RGBColor(200, 200, 200) } } };
}

#endif