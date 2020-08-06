/*
 * Created: 02/08 2020
 *  by Rei de Vries
 */

#include "InputData.hpp"

void InputData::updateValues(const Camera cam)
{
	mouse.ray = GetMouseRay(GetMousePosition(), cam);
	mouse.screen_pos = {
		static_cast<float>(GetMouseX()), 
		static_cast<float>(GetMouseY())
	};
	mouse.l.down = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
	mouse.m.down = IsMouseButtonDown(MOUSE_MIDDLE_BUTTON);
	mouse.r.down = IsMouseButtonDown(MOUSE_RIGHT_BUTTON);
	mouse.l.press = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
	mouse.m.press = IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON);
	mouse.r.press = IsMouseButtonDown(MOUSE_RIGHT_BUTTON);
	mouse.l.rls = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
	mouse.m.rls = IsMouseButtonReleased(MOUSE_MIDDLE_BUTTON);
	mouse.r.rls = IsMouseButtonReleased(MOUSE_RIGHT_BUTTON);
	mouse.scroll = GetMouseWheelMove();
}
