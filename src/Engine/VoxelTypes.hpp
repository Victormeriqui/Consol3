#ifndef VOXELTYPES_HPP
#define VOXELTYPES_HPP

#include "Display/RGBColor.hpp"
#include "Display/RGBColorConstants.hpp"

#include <cstdint>
#include <map>
#include <string_view>
#include <unordered_set>
#include <vector>

using namespace Display;

namespace Engine
{
    enum class VoxelType : uint8_t
    {
        AIR = 0,
        CURSOR,
        ROCK,
        SAND,
        WATER
    };

    static std::unordered_set<VoxelType> voxel_liquid_types = { VoxelType::WATER };
    static std::unordered_set<VoxelType> voxel_solid_types  = { VoxelType::ROCK };
    static std::unordered_set<VoxelType> voxel_air_types    = { VoxelType::AIR, VoxelType::CURSOR };

    static std::map<VoxelType, std::vector<RGBColor>> voxel_color_map = { { VoxelType::AIR, { RGBConstants::Black() } },
                                                                          { VoxelType::CURSOR, { RGBConstants::White() } },
                                                                          { VoxelType::ROCK, { RGBColor(158, 158, 158), RGBColor(135, 135, 135), RGBColor(120, 120, 120), RGBColor(107, 107, 107), RGBColor(92, 92, 92) } },
                                                                          { VoxelType::SAND,
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
                                                                          { VoxelType::WATER, { RGBColor(0, 0, 255) } } };
}

#endif