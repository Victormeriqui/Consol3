#ifndef IGAME_HPP
#define IGAME_HPP

#include "../Display/IFrameDrawer.hpp"

#include <chrono>
#include <cstdint>
#include <memory>

namespace Game
{
    class IGame
    {
    protected:
        virtual void LoadResources() = 0;

    public:
        virtual void SetFrameDrawer(std::shared_ptr<Display::IFrameDrawer> frame_drawer) = 0;
        virtual void HandleInput()                                                       = 0;
        virtual void Update()                                                            = 0;
        virtual std::chrono::milliseconds Render(int64_t delta)                          = 0;
        virtual std::string GetDesiredWindowTitle() const                                = 0;
    };
}

#endif
