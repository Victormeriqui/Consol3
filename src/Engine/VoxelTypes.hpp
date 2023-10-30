#ifndef VOXELTYPES_HPP
#define VOXELTYPES_HPP

#include "Display/RGBColor.hpp"
#include "Display/RGBColorConstants.hpp"

#include <cstdint>
#include <map>
#include <string_view>
#include <vector>

using namespace Display;

namespace Engine
{
    enum class VoxelType : uint8_t
    {
        AIR = 0,
        ROCK,
        SAND,
        WATER
    };

    static std::map<VoxelType, std::vector<RGBColor>> voxel_color_map = { { VoxelType::AIR, { RGBConstants::Black() } },
                                                                          { VoxelType::ROCK, { RGBColor(100, 100, 100) } },
                                                                          { VoxelType::SAND, { RGBColor(255, 255, 0), RGBColor(255, 185, 0), RGBColor(255, 135, 0) } },
                                                                          { VoxelType::WATER, { RGBColor(0, 0, 255) } } };
}

#endif