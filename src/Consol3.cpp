#include <iostream>
#include <cstdio>
#include <cstdio>
#include <memory>

#include "Engine/Consol3Engine.hpp"
#include "Display/DitheredFrameBuffer.hpp"
#include "Display/GreyscaleFrameBuffer.hpp"

using namespace Display;
using namespace Engine;

int main()
{
	uint16_t width = 200;
	uint16_t height = 200;

	std::shared_ptr<DitheredFrameBuffer> framebuffer = std::make_shared<DitheredFrameBuffer>(DitheredFrameBuffer(width, height));
	//std::shared_ptr<GreyscaleFrameBuffer> framebuffer = std::make_shared<GreyscaleFrameBuffer>(GreyscaleFrameBuffer(width, height));
	
	Consol3Engine engine = Consol3Engine(framebuffer);
	
	engine.Start();
	
	//char a;
	//scanf_s("%c", &a, 1);
	return 0;
}

