#include "LinuxTerminalManager.hpp"

#include <cstdint>
#include <iostream>

namespace Display
{
	namespace Linux
	{
		LinuxTerminalManager::LinuxTerminalManager(short width, short height) : width(width), height(height)
		{
			// set window width & height
			std::cout << "\033[8;" << height + 1 << ";" << width << "t";

			DisableCursor();
		}

		LinuxTerminalManager::~LinuxTerminalManager()
		{
			// restore the console to its original state
			EnableCursor();
		}

		void LinuxTerminalManager::SetPalette(const uint32_t palette[])
		{
		}

		void LinuxTerminalManager::SetTitle(const std::string& title)
		{
			std::cout << "\033]2;" << title << "\007";
		}

		void LinuxTerminalManager::EnableCursor()
		{
		}

		void LinuxTerminalManager::DisableCursor()
		{
			std::cout << "\033[?12l";
		}

		void LinuxTerminalManager::WriteFrameBufferData(const char* data)
		{
			// clear screen
			std::cout << "\033[0;0H";

			std::cout << data;
		}

		void LinuxTerminalManager::WriteSizedString(const std::string& string, uint64_t size)
		{
			// clear screen
			std::cout << "\033[0;0H";
			std::cout << string;
		}
	}

}
