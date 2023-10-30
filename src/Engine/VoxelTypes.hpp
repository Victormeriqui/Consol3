#ifndef VOXELTYPES_HPP
#define VOXELTYPES_HPP

#include "Display/RGBColor.hpp"
#include "Display/RGBColorConstants.hpp"

#include <cstdint>
#include <map>
#include <string_view>

using namespace Display;

namespace Engine
{

    struct VoxelData
    {
        std::string_view name;
        RGBColor color;
    };

    enum class VoxelType : uint8_t
    {
        AIR = 0,
        ROCK,
        SAND,
        WATER
    };

    static std::map<VoxelType, VoxelData> voxel_type_map = {
        { VoxelType::AIR, { "Air", RGBConstants::Black() } }, { VoxelType::ROCK, { "Rock", RGBConstants::White() } }, { VoxelType::SAND, { "Sand", RGBColor(255, 255, 0) } }, { VoxelType::WATER, { "Water", RGBColor(0, 0, 255) } }
    };

}

#endif