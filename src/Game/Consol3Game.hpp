#ifndef CONSOL3GAME_HPP
#define CONSOL3GAME_HPP

#include "../Display/FrameBuffer.hpp"
#include "../Engine/Rendering/Rasterizer.hpp"
#include "../Engine/Rendering/Vertex.hpp"
#include "../Engine/Math/Vector3.hpp"
#include "../Engine/Rendering/Camera.hpp"
#include "../Engine/Rendering/Model.hpp"
#include "../Engine/Rendering/Transform.hpp"

#include <cstdint>

using namespace Display;
using namespace Engine;
using namespace Engine::Rendering;
using namespace Engine::Math;

namespace Game
{
	class Consol3Game
	{
	private:
		Rasterizer& rasterizer;

		Camera camera;
		Model model;
		Transform transform;

	public:

		Consol3Game(Rasterizer& rasterizer);

		void HandleInput();
		void Update();
		void Render(int64_t delta);
	};
}

#endif