#ifdef SYS_WINDOWS
#ifndef DITHEREDFRAMEDRAWER_HPP
#define DITHEREDFRAMEDRAWER_HPP

#include "Display/FrameBuffer.hpp"
#include "Display/IFrameDrawer.hpp"
#include "Display/RGBColor.hpp"
#include "WindowsTerminalManager.hpp"

// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>
#include <cstdint>
#include <map>
#include <memory>
#include <string>

namespace Display
{
    namespace Windows
    {
        struct DitheredColor
        {
            CHAR_INFO console_color;
            RGBColor real_color;
        };

        constexpr WCHAR dither_25 = 176;
        // this is not exactly 50 on the consolas font, as the border influnces the dot count
        constexpr WCHAR dither_50 = 177;
        constexpr WCHAR dither_75 = 178;
        constexpr WCHAR dither_0  = 32;

        constexpr WORD darkred_fg_black_bg = 0x04;
        constexpr WORD black_fg_darkred_bg = 0x40;
        constexpr WORD red_fg_darkred_bg   = 0x4C;
        constexpr WORD darkred_fg_red_bg   = 0xC4;

        constexpr WORD darkred_fg_white_bg = 0xF4;
        constexpr WORD white_fg_darkred_bg = 0x4F;
        constexpr WORD red_fg_white_bg     = 0xFC;
        constexpr WORD white_fg_red_bg     = 0xCF;

        constexpr WORD darkgreen_fg_black_bg = 0x02;
        constexpr WORD black_fg_darkgreen_bg = 0x20;
        constexpr WORD green_fg_darkgreen_bg = 0x2A;
        constexpr WORD darkgreen_fg_green_bg = 0xA2;

        constexpr WORD darkblue_fg_black_bg = 0x01;
        constexpr WORD black_fg_darkblue_bg = 0x10;
        constexpr WORD blue_fg_darkblue_bg  = 0x19;
        constexpr WORD darkblue_fg_blue_bg  = 0x91;

        constexpr WORD darkcyan_fg_black_bg = 0x03;
        constexpr WORD black_fg_darkcyan_bg = 0x30;
        constexpr WORD cyan_fg_darkcyan_bg  = 0x3B;
        constexpr WORD darkcyan_fg_cyan_bg  = 0xB3;

        constexpr WORD darkmagenta_fg_black_bg   = 0x05;
        constexpr WORD black_fg_darkmagenta_bg   = 0x50;
        constexpr WORD magenta_fg_darkmagenta_bg = 0x5D;
        constexpr WORD darkmagenta_fg_magenta_bg = 0xD5;

        constexpr WORD darkyellow_fg_black_bg  = 0x06;
        constexpr WORD black_fg_darkyellow_bg  = 0x60;
        constexpr WORD yellow_fg_darkyellow_bg = 0x6E;
        constexpr WORD darkyellow_fg_yellow_bg = 0xE6;

        constexpr WORD darkgrey_fg_black_bg = 0x08;
        constexpr WORD black_fg_darkgrey_bg = 0x80;
        constexpr WORD grey_fg_darkgrey_bg  = 0x87;
        constexpr WORD darkgrey_fg_grey_bg  = 0x78;

        constexpr WORD black_fg_black_bg = 0x00;
        constexpr WORD white_fg_grey_bg  = 0x7F;
        constexpr WORD grey_fg_white_bg  = 0xF7;

