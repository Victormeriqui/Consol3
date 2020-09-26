#ifndef CONSOL3ENGINE_HPP
#define CONSOL3ENGINE_HPP

#include "../Game/Consol3Game.hpp"
#include "../Display/FrameBuffer.hpp"
#include "../Display/GreyscaleFrameBufferRenderer.hpp"
#include "Rendering/Rasterizer.hpp"

#include <chrono>
#include <cstdint>

using namespace std;
using namespace std::chrono;
using namespace Game;
using namespace Display;
using namespace Rendering;

namespace Engine
{
	class Consol3Engine
	{
	private:
		FrameBuffer& framebuffer;
		Rasterizer rasterizer;
		GreyscaleFrameBufferRenderer renderer;

		Consol3Game game;

		bool running;
		float delta;
		
		high_resolution_clock::time_point start_time;

		int64_t GetCurrentTime() const;

		void RunLoop();
		inline void DrawFrame(int64_t delta);
		void UpdateWindowTitle(uint16_t frame_count);

	public:
		Consol3Engine(FrameBuffer& framebuffer);

		void Start();
		void Stop();

		[[nodiscard]] bool IsRunning() const;
	};
}

#endif