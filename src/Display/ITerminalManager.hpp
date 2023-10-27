#ifndef ITERMINALMANAGER_HPP
#define ITERMINALMANAGER_HPP

#include "RGBColor.hpp"

#include <stdint.h>
#include <string>

namespace Display
{
    template<typename T>
    class ITerminalManager
    {
    protected:
        ITerminalManager()
        {
        }

        virtual ~ITerminalManager() = default;

    public:
        virtual void SetPalette(const uint32_t palette[]) = 0;
        virtual void SetTitle(const std::string& title)   = 0;

        virtual void DisableCursor() = 0;
        virtual void EnableCursor()  = 0;

        virtual void WriteFrameBufferData(const T* data) = 0;
        // should contain ansi escape sequences, growing larger than the framebuffer size
        virtual void WriteSizedString(const std::string& string, uint64_t size) = 0;
    };
}

#endif
