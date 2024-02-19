#ifdef SYS_WINDOWS
#ifndef GREYSCALEFRAMEDRAWER_HPP
#define GREYSCALEFRAMEDRAWER_HPP

#include "Display/FrameBuffer.hpp"
#include "Display/IFrameDrawer.hpp"
#include "Display/RGBColor.hpp"
#include "WindowsTerminalManager.hpp"

// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>
#include <cstdint>
#include <memory>

namespace Display
{
    namespace Windows
    {
        static const uint32_t palette_greyscale[16] = {0x000000, 0x111111, 0x212121, 0x333333, 0x444444, 0x565656, 0x666666, 0x777777, 0x898989, 0x999999, 0xAAAAAA, 0xBCBCBC, 0xCCCCCC, 0xDDDDDD, 0xEFEFEF, 0xFFFFFF};

        class GreyscaleFrameDrawer : public IFrameDrawer
        {
        private:
            std::shared_ptr<FrameBuffer<CHAR_INFO>> framebuffer;
            WindowsTerminalManager terminal_manager;

        public:
            GreyscaleFrameDrawer(std::shared_ptr<FrameBuffer<CHAR_INFO>> framebuffer);

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
