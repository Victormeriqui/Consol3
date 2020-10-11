#ifndef CONSOL3ENGINE_HPP
#define CONSOL3ENGINE_HPP

#include "../Game/Consol3Game.hpp"
#include "../Display/AbstractFrameBuffer.hpp"
#include "Rendering/Rasterizer.hpp"
#include "../Display/ConsoleManager.hpp"

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
		// TODO make a new component that holds the framebuffer so the engine doesnt need to specify the framebuffer type
		std::shared_ptr<AbstractFrameBuffer<CHAR_INFO>> framebuffer;

		Rasterizer rasterizer;
		ConsoleManager console_manager;

		Consol3Game game;

		bool running;
		float delta;

		high_resolution_clock::time_point start_time;

		int64_t GetCurrentTime() const;

		void RunLoop();
		inline void DrawFrame(int64_t delta);

	public:
		Consol3Engine(std::shared_ptr<AbstractFrameBuffer<CHAR_INFO>> framebuffer);

		void Start();
		void Stop();

		[[nodiscard]] bool IsRunning() const;
	};
}

#endif