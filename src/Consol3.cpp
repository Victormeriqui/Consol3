#include <iostream>
#include <cstdio>
#include <cstdio>
#include <memory>

// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>
#include "Engine/Consol3Engine.hpp"
#include "Display/FrameBuffer.hpp"
#include "Display/DitheredRenderer.hpp"
#include "Display/GreyscaleRenderer.hpp"
#include "Display/ANSIRenderer.hpp"

using namespace Display;
using namespace Engine;

int main()
{
	uint16_t width = 100;
	uint16_t height = 100;

	//std::shared_ptr<FrameBuffer<CHAR_INFO>> framebuffer = std::make_shared<FrameBuffer<CHAR_INFO>>(FrameBuffer<CHAR_INFO>(width, height));
	std::shared_ptr<FrameBuffer<RGBColor>> framebuffer = std::make_shared<FrameBuffer<RGBColor>>(width, height);
	//std::shared_ptr<GreyscaleRenderer> renderer = std::make_shared<GreyscaleRenderer>(framebuffer);
	//std::shared_ptr<DitheredRenderer> renderer = std::make_shared<DitheredRenderer>(framebuffer);
	std::shared_ptr<ANSIRenderer> renderer = std::make_shared<ANSIRenderer>(framebuffer);

	Consol3Engine engine = Consol3Engine(renderer);
	
	engine.Start();

	//char a;
	//scanf_s("%c", &a, 1);
	return 0;
}