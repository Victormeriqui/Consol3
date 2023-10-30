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