#include "MacTerminalManager.hpp"

#include <cstdint>
#include <cstdio>
#include <iostream>

namespace Display
{
    namespace Mac
    {
        MacTerminalManager::MacTerminalManager(short width, short height) : width(width), height(height)
        {
        }

        MacTerminalManager::~MacTerminalManager()
        {
            // restore the console to its original state
            EnableCursor();
        }

        void MacTerminalManager::SetupTerminalManager()
        {
            // set window width & height
            std::cout << "\033[8;" << height + 1 << ";" << width << "t";

            DisableCursor();
        }

        void MacTerminalManager::SetPalette(const uint32_t palette[])
        {
        }

        void MacTerminalManager::SetTitle(const std::string& title)
        {
            std::cout << "\033]2;" << title << "\007";
        }

        void MacTerminalManager::EnableCursor()
        {
        }

        void MacTerminalManager::DisableCursor()
        {
            std::cout << "\033[?12l";
        }

        void MacTerminalManager::WriteFrameBufferData(const char* data)
        {
            // clear screen
            std::cout << "\033[0;0H";

            printf("%s", data);
        }

        void MacTerminalManager::WriteSizedString(const std::string& string, uint64_t size)
        {
            // clear screen
            std::cout << "\033[0;0H";
            printf("%s", string.c_str());
        }
    }

}
