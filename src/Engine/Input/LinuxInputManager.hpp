#ifndef LINUXINPUTMANAGER_HPP
#define LINUXINPUTMANAGER_HPP

#include "IInputManager.hpp"

#include <cstdint>

namespace Engine
{
    namespace Input
    {

        class LinuxInputManager : public IInputManager
        {
        private:
            uint32_t TranslateKey(Key key) const;

        public:
            LinuxInputManager();

            Vector2I GetMousePosition() const override;
            void SetMousePosition(const Vector2I& position) override;

            Vector2 GetMouseDistanceToCenter() const override;
            void SetMousePositionToCenter() override;

            bool IsKeyPressed(Key key) const override;
            bool IsKeyReleased(Key key) const override;
            bool IsKeyHeld(Key key) const override;
        };
    }
}

#endif