#include "Consol3Game.hpp"

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
#include "../Engine/Rendering/Lighting/ILight.hpp"
#include "../Engine/Rendering/Lighting/DirectionalLight.hpp"
#include "../Engine/Rendering/Lighting/PointLight.hpp"
#include "../Engine/Rendering/Lighting/LightingSystem.hpp"

#include <vector>
// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>
#include <cstdint>
#include <memory>
#include <chrono>

namespace Game
{
	using namespace Display;
	using namespace Engine;
	using namespace Engine::Rendering;
	using namespace Math;

	Consol3Game::Consol3Game(Rasterizer& rasterizer) :
		rasterizer(rasterizer),
		camera(Camera(200, 200, 0.001f, 100.0f, 90.0f)),
		lighting_system(std::make_shared<LightingSystem>())
	{
		camera.SetPosition(Vector3(0, 0.1f, -2.0f));

		rasterizer.SetProjectionMatrix(camera.GetProjectionMatrix());

		mesh = StaticMesh(Model("res/bunny.obj"), Texture("res/text.bmp"), Vector3(0, 0, 0), RGBColor(255, 255, 255));
		mesh.SetScale(Vector3(1, 1, 1));
		plight_mesh = StaticMesh(Model("res/cube.obj"), Vector3(-2, 0, 0), RGBColor(255, 255, 255));
	
		floor = StaticMesh(model_generator.GeneratePlane(50, 50), Vector3(0, 0 ,0), RGBColor(255, 255, 255));
		floor.SetScale(Vector3(6, 6, 6));
		floor.SetPosition(Vector3(-3, -2, -3));

		std::shared_ptr<DirectionalLight> dir_light = std::make_shared<DirectionalLight>(Vector3(-1, -0.5f, 0));
		dir_light->SetIntensity(1.0f);
		point_light = std::make_shared<PointLight>(Vector3(-2, 0, 0));
		point_light->SetRange(15.0f);

		spot_light = std::make_shared<SpotLight>(Vector3(0, 0.1f, -2.0f), Vector3(0, 0, 1));
		spot_light->SetRange(15.0f);
		spot_light->SetAngle(15);
		spot_light->SetIntensity(5.0f);


		lighting_system->AddLight(dir_light);
		//lighting_system->AddLight(point_light);
		//lighting_system->AddLight(spot_light);

		plight_mesh.SetScale(Vector3(0.1f, 0.1f, 0.1f));
	}

	float mov_speed = 0.05f;
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
			rot += 0.001f;
		}

		if (GetKeyState(VK_SHIFT) & 0x8000)
		{
			shifting = true;
			mov_speed = 0.001f;
		}
		else if (shifting)
		{
			shifting = false;
			mov_speed = 0.05f;
		}
	}

	float i = 0;
	void Consol3Game::Update()
	{
		point_light->SetPosition(Vector3(std::sin(i)*2, 0, std::cos(i)*2));
		plight_mesh.SetPosition(Vector3(std::sin(i)*2, 0, std::cos(i)*2));
		spot_light->SetPosition(camera.GetPosition());
		spot_light->SetDirection(camera.GetLookDirection());
		i += 0.01f;
	}

	std::chrono::milliseconds Consol3Game::Render(int64_t delta)
	{
		camera.GetDepthBuffer().ClearBuffer();

		rasterizer.SetViewMatrix(camera.GetViewMatrix());

		auto time = std::chrono::high_resolution_clock::now();
		mesh.DrawMesh(camera, rasterizer);
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - time);
		

		//floor.DrawMesh(camera, lighting_system, rasterizer);

	//	plight_mesh.DrawMesh(camera, lighting_system, rasterizer);
	}
}