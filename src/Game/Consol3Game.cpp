#include "Consol3Game.hpp"

#include "../Math/Util/MathUtil.hpp"
#include "MouseInput.hpp"

// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>
#include <cstdint>

namespace Game
{
	using namespace Display;
	using namespace Engine;
	using namespace Engine::Rendering;
	using namespace Math;

	void Consol3Game::LoadResources()
	{
		resource_manager->LoadModel("res/monkey.obj", NormalGenerationOptions::GENERATE_DISABLED);
		resource_manager->LoadModel("res/cube.obj", NormalGenerationOptions::GENERATE_DISABLED);
		resource_manager->LoadModel("plane50", model_generator.GeneratePlane(50, 50));

		resource_manager->LoadTexture("res/text.bmp", TextureLoadingOptions::DEFAULT);
	}

	float rot = 1.33f;
	Consol3Game::Consol3Game(std::shared_ptr<SceneRenderer> scene_renderer,
							 std::shared_ptr<ResourceManager> resource_manager,
							 std::shared_ptr<LightingSystem> lighting_system) :
		scene_renderer(std::move(scene_renderer)),
		resource_manager(std::move(resource_manager)),
		lighting_system(std::move(lighting_system)),
		camera(std::make_shared<Camera>(200, 200, 0.001f, 100.0f, 90.0f))
	{
		LoadResources();

		camera->SetPosition(Vector3(0, 0.1f, -1.155f));
		this->scene_renderer->SetCamera(camera);

		mesh = StaticMesh("res/monkey.obj", Vector3(0, 0, 0), RGBColor(255, 255, 255));
		mesh.SetScale(Vector3(1, 1, 1));
		//		mesh.SetRotation(Angle(0, 1.33f, 0));
		plight_mesh = StaticMesh("res/cube.obj", Vector3(-2, 0, 0), RGBColor(255, 255, 255));

		floor = StaticMesh("plane50", Vector3(0, 0, 0), RGBColor(255, 255, 255));
		floor.SetScale(Vector3(12, 12, 12));
		floor.SetPosition(Vector3(-6, -2, -6));

		dir_light = std::make_shared<DirectionalLight>(Vector3(-1, -0.5f, 0));
		dir_light->SetIntensity(0.5f);

		point_light = std::make_shared<PointLight>(Vector3(-2, 0, 0));
		point_light->SetRange(15.0f);

		spot_light = std::make_shared<SpotLight>(Vector3(0, 0.1f, -3.0f), Vector3(0, 0, 1));
		spot_light->SetRange(15.0f);
		spot_light->SetAngle(20.0f);
		spot_light->SetIntensity(6.0f);

		spot_light2 = std::make_shared<SpotLight>(Vector3(0, 0.1f, -2.0f), Vector3(0, 0, 1));
		spot_light2->SetRange(15.0f);
		spot_light2->SetAngle(20.0f);
		spot_light2->SetIntensity(6.0f);

		this->lighting_system->SetAmbientLight(0.02f);
		// lighting_system->AddLight(dir_light);
		// lighting_system->AddLight(point_light);
		this->lighting_system->AddLight(spot_light);
		//	lighting_system->AddLight(spot_light2);

		plight_mesh.SetScale(Vector3(0.1f, 0.1f, 0.1f));
	}

	float mov_speed = 0.05f;
	bool shifting	= false;

