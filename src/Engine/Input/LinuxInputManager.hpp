#ifdef SYS_LINUX
#ifndef LINUXINPUTMANAGER_HPP
#define LINUXINPUTMANAGER_HPP

#include "IInputManager.hpp"

#include <cstdint>
#include <linux/input.h>

namespace Engine
{
    namespace Input
    {

        class LinuxInputManager : public IInputManager
        {
        private:
            int keyboard_device_file;

            uint8_t keyboard_state[KEY_MAX / 8 + 1];

            uint32_t TranslateKey(Key key) const;

        public:
            LinuxInputManager();
            ~LinuxInputManager();

            virtual void UpdateInputEvents() override;

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
#endif