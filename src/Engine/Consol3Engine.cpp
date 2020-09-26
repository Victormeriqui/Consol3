#include "Consol3Engine.hpp"

#include "../Game/Consol3Game.hpp"
#include "../Display/FrameBuffer.hpp"
#include "Rendering/Rasterizer.hpp"

#include <chrono>
#include <cstdint>

#define UPDATE_STEP 10

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

	inline int64_t Consol3Engine::GetCurrentTime() const
	{
		high_resolution_clock::time_point now_time = high_resolution_clock::now();
		milliseconds time_span = duration_cast<milliseconds>(now_time - start_time);
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

	void Consol3Engine::UpdateWindowTitle(uint16_t frame_count)
	{
		renderer.ReportFPS(frame_count);
	}

	void Consol3Engine::RunLoop()
	{
		int64_t current_time = GetCurrentTime();
		int64_t last_time = current_time;
		int64_t delta = 0;
		int64_t accumulator = 0;
		int64_t accumulated_delta = 0;

		uint16_t frame_count = 0;

		while (running)
		{
			current_time = GetCurrentTime();
			delta = current_time - last_time;
		
			accumulated_delta += delta;

			// accumulated more than 1 second elapsed time
			if (accumulated_delta > 1000)
			{
				UpdateWindowTitle(frame_count);
				frame_count = 0;
				accumulated_delta = 0;
			}

			accumulator += delta;

			while (accumulator > 0)
			{
				game.HandleInput();
				game.Update();
				accumulator -= UPDATE_STEP;
			}

			DrawFrame(delta);
			frame_count++;
			last_time = current_time;
		}
	}

	inline void Consol3Engine::DrawFrame(int64_t delta)
	{
		framebuffer.ClearBuffer();

		game.Render(delta);

		renderer.TranslateFrameForDrawing(framebuffer);
		renderer.DrawFrame();
	}

}