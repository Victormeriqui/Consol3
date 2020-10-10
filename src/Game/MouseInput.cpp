#include "MouseInput.hpp"

#include "../Engine/Math/Point2.hpp"

#include <cstdint>
// Windows.h overrides std::min
#define NOMINMAX
#include <Windows.h>

namespace Game
{
	MouseInput::MouseInput()
	{};

	Point2 MouseInput::GetMousePosition()
	{
		POINT coord = { 0, 0 };
		GetCursorPos(&coord);

		return Point2((uint16_t)coord.x, (uint16_t)coord.y);
	}

	void MouseInput::SetMousePosition(const Point2& position)
	{
		SetCursorPos(position.x, position.y);
	}

	Vector2 MouseInput::GetMouseDistanceToCenter()
	{
		RECT window = { 0, 0, 0, 0 };
		GetWindowRect(GetConsoleWindow(), &window);

		Point2 center = Point2(
			(uint16_t)(window.left + ((window.right - window.left) / 2)),
			(uint16_t)(window.top + ((window.bottom - window.top) / 2)));

		Point2 mouse_pos = GetMousePosition();

		return Vector2((float)mouse_pos.x - center.x, (float)mouse_pos.y - center.y);
	}

	void MouseInput::SetMousePositionToCenter()
	{
		RECT window = { 0, 0, 0, 0 };
		GetWindowRect(GetConsoleWindow(), &window);

		Point2 center = Point2(
			(uint16_t)(window.left + ((window.right - window.left) / 2)),
			(uint16_t)(window.top + ((window.bottom - window.top) / 2)));

		SetMousePosition(center);
	}
}