        static const DitheredColor dithered_colors[] = {
            {{dither_25, darkred_fg_black_bg}, RGBColor(23, 0, 0)},
            {{dither_50, darkred_fg_black_bg}, RGBColor(38, 0, 0)},
            {{dither_50, black_fg_darkred_bg}, RGBColor(102, 0, 0)},
            {{dither_25, black_fg_darkred_bg}, RGBColor(113, 0, 0)},
            {{dither_0, black_fg_darkred_bg}, RGBColor(128, 0, 0)},
            {{dither_25, red_fg_darkred_bg}, RGBColor(146, 0, 0)},
            {{dither_50, red_fg_darkred_bg}, RGBColor(160, 0, 0)},
            {{dither_50, darkred_fg_red_bg}, RGBColor(225, 0, 0)},
            {{dither_25, darkred_fg_red_bg}, RGBColor(238, 0, 0)},
            {{dither_0, darkred_fg_red_bg}, RGBColor(255, 0, 0)},
            {{dither_25, darkgreen_fg_black_bg}, RGBColor(0, 23, 0)},
            {{dither_50, darkgreen_fg_black_bg}, RGBColor(0, 38, 0)},
            {{dither_50, black_fg_darkgreen_bg}, RGBColor(0, 102, 0)},
            {{dither_25, black_fg_darkgreen_bg}, RGBColor(0, 113, 0)},
            {{dither_0, black_fg_darkgreen_bg}, RGBColor(0, 128, 0)},
            {{dither_25, green_fg_darkgreen_bg}, RGBColor(0, 146, 0)},
            {{dither_50, green_fg_darkgreen_bg}, RGBColor(0, 160, 0)},
            {{dither_50, darkgreen_fg_green_bg}, RGBColor(0, 225, 0)},
            {{dither_25, darkgreen_fg_green_bg}, RGBColor(0, 238, 0)},
            {{dither_0, darkgreen_fg_green_bg}, RGBColor(0, 255, 0)},
            {{dither_25, darkblue_fg_black_bg}, RGBColor(0, 0, 23)},
            {{dither_50, darkblue_fg_black_bg}, RGBColor(0, 0, 38)},
            {{dither_50, black_fg_darkblue_bg}, RGBColor(0, 0, 102)},
            {{dither_25, black_fg_darkblue_bg}, RGBColor(0, 0, 113)},
            {{dither_0, black_fg_darkblue_bg}, RGBColor(0, 0, 128)},
            {{dither_25, blue_fg_darkblue_bg}, RGBColor(0, 0, 146)},
            {{dither_50, blue_fg_darkblue_bg}, RGBColor(0, 0, 160)},
            {{dither_50, darkblue_fg_blue_bg}, RGBColor(0, 0, 225)},
            {{dither_25, darkblue_fg_blue_bg}, RGBColor(0, 0, 238)},
            {{dither_0, darkblue_fg_blue_bg}, RGBColor(0, 0, 255)},
            {{dither_25, darkcyan_fg_black_bg}, RGBColor(0, 23, 23)},
            {{dither_50, darkcyan_fg_black_bg}, RGBColor(0, 38, 38)},
            {{dither_50, black_fg_darkcyan_bg}, RGBColor(0, 102, 102)},
            {{dither_25, black_fg_darkcyan_bg}, RGBColor(0, 113, 113)},
            {{dither_0, black_fg_darkcyan_bg}, RGBColor(0, 128, 128)},
            {{dither_25, cyan_fg_darkcyan_bg}, RGBColor(0, 146, 146)},
            {{dither_50, cyan_fg_darkcyan_bg}, RGBColor(0, 160, 160)},
            {{dither_50, darkcyan_fg_cyan_bg}, RGBColor(0, 225, 225)},
            {{dither_25, darkcyan_fg_cyan_bg}, RGBColor(0, 238, 238)},
            {{dither_0, darkcyan_fg_cyan_bg}, RGBColor(0, 255, 255)},
            {{dither_25, darkmagenta_fg_black_bg}, RGBColor(23, 0, 23)},
            {{dither_50, darkmagenta_fg_black_bg}, RGBColor(38, 0, 38)},
            {{dither_50, black_fg_darkmagenta_bg}, RGBColor(102, 0, 102)},
            {{dither_25, black_fg_darkmagenta_bg}, RGBColor(113, 0, 113)},
            {{dither_0, black_fg_darkmagenta_bg}, RGBColor(128, 0, 128)},
            {{dither_25, magenta_fg_darkmagenta_bg}, RGBColor(146, 0, 146)},
            {{dither_50, magenta_fg_darkmagenta_bg}, RGBColor(160, 0, 160)},
            {{dither_50, darkmagenta_fg_magenta_bg}, RGBColor(225, 0, 225)},
            {{dither_25, darkmagenta_fg_magenta_bg}, RGBColor(238, 0, 238)},
            {{dither_0, darkmagenta_fg_magenta_bg}, RGBColor(255, 0, 255)},
            {{dither_25, darkyellow_fg_black_bg}, RGBColor(23, 23, 0)},
            {{dither_50, darkyellow_fg_black_bg}, RGBColor(38, 38, 0)},
            {{dither_50, black_fg_darkyellow_bg}, RGBColor(102, 102, 0)},
            {{dither_25, black_fg_darkyellow_bg}, RGBColor(113, 113, 0)},
            {{dither_0, black_fg_darkyellow_bg}, RGBColor(128, 128, 0)},
            {{dither_25, yellow_fg_darkyellow_bg}, RGBColor(146, 146, 0)},
            {{dither_50, yellow_fg_darkyellow_bg}, RGBColor(160, 160, 0)},
            {{dither_50, darkyellow_fg_yellow_bg}, RGBColor(225, 225, 0)},
            {{dither_25, darkyellow_fg_yellow_bg}, RGBColor(238, 238, 0)},
            {{dither_0, darkyellow_fg_yellow_bg}, RGBColor(255, 255, 0)},
            {{dither_0, black_fg_black_bg}, RGBColor(0, 0, 0)},
            {{dither_25, darkgrey_fg_black_bg}, RGBColor(15, 15, 15)},
            {{dither_50, darkgrey_fg_black_bg}, RGBColor(32, 32, 32)},
            {{dither_50, black_fg_darkgrey_bg}, RGBColor(95, 95, 95)},
            {{dither_25, black_fg_darkgrey_bg}, RGBColor(113, 113, 113)},
            {{dither_0, black_fg_darkgrey_bg}, RGBColor(128, 128, 128)},
            {{dither_25, grey_fg_darkgrey_bg}, RGBColor(135, 135, 135)},
            {{dither_50, grey_fg_darkgrey_bg}, RGBColor(143, 143, 143)},
            {{dither_50, darkgrey_fg_grey_bg}, RGBColor(176, 176, 176)},
            {{dither_25, darkgrey_fg_grey_bg}, RGBColor(184, 184, 184)},
            {{dither_0, darkgrey_fg_grey_bg}, RGBColor(192, 192, 192)},
            {{dither_25, white_fg_grey_bg}, RGBColor(199, 199, 199)},
            {{dither_50, white_fg_grey_bg}, RGBColor(204, 204, 204)},
            {{dither_50, grey_fg_white_bg}, RGBColor(239, 239, 239)},
            {{dither_25, grey_fg_white_bg}, RGBColor(247, 247, 247)},
            {{dither_0, grey_fg_white_bg}, RGBColor(255, 255, 255)},
        };

        class DitheredFrameDrawer : public IFrameDrawer
        {
        private:
            std::shared_ptr<FrameBuffer<CHAR_INFO>> framebuffer;
            WindowsTerminalManager terminal_manager;

        public:
            DitheredFrameDrawer(std::shared_ptr<FrameBuffer<CHAR_INFO>> framebuffer);

            virtual void SetupFrameDrawer() override;

            virtual void SetPixel(uint16_t x, uint16_t y, RGBColor color) override;

            virtual void DisplayFrame() override;

            virtual void ClearFrameBuffer() override;

            virtual void ReportInformation(const std::string& info) override;

            [[nodiscard]] virtual const uint16_t GetFrameBufferWidth() const override;
            [[nodiscard]] virtual const uint16_t GetFrameBufferHeight() const override;
        };
    }
}

#endif
#endif
