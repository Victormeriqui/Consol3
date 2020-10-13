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

using namespace Display;
using namespace Engine;

int main()
{
	uint16_t width = 200;
	uint16_t height = 200;

	std::shared_ptr<FrameBuffer<CHAR_INFO>> framebuffer = std::make_shared<FrameBuffer<CHAR_INFO>>(FrameBuffer<CHAR_INFO>(width, height));
	std::shared_ptr<GreyscaleRenderer> renderer = std::make_shared<GreyscaleRenderer>(framebuffer);

	Consol3Engine engine = Consol3Engine(renderer);

	engine.Start();

	//char a;
	//scanf_s("%c", &a, 1);
	return 0;
}