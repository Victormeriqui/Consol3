#include "Consol3Engine.hpp"

#include "../Game/Consol3Game.hpp"
#include "../Display/FrameBuffer.hpp"
#include "Rendering/Rasterizer.hpp"

#include <chrono>

using namespace std;
using namespace std::chrono;
using namespace Game;
using namespace Display;
using namespace Rendering;

namespace Engine
{
	Consol3Engine::Consol3Engine(FrameBuffer& framebuffer) :
		framebuffer(framebuffer),
		rasterizer(Rasterizer(framebuffer)),
		game(Consol3Game(rasterizer)),
		renderer(GreyscaleFrameBufferRenderer(framebuffer)),
		running(false),
		delta(0)
	{
	}

	inline long long Consol3Engine::GetCurrentTime() const
	{
		high_resolution_clock::time_point now_time = high_resolution_clock::now();
		nanoseconds time_span = duration_cast<nanoseconds>(now_time - start_time);
		return time_span.count();
	}

	void Consol3Engine::Start()
	{
		if (running)
			return;

		running = true;

		start_time = std::chrono::high_resolution_clock::now();
		RunLoop();
	}

	void Consol3Engine::Stop()
	{
		if (!running)
			return;

		running = false;
	}

	bool Consol3Engine::IsRunning() const
	{
		return running;
	}

	void Consol3Engine::RunLoop()
	{
		long long last_time = GetCurrentTime();
		long long current_time = GetCurrentTime();
		long long elapsed_time = 0;
		while (running)
		{
			current_time = GetCurrentTime();
			elapsed_time = current_time - last_time;
			
			game.HandleInput();
			game.Update(elapsed_time);
			DrawFrame();

			last_time = current_time;

		}
	}

	bool added = false;
	inline void Consol3Engine::DrawFrame()
	{
		framebuffer.ClearBuffer();

		game.Render();

		renderer.TranslateFrameForDrawing(framebuffer);
		renderer.DrawFrame();
	}

}