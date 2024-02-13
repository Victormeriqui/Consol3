#ifndef CONSOL3ENGINE_HPP
#define CONSOL3ENGINE_HPP

#include "Display/FrameBuffer.hpp"
#include "Display/IFrameDrawer.hpp"
#include "Engine/Rendering/RasterSceneRenderer.hpp"
#include "Game/IGame.hpp"
#include "Input/IInputManager.hpp"

#include <chrono>
#include <cstdint>
#include <memory>
#include <vector>

namespace Engine
{
    using namespace std;
    using namespace std::chrono;
    using namespace Game;
    using namespace Display;
    using namespace Rendering;
    using namespace Input;

    class Consol3Engine
    {
    private:
        std::vector<std::shared_ptr<IFrameDrawer>> frame_drawers;
        uint8_t cur_frame_drawer_index = 0;

        std::shared_ptr<IInputManager> input_manager;

        std::unique_ptr<IGame> game;

        bool running;
        float delta;
        bool changed_frame_drawer = false;

        high_resolution_clock::time_point start_time;

        int64_t GetCurrentTime() const;

        void RunLoop();
        inline void DrawFrame(int64_t delta);

    public:
        Consol3Engine(std::shared_ptr<IInputManager> input_manager);

        void RegisterFrameDrawer(std::shared_ptr<IFrameDrawer> frame_drawer);

        void HandleFrameDrawerChangeInput();

        void Start();
        void Stop();

        [[nodiscard]] bool IsRunning() const;
    };
}

#endif
