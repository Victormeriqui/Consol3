#include "LinuxInputManager.hpp"

namespace Engine {
namespace Input {

    LinuxInputManager::LinuxInputManager() {
    }

    uint32_t LinuxInputManager::TranslateKey(Key key) const {
        switch (key) {
        case Key::SPACE:
            return 32;
        }
        return 0;
    }

    Point2 LinuxInputManager::GetMousePosition() const {
        return Point2(0, 0);
    }

    void LinuxInputManager::SetMousePosition(const Point2& position) {
    }

    Vector2 LinuxInputManager::GetMouseDistanceToCenter() const {
        return Vector2(0, 0);
    }

    void LinuxInputManager::SetMousePositionToCenter() {
    }

    bool LinuxInputManager::IsKeyPressed(Key key) const {
        return false;
    }

    bool LinuxInputManager::IsKeyReleased(Key key) const {
        return false;
    }

    bool LinuxInputManager::IsKeyHeld(Key key) const {
        return false;
    }

}
}
