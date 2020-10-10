#ifndef MOUSEINPUT_HPP
#define MOUSEINPUT_HPP

#include "../Engine/Math/Point2.hpp"

#include <cstdint>
// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>

using namespace Engine::Math;

namespace Game
{
	class MouseInput
	{
	public:

		MouseInput();

		[[nodiscard]] static Point2 GetMousePosition();
		static inline void SetMousePosition(const Point2& position);

		[[nodiscard]] static Vector2 GetMouseDistanceToCenter();
		static void SetMousePositionToCenter();
	};
}

#endif
