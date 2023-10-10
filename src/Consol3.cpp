#include <cstdio>
#include <iostream>
#include <memory>

// Windows.h overrides std::min
#define NOMINMAX
#include "Display/DitheredFrameDrawer.hpp"
#include "Display/DitheredGreyscaleFrameDrawer.hpp"
#include "Display/FrameBuffer.hpp"
#include "Display/GreyscaleFrameDrawer.hpp"
#include "Display/TextOnlyFrameDrawer.hpp"
#include "Display/VT24BitFrameDrawer.hpp"
#include "Display/VT8BitFrameDrawer.hpp"
#include "Engine/Consol3Engine.hpp"
#include <Windows.h>

using namespace Display;
using namespace Engine;

int main()
{
	uint16_t width	= 200;
	uint16_t height = 200;

	// std::shared_ptr<FrameBuffer<CHAR_INFO>> framebuffer = std::make_shared<FrameBuffer<CHAR_INFO>>(FrameBuffer<CHAR_INFO>(width, height));
	// std::shared_ptr<DitheredFrameDrawer> frame_drawer			= std::make_shared<DitheredFrameDrawer>(framebuffer);

	// std::shared_ptr<FrameBuffer<uint32_t>> framebuffer = std::make_shared<FrameBuffer<uint32_t>>(width, height);
	// std::shared_ptr<VT24BitFrameDrawer> frame_drawer	   = std::make_shared<VT24BitFrameDrawer>(framebuffer);

	std::shared_ptr<FrameBuffer<uint8_t>> framebuffer = std::make_shared<FrameBuffer<uint8_t>>(width, height);
	std::shared_ptr<VT8BitFrameDrawer> frame_drawer	  = std::make_shared<VT8BitFrameDrawer>(framebuffer);

	// std::shared_ptr<GreyscaleFrameDrawer> frame_drawer = std::make_shared<GreyscaleFrameDrawer>(framebuffer);

	// std::shared_ptr<DitheredGreyscaleFrameDrawer> frame_drawer = std::make_shared<DitheredGreyscaleFrameDrawer>(framebuffer);
	// std::shared_ptr<TextOnlyFrameDrawer> frame_drawer = std::make_shared<TextOnlyFrameDrawer>(framebuffer);

	Consol3Engine engine = Consol3Engine(frame_drawer);

	engine.Start();

	// char a;
	// scanf_s("%c", &a, 1);

	return 0;
}
