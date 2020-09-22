#include "Consol3Game.hpp"

#include "../Display/FrameBuffer.hpp"
#include "../Engine/Rendering/Rasterizer.hpp"
#include "../Engine/Rendering/Vertex.hpp"
#include "../Engine/Math/Vector3.hpp"
#include "../Engine/Rendering/Model.hpp"
#include "../Display/Color.hpp"
#include "../Engine/Rendering/Transform.hpp"

#include <vector>
#include <Windows.h>

using namespace Display;
using namespace Engine;
using namespace Engine::Rendering;
using namespace Engine::Math;

namespace Game
{
	Consol3Game::Consol3Game(Rasterizer& rasterizer) : rasterizer(rasterizer)
	{
		camera = Camera(200, 200, 0.01f, 100.0f, 90.0f);
		camera.SetPosition(Vector3(0, 0.1f, -0.2f));

		rasterizer.SetProjectionMatrix(camera.GetProjectionMatrix());

		Vertex v0 = Vertex(Vector3(-2, 2, 5));
		Vertex v1 = Vertex(Vector3(1, 1, 5));
		Vertex v2 = Vertex(Vector3(0, -1, 5));
		
		std::vector<Vertex> vertices{ v0, v1, v2 };
		std::vector<unsigned int> indices{ 0, 1, 2 };

		model = Model("res/bunny.obj");
		//model = Model(vertices, indices);
		transform = Transform();
	}

	float mov_speed = 0.02f;
	bool shifting = false;
	float rot = 0;
	void Consol3Game::HandleInput()
	{
		if (GetKeyState(VK_SPACE) & 0x8000)
			camera.MoveY(mov_speed);

		if (GetKeyState(VK_CONTROL) & 0x8000)
			camera.MoveY(-mov_speed);

		if (GetKeyState(0x41) & 0x8000)
			camera.MoveX(-mov_speed);

		if (GetKeyState(0x44) & 0x8000)
			camera.MoveX(mov_speed);

		if (GetKeyState(0x53) & 0x8000)
			camera.MoveZ(-mov_speed);

		if (GetKeyState(0x57) & 0x8000)
			camera.MoveZ(mov_speed);

		if (GetKeyState(VK_LEFT) & 0x8000)
			camera.RotateYaw(-1);

		if (GetKeyState(VK_RIGHT) & 0x8000)
			camera.RotateYaw(1);

		if (GetKeyState(VK_UP) & 0x8000)
			camera.RotatePitch(-1);

		if (GetKeyState(VK_DOWN) & 0x8000)
			camera.RotatePitch(1);

		if (GetKeyState(VK_NUMPAD1) & 0x8000)
		{
			transform.SetRotation(Quaternion(Vector3(0, 1, 0), rot));
			rot += 0.05f;
		}

		if (GetKeyState(VK_SHIFT) & 0x8000)
		{
			shifting = true;
			mov_speed = 0.2f;
		}
		else if (shifting)
		{
			shifting = false;
			mov_speed = 0.02f;
		}
		
	}

	void Consol3Game::Update(long long delta)
	{

	}

	void Consol3Game::Render()
	{
		rasterizer.SetViewMatrix(camera.GetViewMatrix());

		model.DrawModel(transform, rasterizer);
	}

}