#ifndef IINPUTMANAGER_HPP
#define IINPUTMANAGER_HPP

#include "Math/Vector2I.hpp"

using namespace Math;

namespace Engine
{
    namespace Input
    {

        enum class Key
        {
            LEFT_ARROW,
            RIGHT_ARROW,
            UP_ARROW,
            DOWN_ARROW,
            CAPITAL,
            SPACE,
            LCONTROL,
            LSHIFT,
            Q,
            W,
            E,
            A,
            S,
            D,
            P,
            R,
            T,
            G,
            F,
            H,
            TAB,
            MOUSE1,
            MOUSE2,
            MOUSE3,
            MOUSE4,
            MOUSE5,
            N1,
            N2,
            N3,
            N4,
            N5,
            N6,
            N7,
            N8,
            N9,
            N0,
            PAGE_UP,
            PAGE_DOWN,
        };

        class IInputManager
        {
        public:
            virtual ~IInputManager() = default;

            virtual void UpdateInputEvents() = 0;

            [[nodiscard]] virtual Vector2I GetMousePosition() const = 0;
            virtual void SetMousePosition(const Vector2I& position) = 0;

            [[nodiscard]] virtual Vector2 GetMouseDistanceToCenter() const = 0;
            virtual void SetMousePositionToCenter()                        = 0;

            [[nodiscard]] virtual bool IsKeyPressed(Key key) const  = 0;
            [[nodiscard]] virtual bool IsKeyReleased(Key key) const = 0;
            [[nodiscard]] virtual bool IsKeyHeld(Key key) const     = 0;
        };
    }
}

#endif