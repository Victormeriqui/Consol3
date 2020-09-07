#include <iostream>
#include <cstdio>

#include "FrameBuffer.hpp"
#include "GreyscaleFrameBufferRenderer.hpp"
#include "MathUtil.hpp"
#include "BarycentricRasterizer.hpp"
#include "Vertex.hpp"
#include "Vector3.hpp"

using namespace Engine::Math;
using namespace Display;
using namespace Engine::Rendering;

int random(int min, int max)
{
	return rand() % (max + 1 - min) + min;
}

int main()
{
	float width = 100;
	float height = 80;

	FrameBuffer framebuffer = FrameBuffer(width, height);
	framebuffer.ClearBuffer();

	BarycentricRasterizer rasterizer = BarycentricRasterizer(framebuffer);
	rasterizer.SetModelMatrix(Matrix4().SetIdentity());
	rasterizer.SetViewMatrix(Matrix4().SetIdentity());
	rasterizer.SetProjectionMatrix(Matrix4().SetIdentity());

	for (int i = 0; i < 10; i++)
	{
		Vertex v0 = Vertex(Vector3(10, 60, 0));
		Vertex v1 = Vertex(Vector3(80, 30, 0));
		Vertex v2 = Vertex(Vector3(20, 10, 0));
		

		rasterizer.RasterizeTriangle(v0, v1, v2, 0xFFFFFF);
	}
	GreyscaleFrameBufferRenderer renderer = GreyscaleFrameBufferRenderer(framebuffer);

	renderer.TranslateFrameForDrawing(framebuffer);
	renderer.DrawFrame();

	char a;
	scanf_s("%c", &a, 1);
	return 0;
}
