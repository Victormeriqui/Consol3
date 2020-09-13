#include <iostream>
#include <cstdio>
#include <cstdio>

#include "Display/FrameBuffer.hpp"
#include "Engine/Consol3Engine.hpp"

using namespace Display;
using namespace Engine;

int main()
{
	uint16_t width = 200;
	uint16_t height = 200;

	FrameBuffer framebuffer = FrameBuffer(width, height);
	framebuffer.ClearBuffer();

	Consol3Engine engine = Consol3Engine(framebuffer);
	
	engine.Start();
	
	//char a;
	//scanf_s("%c", &a, 1);
	return 0;
}
