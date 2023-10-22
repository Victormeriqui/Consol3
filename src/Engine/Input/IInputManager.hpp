#ifndef IINPUTMANAGER_HPP
#define IINPUTMANAGER_HPP

#include "Math/Point2.hpp"
#include "Math/Vector2.hpp"

using namespace Math;

namespace Engine {
namespace Input {

    enum class Key {
        CAPITAL,
        SPACE,
        LCONTROL,
        LSHIFT,
        W,
        A,
        S,
        D,
        TAB,
        MOUSE1,
        MOUSE2,
        MOUSE3,
        MOUSE4,
        MOUSE5
    };

    class IInputManager {
    public:
        virtual ~IInputManager() = default;

        [[nodiscard]] virtual Point2 GetMousePosition() const = 0;
        virtual void SetMousePosition(const Point2& position) = 0;

        [[nodiscard]] virtual Vector2 GetMouseDistanceToCenter() const = 0;
        virtual void SetMousePositionToCenter() = 0;

        [[nodiscard]] virtual bool IsKeyPressed(Key key) const = 0;
        [[nodiscard]] virtual bool IsKeyReleased(Key key) const = 0;
        [[nodiscard]] virtual bool IsKeyHeld(Key key) const = 0;
    };
}
}

#endif