#include <cstdio>
#include <iostream>
#include <memory>

// Windows.h overrides std::min
#define NOMINMAX
#include "Display/ANSIRenderer.hpp"
#include "Display/DitheredGreyscaleRenderer.hpp"
#include "Display/DitheredRenderer.hpp"
#include "Display/FrameBuffer.hpp"
#include "Display/GreyscaleRenderer.hpp"
#include "Display/TextOnlyRenderer.hpp"
#include "Engine/Consol3Engine.hpp"
#include <Windows.h>

using namespace Display;
using namespace Engine;

int main()
{
	uint16_t width	= 200;
	uint16_t height = 200;

	std::shared_ptr<FrameBuffer<CHAR_INFO>> framebuffer = std::make_shared<FrameBuffer<CHAR_INFO>>(FrameBuffer<CHAR_INFO>(width, height));
	// std::shared_ptr<FrameBuffer<RGBColor>> framebuffer = std::make_shared<FrameBuffer<RGBColor>>(width, height);
	// std::shared_ptr<GreyscaleRenderer> renderer = std::make_shared<GreyscaleRenderer>(framebuffer);
	// std::shared_ptr<DitheredRenderer> renderer = std::make_shared<DitheredRenderer>(framebuffer);
	// std::shared_ptr<DitheredGreyscaleRenderer> renderer = std::make_shared<DitheredGreyscaleRenderer>(framebuffer);
	// std::shared_ptr<ANSIRenderer> renderer = std::make_shared<ANSIRenderer>(framebuffer);

	std::shared_ptr<TextOnlyRenderer> renderer = std::make_shared<TextOnlyRenderer>(framebuffer);

	Consol3Engine engine = Consol3Engine(renderer);

	engine.Start();

	// char a;
	// scanf_s("%c", &a, 1);
	return 0;
}
