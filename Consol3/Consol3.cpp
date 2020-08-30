#include <iostream>
#include <cstdio>

#include "FrameBuffer.hpp"
#include "GreyscaleFrameBufferRenderer.hpp"
#include "MathUtil.hpp"

using namespace Engine::Math;
using namespace Display;

int random(int min, int max)
{
	return rand() % (max + 1 - min) + min;
}

int main()
{
	float width = 100;
	float height = 80;

	FrameBuffer framebuffer = FrameBuffer(width, height);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			uint8_t inc = Util::Lerp(x / (float)width, 0, 255);
			uint8_t dec = Util::Lerp(x / (float)width, 255, 0);
			Color col1 = Color(inc, inc, inc);
			Color col2 = Color(dec, dec, dec);

			Color rand1 = Color(random(0, 255), random(0, 255), random(0, 255));
			Color rand2 = Color(random(0, 255), random(0, 255), random(0, 255));

			if ((y & 0x01) != 1)
				framebuffer.SetPixel(x, y, rand1);
			else
				framebuffer.SetPixel(x, y, rand2);
		}
	}

	

	GreyscaleFrameBufferRenderer renderer = GreyscaleFrameBufferRenderer(framebuffer);

	renderer.TranslateFrameForDrawing(framebuffer);
	renderer.DrawFrame();

	char a;
	scanf_s("%c", &a, 1);
	return 0;
}