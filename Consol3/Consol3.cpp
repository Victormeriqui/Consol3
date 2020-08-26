#include <iostream>
#include <cstdio>

#include "FrameBuffer.hpp"

using namespace Display;

int main()
{
	FrameBuffer framebuffer = FrameBuffer(5, 5);

	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			framebuffer.SetPixel(x, y, x+y);
		}
	}

	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			printf("%u ", framebuffer.GetPixel(x, y).GetHexValues());
		}
		printf("\n");
	}

	return 0;
}
