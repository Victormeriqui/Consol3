#ifndef RGBCONSTANTS_HPP
#define RGBCONSTANTS_HPP

#include "RGBColor.hpp"

namespace Display
{
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
