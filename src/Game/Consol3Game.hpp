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

#include <cstdint>
#include <memory>

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

		Camera camera;
		StaticMesh mesh;

	public:

		Consol3Game(Rasterizer& rasterizer);

		void HandleInput();
		void Update();
		void Render(int64_t delta);
	};
}

#endif