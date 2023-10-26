

#ifndef VT24BITFRAMEDRAWER_HPP
#define VT24BITFRAMEDRAWER_HPP

#include "Display/FrameBuffer.hpp"
#include "Display/IFrameDrawer.hpp"
#include "Display/ITerminalManager.hpp"
#include "Display/RGBColor.hpp"

#include <cstdint>
#include <memory>
#include <string>

namespace Display
{
    namespace Multiplatform
    {
        template<typename T>
        class VT24BitFrameDrawer : public IFrameDrawer
        {
        private:
            std::shared_ptr<FrameBuffer<uint32_t>> framebuffer;
            std::shared_ptr<ITerminalManager<T>> terminal_manager;

            std::string framebuffer_string;
            uint64_t framebuffer_string_len;

            const std::string esc_sequence_start = "\x1b[48;2;";
            const uint8_t esc_sequence_start_len = (uint8_t)esc_sequence_start.length();
            const std::string esc_sequence_end   = "m";
            const uint8_t esc_sequence_len       = (uint8_t)esc_sequence_end.length();

            void TranslateFrameBuffer();

        public:
            VT24BitFrameDrawer(std::shared_ptr<FrameBuffer<uint32_t>> framebuffer, std::shared_ptr<ITerminalManager<T>> terminal_manager);

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
