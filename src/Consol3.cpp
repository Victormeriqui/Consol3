#include <cstdio>
#include <iostream>
#include <memory>

#define SYS_WIN 1
#define SYS_LIN 2

#define CUR_SYS SYS_WIN

#include "Display/FrameBuffer.hpp"
#include "Engine/Consol3Engine.hpp"

#if CUR_SYS == SYS_WIN
#include "Display/Windows/DitheredFrameDrawer.hpp"
#include "Display/Windows/DitheredGreyscaleFrameDrawer.hpp"
#include "Display/Windows/GreyscaleFrameDrawer.hpp"
#include "Display/Windows/WindowsTerminalManager.hpp"
#elif CUR_SYS == SYS_LIN
#include "Display/Linux/LinuxTerminalManager.hpp"
#endif

#include "Display/Multiplatform/TextOnlyFrameDrawer.hpp"
#include "Display/Multiplatform/VT24BitFrameDrawer.hpp"
#include "Display/Multiplatform/VT8BitFrameDrawer.hpp"

using namespace Display;
using namespace Engine;

int main()
{
	uint16_t width	= 100;
	uint16_t height = 100;

	// std::shared_ptr<FrameBuffer<CHAR_INFO>> framebuffer = std::make_shared<FrameBuffer<CHAR_INFO>>(width, height);
	// std::shared_ptr<DitheredFrameDrawer> frame_drawer			= std::make_shared<DitheredFrameDrawer>(framebuffer);

	// std::shared_ptr<FrameBuffer<uint32_t>> framebuffer = std::make_shared<FrameBuffer<uint32_t>>(width, height);
	// std::shared_ptr<VT24BitFrameDrawer> frame_drawer	   = std::make_shared<VT24BitFrameDrawer>(framebuffer);

	// std::shared_ptr<FrameBuffer<uint8_t>> framebuffer = std::make_shared<FrameBuffer<uint8_t>>(width, height);
	// std::shared_ptr<VT8BitFrameDrawer> frame_drawer	  = std::make_shared<VT8BitFrameDrawer>(framebuffer);

	// std::shared_ptr<FrameBuffer<CHAR_INFO>> framebuffer = std::make_shared<FrameBuffer<CHAR_INFO>>(width, height);
	// std::shared_ptr<FrameBuffer<char>> framebuffer	   = std::make_shared<FrameBuffer<char>>(width, height);
	// std::shared_ptr<GreyscaleFrameDrawer> frame_drawer = std::make_shared<GreyscaleFrameDrawer>(framebuffer);

	// std::shared_ptr<FrameBuffer<CHAR_INFO>> framebuffer = std::make_shared<FrameBuffer<CHAR_INFO>>(width, height);
	// std::shared_ptr<DitheredGreyscaleFrameDrawer> frame_drawer = std::make_shared<DitheredGreyscaleFrameDrawer>(framebuffer);

	std::shared_ptr<FrameBuffer<CHAR_INFO>> framebuffer = std::make_shared<FrameBuffer<CHAR_INFO>>(width, height);
	// std::shared_ptr<FrameBuffer<char>> framebuffer		   = std::make_shared<FrameBuffer<char>>(width, height);
	// std::shared_ptr<ITerminalManager<char>> linux_terminal = std::make_shared<Linux::LinuxTerminalManager>(width, height);
	std::shared_ptr<ITerminalManager<CHAR_INFO>> windows_terminal =
		std::make_shared<Windows::WindowsTerminalManager>(width, height, L"Terminal", 4, 4);
	std::shared_ptr<Multiplatform::TextOnlyFrameDrawer<CHAR_INFO>> frame_drawer =
		std::make_shared<Multiplatform::TextOnlyFrameDrawer<CHAR_INFO>>(framebuffer, windows_terminal);

	/* std::shared_ptr<FrameBuffer<uint8_t>> framebuffer   = std::make_shared<FrameBuffer<uint8_t>>(width, height);
	std::shared_ptr<ITerminalManager<char>> linux_terminal = std::make_shared<Linux::LinuxTerminalManager>(width, height);
	std::shared_ptr<Multiplatform::VT8BitFrameDrawer<char>> frame_drawer =
		std::make_shared<Multiplatform::VT8BitFrameDrawer<char>>(framebuffer, linux_terminal);*/

	Consol3Engine engine = Consol3Engine(frame_drawer);

	engine.Start();

	// char a;
	// scanf_s("%c", &a, 1);

	return 0;
}
