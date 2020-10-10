#include "Consol3Game.hpp"

#include "../Display/FrameBuffer.hpp"
#include "../Engine/Rendering/Rasterizer.hpp"
#include "../Engine/Rendering/Vertex.hpp"
#include "../Math/Vector3.hpp"
#include "../Engine/Rendering/Model.hpp"
#include "../Display/RGBColor.hpp"
#include "../Engine/Rendering/Transform.hpp"
#include "../Math/Point2.hpp"
#include "../Math/Vector2.hpp"
#include "MouseInput.hpp"
#include "../Engine/Rendering/StaticMesh.hpp"

#include <vector>
// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>
#include <cstdint>

using namespace Display;
using namespace Engine;
using namespace Engine::Rendering;
using namespace Math;

namespace Game
{
	Consol3Game::Consol3Game(Rasterizer& rasterizer) : rasterizer(rasterizer)
	{
		camera = Camera(200, 200, 0.01f, 100.0f, 90.0f);
		camera.SetPosition(Vector3(0, 0.1f, -0.2f));

		rasterizer.SetProjectionMatrix(camera.GetProjectionMatrix());

		mesh = StaticMesh(Model("res/bunny.obj"), Vector3(0, 0, 0), RGBColor(255, 255, 255));
	}

	float mov_speed = 0.01f;
	bool shifting = false;
	float rot = 0;
	void Consol3Game::HandleInput()
	{
		if (GetKeyState(VK_CAPITAL))
		{
			Vector2 mouse_dist_center = MouseInput::GetMouseDistanceToCenter();

			if (mouse_dist_center.x != 0)
				camera.RotateYaw(mouse_dist_center.x * 0.1f);
			if (mouse_dist_center.y != 0)
				camera.RotatePitch(mouse_dist_center.y * 0.1f);

			MouseInput::SetMousePositionToCenter();
		}

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
			mesh.SetRotation(Angle(0, rot, 0));
			rot += 0.05f;
		}

		if (GetKeyState(VK_SHIFT) & 0x8000)
		{
			shifting = true;
			mov_speed = 0.1f;
		}
		else if (shifting)
		{
			shifting = false;
			mov_speed = 0.01f;
		}

	}

	void Consol3Game::Update()
	{

	}

	void Consol3Game::Render(int64_t delta)
	{
		rasterizer.SetViewMatrix(camera.GetViewMatrix());

		mesh.DrawMesh(rasterizer);
	}

}