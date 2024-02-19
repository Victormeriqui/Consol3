#ifdef SYS_WINDOWS

#include "WindowsTerminalManager.hpp"

#include <algorithm>

namespace Display
{
    namespace Windows
    {
        WindowsTerminalManager::WindowsTerminalManager(short width, short height, std::wstring font_name, short font_width, short font_height, const uint32_t color_palette[]) : width(width), height(height), font_name(font_name)
        {
            font_size = {font_width, font_height};

            for (uint8_t i = 0; i < 16; i++)
                palette[i] = color_palette[i];
        }

        WindowsTerminalManager::~WindowsTerminalManager()
        {
            // restore the console to its original state

            SetFontInfo(original_font_name, original_front_size, original_font_weight, original_font_family);
            EnableCursor();
        }

        void WindowsTerminalManager::SetupTerminalManager()
        {
            consolescreenbuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,          // access
                                                            FILE_SHARE_READ | FILE_SHARE_WRITE,    // share mode
                                                            NULL,                                  // security attributes
                                                            CONSOLE_TEXTMODE_BUFFER,               // mode TODO: check if CONSOLE_GRAPHICS_BUFFER works
                                                            NULL);                                 // used by CONSOLE_GRAPHICS_BUFFER

            SetConsoleScreenBufferInfo(consolescreenbuffer, width, height, font_size.X, font_size.Y, palette);

            writeregion = {0, 0, width, height};

            SetConsoleActiveScreenBuffer(consolescreenbuffer);

            StoreOriginalFontInfo();
            SetFontInfo(font_name, font_size, 0, 0);

            DisableCursor();
            // SetConsoleMode(consolescreenbuffer, ENABLE_WRAP_AT_EOL_OUTPUT);
        }

        void WindowsTerminalManager::SetConsoleScreenBufferInfo(HANDLE consolescreenbuffer, short width, short height, short font_width, short font_height, const uint32_t palette[])
        {
            CONSOLE_SCREEN_BUFFER_INFOEX info;

            // struct
            info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
            // resolution, how many chars there are
            info.dwSize = {width, static_cast<SHORT>(height + 1)};    // the +1 avoids problems when resizing
            // position of the cursor (irrelevant since we're going to hide it)
            info.dwCursorPosition = {0, 0};
            // attributes used by preceding writes (irrelevant since we're not going to give input to the console)
            info.wAttributes = 0;
            // window size
            info.srWindow = {0, 0, static_cast<SHORT>(width + 1), static_cast<SHORT>(height + 1)};
            // maximum window size in chars
            info.dwMaximumWindowSize = {1920, 1080};
            // attributes used by popups (irrelevant)
            info.wPopupAttributes = 0;
            // TODO: test this?
            info.bFullscreenSupported = false;
            std::copy(palette, palette + 16, info.ColorTable);

            SetConsoleScreenBufferInfoEx(consolescreenbuffer, &info);
        }

        void WindowsTerminalManager::SetPalette(const uint32_t palette[])
        {
            CONSOLE_SCREEN_BUFFER_INFOEX curinfo = {};

            // the struct size needs to be filled before the call to GetConsoleScreenBufferInfoEx, otherwise it will fail for some reason
            curinfo.cbSize = sizeof(curinfo);

            GetConsoleScreenBufferInfoEx(consolescreenbuffer, &curinfo);

            std::copy(palette, palette + 16, curinfo.ColorTable);

            SetConsoleScreenBufferInfoEx(consolescreenbuffer, &curinfo);
        }

        void WindowsTerminalManager::StoreOriginalFontInfo()
        {
            CONSOLE_FONT_INFOEX font_info;

            // the struct size needs to be filled before the call to GetCurrentConsoleFontEx, otherwise it will fail for some reason
            font_info.cbSize = sizeof(font_info);

            GetCurrentConsoleFontEx(consolescreenbuffer, false, &font_info);

            original_front_size  = font_info.dwFontSize;
            original_font_name   = std::wstring(font_info.FaceName);
            original_font_weight = font_info.FontWeight;
            original_font_family = font_info.FontFamily;
        }

        void WindowsTerminalManager::SetFontInfo(std::wstring name, COORD font_size, short weight, short family)
        {
            CONSOLE_FONT_INFOEX font_info;

            font_info.cbSize = sizeof(font_info);

            font_info.dwFontSize = font_size;

            wcscpy_s(font_info.FaceName, name.c_str());

            font_info.FontWeight = weight;

            font_info.FontFamily = family;
            font_family          = family;

            SetCurrentConsoleFontEx(consolescreenbuffer, false, &font_info);

            CONSOLE_FONT_INFOEX font_info2;

            // the struct size needs to be filled before the call to GetCurrentConsoleFontEx, otherwise it will fail for some reason
            font_info2.cbSize = sizeof(font_info2);

            GetCurrentConsoleFontEx(consolescreenbuffer, false, &font_info2);
        }

        void WindowsTerminalManager::SetTitle(const std::string& title)
        {
            SetConsoleTitleA(title.c_str());
        }

        void WindowsTerminalManager::EnableCursor()
        {
            CONSOLE_CURSOR_INFO cursorinfo;

            GetConsoleCursorInfo(consolescreenbuffer, &cursorinfo);
            cursorinfo.bVisible = true;

            SetConsoleCursorInfo(consolescreenbuffer, &cursorinfo);
        }

        void WindowsTerminalManager::DisableCursor()
        {
            CONSOLE_CURSOR_INFO cursorinfo;

            GetConsoleCursorInfo(consolescreenbuffer, &cursorinfo);
            cursorinfo.bVisible = false;

            SetConsoleCursorInfo(consolescreenbuffer, &cursorinfo);
        }

        void WindowsTerminalManager::WriteFrameBufferData(const CHAR_INFO* data)
        {
            WriteConsoleOutputA(consolescreenbuffer,    // screen buffer to write to
                                data,                   // data to write
                                {width, height},        // size of the data to write
                                {0, 0},                 // start of the data to write
                                &writeregion);          // region to write to
        }

        void WindowsTerminalManager::WriteSizedString(const std::string& string, uint64_t size)
        {
            DWORD written_count;

            SetConsoleCursorPosition(consolescreenbuffer, {0, 0});

            WriteFile(consolescreenbuffer, string.c_str(), (DWORD)size, &written_count, nullptr);
        }
    }
}

#endif
