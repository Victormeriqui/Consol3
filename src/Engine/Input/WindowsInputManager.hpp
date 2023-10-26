#ifdef SYS_WINDOWS
#ifndef WINDOWSINPUTMANAGER_HPP
#define WINDOWSINPUTMANAGER_HPP

#include "IInputManager.hpp"

// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>

namespace Engine
{
    namespace Input
    {

        class WindowsInputManager : public IInputManager
        {
        private:
            DWORD TranslateKey(Key key) const;

        public:
            WindowsInputManager();

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
#endif