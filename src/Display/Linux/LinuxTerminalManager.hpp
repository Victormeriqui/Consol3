#ifndef LINUXTERMINALMANAGER_HPP
#define LINUXTERMINALMANAGER_HPP

#include "Display/ITerminalManager.hpp"

#include <cstdint>
#include <string>

namespace Display
{
    namespace Linux
    {
        class LinuxTerminalManager : public ITerminalManager<char>
        {
        private:
            short width;
            short height;

        public:
            LinuxTerminalManager(short width, short height);
            ~LinuxTerminalManager();

            virtual void SetupTerminalManager() override;

            virtual void SetPalette(const uint32_t palette[]) override;
            virtual void SetTitle(const std::string& title) override;

            virtual void DisableCursor() override;
            virtual void EnableCursor() override;

            virtual void WriteFrameBufferData(const char* data) override;
            virtual void WriteSizedString(const std::string& string, uint64_t size) override;
        };
    }

}

#endif