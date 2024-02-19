#ifdef SYS_LINUX
#include "LinuxInputManager.hpp"

#include <algorithm>
#include <fcntl.h>
#include <fstream>
#include <sys/ioctl.h>
#include <unistd.h>

namespace Engine
{
    namespace Input
    {

        LinuxInputManager::LinuxInputManager()
        {
            keyboard_device_file = open("/dev/input/by-path/platform-i8042-serio-0-event-kbd", O_RDONLY);
        }

        LinuxInputManager::~LinuxInputManager()
        {
            close(keyboard_device_file);
        }

        void LinuxInputManager::UpdateInputEvents()
        {
            // clear state
            std::fill(keyboard_state, keyboard_state + KEY_MAX / 8 + 1, 0);

            ioctl(keyboard_device_file, EVIOCGKEY(sizeof(keyboard_state)), keyboard_state);
        }

        uint32_t LinuxInputManager::TranslateKey(Key key) const
        {
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
            }
            return 0;
        }

        Vector2I LinuxInputManager::GetMousePosition() const
        {
            return Vector2I(0, 0);
        }

        void LinuxInputManager::SetMousePosition(const Vector2I& position)
        {
        }

        Vector2 LinuxInputManager::GetMouseDistanceToCenter() const
        {
            return Vector2(0, 0);
        }

        void LinuxInputManager::SetMousePositionToCenter()
        {
        }

        bool LinuxInputManager::IsKeyPressed(Key key) const
        {
            return false;
        }

        bool LinuxInputManager::IsKeyReleased(Key key) const
        {
            return false;
        }

        bool LinuxInputManager::IsKeyHeld(Key key) const
        {
            uint32_t linux_key = TranslateKey(key);
            uint16_t key_index = linux_key / 8;
            uint16_t held_mask = 1 << (linux_key % 8);

            return keyboard_state[key_index] & held_mask;
        }

    }
}
#endif