#ifndef CONSOL3GAME_HPP
#define CONSOL3GAME_HPP

#include "../Engine/Rendering/Rasterizer.hpp"
#include "../Engine/Rendering/Vertex.hpp"
#include "../Math/Vector3.hpp"
#include "../Engine/Rendering/Camera.hpp"
#include "../Engine/Rendering/Model.hpp"
#include "../Engine/Rendering/Transform.hpp"
#include "../Engine/Rendering/StaticMesh.hpp"
#include "../Engine/Rendering/Lighting/ILight.hpp"
#include "../Engine/Rendering/Lighting/DirectionalLight.hpp"
#include "../Engine/Rendering/Lighting/LightingSystem.hpp"
#include "../Engine/Rendering/Lighting/PointLight.hpp"
#include "ModelGenerator.hpp"

#include <cstdint>
#include <memory>
#include "ModelGenerator.hpp"

namespace Game
{
	using namespace Display;
	using namespace Engine;
	using namespace Engine::Rendering;
	using namespace Math;
	using namespace Lighting;

	class Consol3Game
	{
	private:
		Rasterizer& rasterizer;
		std::shared_ptr<LightingSystem> lighting_system;
		ModelGenerator model_generator;

		Camera camera;

		StaticMesh mesh;
		StaticMesh floor;

		std::shared_ptr<PointLight> point_light;
		StaticMesh plight_mesh;

	public:

		Consol3Game(Rasterizer& rasterizer);

		void HandleInput();
		void Update();
		void Render(int64_t delta);
	};
}

#endif