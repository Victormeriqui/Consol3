#ifdef SYS_WINDOWS

#include "WindowsInputManager.hpp"

namespace Engine
{
    namespace Input
    {

        WindowsInputManager::WindowsInputManager()
        {
        }

        DWORD WindowsInputManager::TranslateKey(Key key) const
        {
            switch (key)
            {
            case Key::CAPITAL:
                return VK_CAPITAL;
            case Key::SPACE:
                return VK_SPACE;
            case Key::LCONTROL:
                return VK_LCONTROL;
            case Key::LSHIFT:
                return VK_LSHIFT;
            case Key::Q:
                return 0x51;
            case Key::W:
                return 0x57;
            case Key::E:
                return 0x45;
            case Key::A:
                return 0x41;
            case Key::S:
                return 0x53;
            case Key::D:
                return 0x44;
            case Key::P:
                return 0x50;
            case Key::TAB:
                return VK_TAB;
            case Key::MOUSE1:
                return VK_LBUTTON;
            case Key::MOUSE2:
                return VK_RBUTTON;
            case Key::MOUSE3:
                return VK_MBUTTON;
            case Key::MOUSE4:
                return VK_XBUTTON1;
            case Key::MOUSE5:
                return VK_XBUTTON2;
            case Key::N1:
                return 0x31;
            case Key::N2:
                return 0x32;
            case Key::N3:
                return 0x33;
            case Key::N4:
                return 0x34;
            case Key::N5:
                return 0x35;
            case Key::N6:
                return 0x36;
            case Key::N7:
                return 0x37;
            case Key::N8:
                return 0x38;
            case Key::N9:
                return 0x39;
            case Key::N0:
                return 0x30;
            }
            return 0;
        }

        Point2 WindowsInputManager::GetMousePosition() const
        {
            POINT coord = { 0, 0 };
            GetCursorPos(&coord);

            return Point2((uint16_t)coord.x, (uint16_t)coord.y);
        }

        void WindowsInputManager::SetMousePosition(const Point2& position)
        {
            SetCursorPos(position.x, position.y);
        }

        Vector2 WindowsInputManager::GetMouseDistanceToCenter() const
        {
            RECT window = { 0, 0, 0, 0 };
            GetWindowRect(GetConsoleWindow(), &window);

            Point2 center = Point2((uint16_t)(window.left + ((window.right - window.left) / 2)), (uint16_t)(window.top + ((window.bottom - window.top) / 2)));

            Point2 mouse_pos = GetMousePosition();

            return Vector2((float)mouse_pos.x - center.x, (float)mouse_pos.y - center.y);
        }

        void WindowsInputManager::SetMousePositionToCenter()
        {
            RECT window = { 0, 0, 0, 0 };
            GetWindowRect(GetConsoleWindow(), &window);

            Point2 center = Point2((uint16_t)(window.left + ((window.right - window.left) / 2)), (uint16_t)(window.top + ((window.bottom - window.top) / 2)));

            SetMousePosition(center);
        }

        bool WindowsInputManager::IsKeyPressed(Key key) const
        {
            return GetKeyState(TranslateKey(key)) & 0x0001;
        }

        bool WindowsInputManager::IsKeyReleased(Key key) const
        {
            return (GetKeyState(TranslateKey(key)) & 0x8000) == 0;
        }

        bool WindowsInputManager::IsKeyHeld(Key key) const
        {
            return (GetKeyState(TranslateKey(key)) & 0x8000) != 0;
        }

    }
}
#endif