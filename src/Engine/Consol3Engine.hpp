#ifndef CONSOL3ENGINE_HPP
#define CONSOL3ENGINE_HPP

#include "../Game/Consol3Game.hpp"
#include "../Display/FrameBuffer.hpp"
#include "../Display/IRenderer.hpp"
#include "../Display/ConsoleManager.hpp"
#include "../Engine/Rendering/Lighting/LightingSystem.hpp"
#include "Rendering/SceneRenderer.hpp"

#include <chrono>
#include <cstdint>
#include <memory>

namespace Engine
{
	using namespace std;
	using namespace std::chrono;
	using namespace Game;
	using namespace Display;
	using namespace Rendering;

	class Consol3Engine
	{
	private:
		std::shared_ptr<IRenderer> renderer;
		std::shared_ptr<LightingSystem> lighting_system;
		std::shared_ptr<SceneRenderer> scene_renderer;

		Consol3Game game;

		bool running;
		float delta;

		high_resolution_clock::time_point start_time;

		int64_t GetCurrentTime() const;

		void RunLoop();
		inline void DrawFrame(int64_t delta);

	public:
		Consol3Engine(std::shared_ptr<IRenderer> renderer);

		void Start();
		void Stop();

		[[nodiscard]] bool IsRunning() const;
	};
}

#endif