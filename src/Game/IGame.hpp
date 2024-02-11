#ifndef IGAME_HPP
#define IGAME_HPP

#include <chrono>
#include <cstdint>

namespace Game
{
    class IGame
    {
    protected:
        IGame()
        {
        }

        virtual void LoadResources() = 0;

    public:
        virtual void HandleInput()                              = 0;
        virtual void Update()                                   = 0;
        virtual std::chrono::milliseconds Render(int64_t delta) = 0;
        virtual std::string GetDesiredWindowTitle() const       = 0;
    };
}

#endif
