#include <cstdio>
#include <iostream>
#include <memory>

#define SYS_WIN 1
#define SYS_LIN 2

#define CUR_SYS SYS_LIN

#include "Display/FrameBuffer.hpp"
#include "Engine/Consol3Engine.hpp"

#if CUR_SYS == SYS_WIN
#include "Display/Windows/DitheredFrameDrawer.hpp"
#include "Display/Windows/DitheredGreyscaleFrameDrawer.hpp"
#include "Display/Windows/GreyscaleFrameDrawer.hpp"
#include "Display/Windows/WindowsTerminalManager.hpp"
// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>
#elif CUR_SYS == SYS_LIN
#include "Display/Linux/LinuxTerminalManager.hpp"
typedef struct _CHAR_INFO
{
	union
	{
		char UnicodeChar;
		char AsciiChar;
	} Char;
	uint32_t Attributes;
} CHAR_INFO, *PCHAR_INFO;
#endif

#include "Display/Multiplatform/TextOnlyFrameDrawer.hpp"
#include "Display/Multiplatform/VT24BitFrameDrawer.hpp"
#include "Display/Multiplatform/VT8BitFrameDrawer.hpp"

enum class FrameDrawerSelection
{
	DITHERED,
	GREYSCALE,
	DITHEREDGREYSCALE,
	TEXTONLY,
	VT8BIT,
	VT24BIT,
};

using namespace Display;
using namespace Engine;

int main()
{
	uint16_t width	= 100;
	uint16_t height = 100;

	FrameDrawerSelection selected_frame_drawer = FrameDrawerSelection::TEXTONLY;

	std::shared_ptr<IFrameDrawer> frame_drawer;

	std::shared_ptr<FrameBuffer<uint32_t>> uint32_t_framebuffer	  = std::make_shared<FrameBuffer<uint32_t>>(width, height);
	std::shared_ptr<FrameBuffer<uint8_t>> uint8_t_framebuffer	  = std::make_shared<FrameBuffer<uint8_t>>(width, height);
	std::shared_ptr<FrameBuffer<CHAR_INFO>> char_info_framebuffer = std::make_shared<FrameBuffer<CHAR_INFO>>(width, height);
	std::shared_ptr<FrameBuffer<char>> char_framebuffer			  = std::make_shared<FrameBuffer<char>>(width, height);

#if CUR_SYS == SYS_WIN
	switch (selected_frame_drawer)
	{
	case FrameDrawerSelection::DITHERED:
	{
		frame_drawer = std::make_shared<Windows::DitheredFrameDrawer>(char_info_framebuffer);
		break;
	}
	case FrameDrawerSelection::GREYSCALE:
	{
		frame_drawer = std::make_shared<Windows::GreyscaleFrameDrawer>(char_info_framebuffer);
		break;
	}
	case FrameDrawerSelection::DITHEREDGREYSCALE:
	{
		frame_drawer = std::make_shared<Windows::DitheredGreyscaleFrameDrawer>(char_info_framebuffer);
		break;
	}
	// the frame drawers after this point need to be supplied with a terminal manager, as they are multiplatform
	case FrameDrawerSelection::VT8BIT:
	{
		std::shared_ptr<ITerminalManager<CHAR_INFO>> windows_terminal = std::make_shared<Windows::WindowsTerminalManager>(width, height, L"Consolas", 4, 4);
		frame_drawer												  = std::make_shared<Multiplatform::VT8BitFrameDrawer<CHAR_INFO>>(uint8_t_framebuffer, windows_terminal);

		break;
	}
	case FrameDrawerSelection::VT24BIT:
	{
		std::shared_ptr<ITerminalManager<CHAR_INFO>> windows_terminal = std::make_shared<Windows::WindowsTerminalManager>(width, height, L"Consolas", 4, 4);
		frame_drawer												  = std::make_shared<Multiplatform::VT24BitFrameDrawer<CHAR_INFO>>(uint32_t_framebuffer, windows_terminal);

		break;
	}
	case FrameDrawerSelection::TEXTONLY:
	default:
	{
		std::shared_ptr<ITerminalManager<CHAR_INFO>> windows_terminal_terminal_font = std::make_shared<Windows::WindowsTerminalManager>(width, height, L"Terminal", 4, 4);
		frame_drawer																= std::make_shared<Multiplatform::TextOnlyFrameDrawer<CHAR_INFO>>(char_info_framebuffer, windows_terminal_terminal_font);
		break;
	}
	}

#elif CUR_SYS == SYS_LIN
	std::shared_ptr<ITerminalManager<char>> linux_terminal_manager = std::make_shared<Linux::LinuxTerminalManager>(width, height);

	switch (selected_frame_drawer)
	{
	case FrameDrawerSelection::VT8BIT:
	{
		frame_drawer = std::make_shared<Multiplatform::VT8BitFrameDrawer<char>>(uint8_t_framebuffer, linux_terminal_manager);
		break;
	}
	case FrameDrawerSelection::VT24BIT:
	{
		frame_drawer = std::make_shared<Multiplatform::VT24BitFrameDrawer<char>>(uint32_t_framebuffer, linux_terminal_manager);
		break;
	}
	case FrameDrawerSelection::TEXTONLY:
	default:
	{
		frame_drawer = std::make_shared<Multiplatform::TextOnlyFrameDrawer<char>>(char_framebuffer, linux_terminal_manager);
		break;
	}
	}
#endif

	Consol3Engine engine = Consol3Engine(frame_drawer);
	engine.Start();

	// char a;
	// scanf_s("%c", &a, 1);

	return 0;
}
