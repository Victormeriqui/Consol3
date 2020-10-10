#ifndef CONSOL3ENGINE_HPP
#define CONSOL3ENGINE_HPP

#include "../Game/Consol3Game.hpp"
#include "../Display/FrameBuffer.hpp"
#include "../Display/IPixelTranslator.hpp"
#include "../Display/DitheredPixelTranslator.hpp"
#include "../Display/GreyscalePixelTranslator.hpp"
#include "Rendering/Rasterizer.hpp"
#include "../Display/ConsoleManager.hpp"

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
		const IPixelTranslator& pixel_translator;

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
		Consol3Engine(FrameBuffer& framebuffer, const IPixelTranslator& pixel_translator);

		void Start();
		void Stop();

		[[nodiscard]] bool IsRunning() const;
	};
}

#endif