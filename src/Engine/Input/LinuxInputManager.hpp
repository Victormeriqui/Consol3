#ifndef LINUXINPUTMANAGER_HPP
#define LINUXINPUTMANAGER_HPP

#include "IInputManager.hpp"

#include <cstdint>

namespace Engine {
namespace Input {

    class LinuxInputManager : public IInputManager {
    private:
        uint32_t TranslateKey(Key key) const;

    public:
        LinuxInputManager();

        Point2 GetMousePosition() const override;
        void SetMousePosition(const Point2& position) override;

        Vector2 GetMouseDistanceToCenter() const override;
        void SetMousePositionToCenter() override;

        bool IsKeyPressed(Key key) const override;
        bool IsKeyReleased(Key key) const override;
        bool IsKeyHeld(Key key) const override;
    };
}
}

#endif