#ifdef SYS_WINDOWS

#include "GreyscaleFrameDrawer.hpp"

namespace Display
{
    namespace Windows
    {
        GreyscaleFrameDrawer::GreyscaleFrameDrawer(std::shared_ptr<FrameBuffer<CHAR_INFO>> framebuffer) :
            framebuffer(std::move(framebuffer)),
            terminal_manager(WindowsTerminalManager(this->framebuffer->GetWidth(), this->framebuffer->GetHeight(), L"Consolas", 4, 4, palette_greyscale))
        {
            // ensure that every char is a space, that way we can just control the color
            this->framebuffer->FillBuffer({ { ' ' }, 0x00 });
        }

        void GreyscaleFrameDrawer::SetPixel(uint16_t x, uint16_t y, RGBColor color)
        {
            float luminance = color.GetColorNormal();

            // map it to a 0-15 palette index
            uint8_t index            = 16 + (uint8_t)((15.0f * luminance));
            uint8_t background_index = index * 16;

            // in greyscale all pixels are a space and we control the color through the background
            framebuffer->SetValue(x, y, { { ' ' }, background_index });
        }

        void GreyscaleFrameDrawer::DisplayFrame()
        {
            terminal_manager.WriteFrameBufferData(framebuffer->GetFrameBufferData());
        }

        void GreyscaleFrameDrawer::ReportInformation(const std::string& info)
        {
            terminal_manager.SetTitle(info);
        }

        void GreyscaleFrameDrawer::ClearFrameBuffer()
        {
            this->framebuffer->FillBuffer({ { ' ' }, 0x00 });
        }

        const uint16_t GreyscaleFrameDrawer::GetFrameBufferWidth() const
        {
            return framebuffer->GetWidth();
        }

        const uint16_t GreyscaleFrameDrawer::GetFrameBufferHeight() const
        {
            return framebuffer->GetHeight();
        }

    }
}
#endif
