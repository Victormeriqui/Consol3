#ifndef COLORMAPPING_HPP
#define COLORMAPPING_HPP

#include <cstdint>

namespace Display
{
    static constexpr uint32_t input_space_length = 16777216;

    // these arrays contain the direct color mapping of all 24bit colors to the corresponding palette, thus they are huge
    
    static const uint8_t dithered_color_mapping[16777216] = {
#include "Windows/dithered_color_map.data"
    };

    static const uint8_t vt8bit_color_mapping[16777216] = {
#include "Multiplatform/vt8bit_color_map.data"
    };

}

#endif
