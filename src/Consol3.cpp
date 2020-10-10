#include <iostream>
#include <cstdio>
#include <cstdio>

#include "Display/FrameBuffer.hpp"
#include "Engine/Consol3Engine.hpp"
#include "Display/DitheredPixelTranslator.hpp"
#include "Display/GreyscalePixelTranslator.hpp"

using namespace Display;
using namespace Engine;

int main()
{
	uint16_t width = 200;
	uint16_t height = 200;

	FrameBuffer framebuffer = FrameBuffer(width, height);
	framebuffer.ClearBuffer();

	//GreyscalePixelTranslator pixel_translator = GreyscalePixelTranslator();
	DitheredPixelTranslator pixel_translator = DitheredPixelTranslator();

	Consol3Engine engine = Consol3Engine(framebuffer, pixel_translator);
	
	engine.Start();
	
	//char a;
	//scanf_s("%c", &a, 1);
	return 0;
}

