#ifndef CONSOL3GAME_HPP
#define CONSOL3GAME_HPP

#include "../Engine/Rendering/AnimatedMesh.hpp"
#include "../Engine/Rendering/Camera.hpp"
#include "../Engine/Rendering/Lighting/DirectionalLight.hpp"
#include "../Engine/Rendering/Lighting/ILight.hpp"
#include "../Engine/Rendering/Lighting/LightingSystem.hpp"
#include "../Engine/Rendering/Lighting/PointLight.hpp"
#include "../Engine/Rendering/Lighting/SpotLight.hpp"
#include "../Engine/Rendering/Rasterizer.hpp"
#include "../Engine/Rendering/SceneRenderer.hpp"
#include "../Engine/Rendering/StaticMesh.hpp"
#include "../Engine/Rendering/Transform.hpp"
#include "../Engine/Rendering/Vertex.hpp"
#include "../Engine/Resources/ResourceManager.hpp"
#include "../Math/Vector3.hpp"
#include "ModelGenerator.hpp"

#include <chrono>
#include <cstdint>
#include <memory>

namespace Game
{
	using namespace Display;
	using namespace Engine;
	using namespace Engine::Rendering;
	using namespace Math;
	using namespace Lighting;
	using namespace Resources;

	class Consol3Game
	{
	private:
		std::shared_ptr<SceneRenderer> scene_renderer;
		std::shared_ptr<LightingSystem> lighting_system;
		ModelGenerator model_generator;

		std::shared_ptr<ResourceManager> resource_manager;

		std::shared_ptr<Camera> camera;

		StaticMesh mesh;

		AnimatedMesh anim_mesh;
		StaticMesh floor;

		std::shared_ptr<DirectionalLight> dir_light;
		std::shared_ptr<PointLight> point_light;
		std::shared_ptr<SpotLight> spot_light;
		StaticMesh plight_mesh;

		void LoadResources();

	public:
		Consol3Game(std::shared_ptr<SceneRenderer> scene_renderer,
					std::shared_ptr<ResourceManager> resource_manager,
					std::shared_ptr<LightingSystem> lighting_system);

		void HandleInput();
		void Update();
		std::chrono::microseconds Render(int64_t delta);
	};
}

#endif
