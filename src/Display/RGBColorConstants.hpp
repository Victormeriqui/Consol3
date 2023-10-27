#ifndef RGBCONSTANTS_HPP
#define RGBCONSTANTS_HPP

#include "RGBColor.hpp"

namespace Display
{
    static const uint32_t palette_textonly[16] = { 0x000000, 0x111111, 0x212121, 0x333333, 0x444444, 0x565656, 0x666666, 0x777777, 0x898989, 0x999999, 0xAAAAAA, 0xBCBCBC, 0xCCCCCC, 0xDDDDDD, 0xEFEFEF, 0xFFFFFF };

    class RGBConstants
    {
    public:
        constexpr static RGBColor White()
        {
            return RGBColor(255, 255, 255);
        }

        constexpr static RGBColor Black()
        {
            return RGBColor(0, 0, 0);
        }

        constexpr static RGBColor Red()
        {
            return RGBColor(255, 0, 0);
        }

        constexpr RGBColor Green()
        {
            return RGBColor(0, 255, 0);
        }

        constexpr static RGBColor Blue()
        {
            return RGBColor(0, 0, 255);
        }
    };

}

#endif
