#ifndef HSVCOLOR_HPP
#define HSVCOLOR_HPP

#include "RGBColor.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace Display
{
    class HSVColor
    {
    public:
        float hue;
        float saturation;
        float value;

        constexpr HSVColor() : hue(0), saturation(0), value(0)
        {
        }
        constexpr HSVColor(float hue, float saturation, float value) : hue(hue), saturation(saturation), value(value)
        {
        }
        HSVColor(RGBColor rgb_color) : hue(), saturation(), value()
        {
            // http://lolengine.net/blog/2013/01/13/fast-rgb-to-hsv

            float r = rgb_color.r / 255.0f;
            float g = rgb_color.g / 255.0f;
            float b = rgb_color.b / 255.0f;

            float k = 0.0f;

            if (g < b)
            {
                std::swap(g, b);
                k = -1.0f;
            }

            if (r < g)
            {
                std::swap(r, g);
                k = -2.0f / 6.0f - k;
            }

            float chroma = r - std::min(g, b);

            hue        = std::fabs(k + (g - b) / (6.0f * chroma + 1e-20f)) * 360.0f;
            saturation = chroma / (r + 1e-20f);
            value      = r;
        }
    };
}

#endif