	void Consol3Game::HandleInput()
	{
		if (GetKeyState(VK_CAPITAL))
		{
			Vector2 mouse_dist_center = MouseInput::GetMouseDistanceToCenter();

			if (mouse_dist_center.x != 0)
				camera->RotateYaw(mouse_dist_center.x * 0.1f);
			if (mouse_dist_center.y != 0)
				camera->RotatePitch(mouse_dist_center.y * 0.1f);

			MouseInput::SetMousePositionToCenter();
		}

		if (GetKeyState(VK_SPACE) & 0x8000)
			camera->MoveY(mov_speed);

		if (GetKeyState(VK_CONTROL) & 0x8000)
			camera->MoveY(-mov_speed);

		if (GetKeyState(0x41) & 0x8000)
			camera->MoveX(-mov_speed);

		if (GetKeyState(0x44) & 0x8000)
			camera->MoveX(mov_speed);

		if (GetKeyState(0x53) & 0x8000)
			camera->MoveZ(-mov_speed);

		if (GetKeyState(0x57) & 0x8000)
			camera->MoveZ(mov_speed);

		if (GetKeyState(VK_LEFT) & 0x8000)
			camera->RotateYaw(-1);

		if (GetKeyState(VK_RIGHT) & 0x8000)
			camera->RotateYaw(1);

		if (GetKeyState(VK_UP) & 0x8000)
			camera->RotatePitch(-1);

		if (GetKeyState(VK_DOWN) & 0x8000)
			camera->RotatePitch(1);

		if (GetKeyState(VK_NUMPAD1) & 0x8000)
		{
			mesh.SetRotation(Angle(0, rot, 0));
			rot += 0.01f;
		}

		if (GetKeyState(VK_SHIFT) & 0x8000)
		{
			shifting  = true;
			mov_speed = 0.001f;
		}
		else if (shifting)
		{
			shifting  = false;
			mov_speed = 0.05f;
		}

		if (GetKeyState(VK_MBUTTON) & 0X8000)
			dir_light->SetDirection(camera->GetLookDirection());

		if (GetKeyState(VK_RBUTTON) & 0X8000)
		{
			spot_light->SetPosition(camera->GetPosition());
			spot_light->SetDirection(camera->GetLookDirection());
		}
		if (GetKeyState(VK_LBUTTON) & 0X8000)
		{
			spot_light2->SetPosition(camera->GetPosition());
			spot_light2->SetDirection(camera->GetLookDirection());
		}
	}

	float i = 0;
	void Consol3Game::Update()
	{
		point_light->SetPosition(Vector3(std::sin(i) * 2, 0.5, std::cos(i) * 2));
		plight_mesh.SetPosition(Vector3(std::sin(i) * 2, 0.5, std::cos(i) * 2));

		i += 0.01f;
	}

	std::chrono::milliseconds Consol3Game::Render(int64_t delta)
	{
		auto time = std::chrono::high_resolution_clock::now();

		scene_renderer->DrawShadedMesh(floor);
		scene_renderer->DrawShadedMesh(mesh);

		scene_renderer->RenderScene();

		for (int y = 0; y < 200; y++)
		{
			for (int x = 0; x < 200; x++)
			{
				float z		= dir_light->GetLightDepthBuffer().value().get().GetValue(x, y);
				uint16_t nx = Util::LerpCast<uint16_t>(x / 200.0f, 0, 50);
				uint16_t ny = Util::LerpCast<uint16_t>(y / 200.0f, 0, 50);
				scene_renderer->DrawPixel(nx, ny, HSVColor(0, 0, Util::Lerp(z, 0, 1)));
			}
		}

		for (int y = 0; y < 200; y++)
		{
			for (int x = 0; x < 200; x++)
			{
				float z		= spot_light->GetLightDepthBuffer().value().get().GetValue(x, y);
				uint16_t nx = Util::LerpCast<uint16_t>(x / 200.0f, 0, 50);
				uint16_t ny = Util::LerpCast<uint16_t>(y / 200.0f, 0, 50);
				scene_renderer->DrawPixel(50 + nx, ny, HSVColor(0, 0, Util::Lerp(z, 0, 1)));
			}
		}

		/*
		for (int y = 0; y < 200; y++)
		{
			for (int x = 0; x < 200; x++)
			{
				float z = spot_light2->GetLightDepthBuffer().value().get().GetValue(x, y);
				uint16_t nx = Util::LerpCast<uint16_t>(x / 200.0f, 0, 50);
				uint16_t ny = Util::LerpCast<uint16_t>(y / 200.0f, 0, 50);
				scene_renderer->DrawPixel(150+nx, ny, HSVColor(0, 0, Util::Lerp(z, 0, 1)));
			}
		}*/

		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - time);
	}
}
