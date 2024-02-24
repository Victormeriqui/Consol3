#ifdef SYS_MAC
#include "MacInputManager.hpp"
#include "Display/Windows/WindowsStructsForLinux.hpp"

//#include <CoreGraphics/CGWindow.h>
#include <CoreGraphics/CGEvent.h>
#include <CoreGraphics/CGEventSource.h>

#include <algorithm>
#include <fcntl.h>
#include <fstream>
#include <sys/ioctl.h>
#include <unistd.h>

namespace Engine
{
    namespace Input
    {

        MacInputManager::MacInputManager()
        {
            //keyboard_device_file = open("/dev/input/by-path/platform-i8042-serio-0-event-kbd", O_RDONLY);
        }

        MacInputManager::~MacInputManager()
        {
            //close(keyboard_device_file);
        }

        void MacInputManager::UpdateInputEvents()
        {
            // clear state
            std::fill(keyboard_state, keyboard_state + KEY_MAX / 8 + 1, 0);

            //ioctl(keyboard_device_file, EVIOCGKEY(sizeof(keyboard_state)), keyboard_state);
        }

        CGKeyCode MacInputManager::TranslateKey(Key key) const
        {
            /*
            switch (key)
            {
            case Key::LEFT_ARROW:
                return KEY_LEFT;
            case Key::RIGHT_ARROW:
                return KEY_RIGHT;
            case Key::UP_ARROW:
                return KEY_UP;
            case Key::DOWN_ARROW:
                return KEY_DOWN;
            case Key::CAPITAL:
                return KEY_CAPSLOCK;
            case Key::SPACE:
                return KEY_SPACE;
            case Key::LCONTROL:
                return KEY_LEFTCTRL;
            case Key::LSHIFT:
                return KEY_LEFTSHIFT;
            case Key::Q:
                return KEY_Q;
            case Key::W:
                return KEY_W;
            case Key::E:
                return KEY_E;
            case Key::A:
                return KEY_A;
            case Key::S:
                return KEY_S;
            case Key::D:
                return KEY_D;
            case Key::P:
                return KEY_P;
            case Key::R:
                return KEY_R;
            case Key::T:
                return KEY_T;
            case Key::F:
                return KEY_F;
            case Key::G:
                return KEY_G;
            case Key::H:
                return KEY_H;
            case Key::TAB:
                return KEY_TAB;
            case Key::MOUSE1:
                return BTN_LEFT;
            case Key::MOUSE2:
                return BTN_RIGHT;
            case Key::MOUSE3:
                return BTN_MIDDLE;
            case Key::MOUSE4:
                return BTN_FORWARD;
            case Key::MOUSE5:
                return BTN_BACK;
            case Key::N1:
                return KEY_1;
            case Key::N2:
                return KEY_2;
            case Key::N3:
                return KEY_3;
            case Key::N4:
                return KEY_4;
            case Key::N5:
                return KEY_5;
            case Key::N6:
                return KEY_6;
            case Key::N7:
                return KEY_7;
            case Key::N8:
                return KEY_8;
            case Key::N9:
                return KEY_9;
            case Key::N0:
                return KEY_0;
            case Key::PAGE_UP:
                return KEY_PAGEUP;
            case Key::PAGE_DOWN:
                return KEY_PAGEDOWN;
            }*/
            return 0;
        }

        Vector2I MacInputManager::GetMousePosition() const
        {
            CGEventRef event = CGEventCreate(nil);
            CGPoint coord = CGEventGetLocation(event);
            CFRelease(event);
            return Vector2I((uint16_t)coord.x, (uint16_t)coord.y);
        }

        void MacInputManager::SetMousePosition(const Vector2I& position)
        {
            CGWarpMouseCursorPosition(CGPointMake(position.x, position.y));
        }

        Vector2 MacInputManager::GetMouseDistanceToCenter() const
        {
            // RECT window = {0, 0, 0, 0};
            // GetWindowRect(GetConsoleWindow(), &window);

            // Vector2I center = Vector2I((uint16_t)(window.left + ((window.right - window.left) / 2)), (uint16_t)(window.top + ((window.bottom - window.top) / 2)));

            // Vector2I mouse_pos = GetMousePosition();

            // return Vector2((float)mouse_pos.x - center.x, (float)mouse_pos.y - center.y);
            return Vector2(0, 0);
        }

        void MacInputManager::SetMousePositionToCenter()
        {
            // RECT window = {0, 0, 0, 0};
            // GetWindowRect(GetConsoleWindow(), &window);

            // Vector2I center = Vector2I((uint16_t)(window.left + ((window.right - window.left) / 2)), (uint16_t)(window.top + ((window.bottom - window.top) / 2)));

            // SetMousePosition(center);
        }

        bool MacInputManager::IsKeyPressed(Key key) const
        {
            return CGEventSourceKeyState(kCGEventSourceStateCombinedSessionState, TranslateKey(key));
        }

        bool MacInputManager::IsKeyReleased(Key key) const
        {
            return (CGEventSourceKeyState(kCGEventSourceStateCombinedSessionState, TranslateKey(key)) & 0x8000) == 0;
        }

        bool MacInputManager::IsKeyHeld(Key key) const
        {
            uint32_t mac_key = TranslateKey(key);
            uint16_t key_index = mac_key / 8;
            uint16_t held_mask = 1 << (mac_key % 8);

            return keyboard_state[key_index] & held_mask;
        }

    }
}
#endif