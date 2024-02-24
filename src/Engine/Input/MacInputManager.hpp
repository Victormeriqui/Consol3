#ifdef SYS_MAC
#ifndef MACINPUTMANAGER_HPP
#define MACINPUTMANAGER_HPP

#include "IInputManager.hpp"

#include <CoreGraphics/CGEvent.h>
#include <CoreGraphics/CGEventSource.h>
//##include <CoreFoundation/CFRuntime.h>
//#include <Foundation/Foundation.h>

#include <cstdint>
//#include <linux/input.h>

#define KEY_MAX		0777

namespace Engine
{
    namespace Input
    {

        class MacInputManager : public IInputManager
        {
        private:
            int keyboard_device_file;

            uint8_t keyboard_state[KEY_MAX / 8 + 1];

            CGKeyCode TranslateKey(Key key) const;

        public:
            MacInputManager();
            ~MacInputManager();

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