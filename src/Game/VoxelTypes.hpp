#ifndef VOXELTYPES_HPP
#define VOXELTYPES_HPP

#include "Display/RGBColor.hpp"
#include "Display/RGBColorConstants.hpp"

#include <cstdint>
#include <map>
#include <string_view>

using namespace Display;

namespace Game
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

    static const std::map<uint8_t, RGBColor> ConvertTypeMapToPalette(const std::map<VoxelType, VoxelData>& voxel_type_map)
    {
        std::map<uint8_t, RGBColor> voxel_palette_map;

        for (const auto& entry : voxel_type_map)
            voxel_palette_map[static_cast<uint8_t>(entry.first)] = entry.second.color;

        return voxel_palette_map;
    }

}

#endif