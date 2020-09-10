#include <iostream>
#include <cstdio>

#include "Display/FrameBuffer.hpp"
#include "Display/GreyscaleFrameBufferRenderer.hpp"
#include "Engine/Math/Util/MathUtil.hpp"
#include "Engine/Rendering/BarycentricRasterizer.hpp"
#include "Engine/Rendering/Vertex.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Rendering/Camera.hpp"
#include "Engine/Rendering/Model.hpp"
#include "Engine/Rendering/Transform.hpp"


using namespace Engine::Math;
using namespace Display;
using namespace Engine::Rendering;

int random(int min, int max)
{
	return rand() % (max + 1 - min) + min;
}

int main()
{
	uint16_t width = 100;
	uint16_t height = 80;

	FrameBuffer framebuffer = FrameBuffer(width, height);
	framebuffer.ClearBuffer();

	BarycentricRasterizer rasterizer = BarycentricRasterizer(framebuffer);

	Camera camera = Camera(100, 80, 0.001f, 1000.0f, 90.0f);

	Vertex v0 = Vertex(Vector3(-2, 2, 5));
	Vertex v1 = Vertex(Vector3(1, 1, 5));
	Vertex v2 = Vertex(Vector3(0, -1, 5));

	std::vector<Vertex> vertices{ v0, v1, v2 };
	std::vector<unsigned int> indices{0, 1, 2};

	Model model = Model(vertices, indices);
	Transform transform = Transform();

	rasterizer.SetProjectionMatrix(camera.GetProjectionMatrix());

	GreyscaleFrameBufferRenderer renderer = GreyscaleFrameBufferRenderer(framebuffer);

	int i = 1000;
	while (i > 0)
	{
		framebuffer.ClearBuffer();

		rasterizer.SetModelMatrix(transform.GetTransformationMatrix());

		rasterizer.SetViewMatrix(camera.GetViewMatrix());
		for (uint16_t i = 0; i < indices.size(); i += 3)
		{
			Vertex v0 = vertices[indices[i]];
			Vertex v1 = vertices[indices[i + 1]];
			Vertex v2 = vertices[indices[i + 2]];

			rasterizer.RasterizeTriangle(v0, v1, v2, 0xFFFFFF);
		}

		renderer.TranslateFrameForDrawing(framebuffer);
		renderer.DrawFrame();
		camera.Move(Vector3(0, 1, 0), 0.001f);
		i--;
	}

	//char a;
	//scanf_s("%c", &a, 1);
	return 0;
}
