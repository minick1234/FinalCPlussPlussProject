#include "Mouse.h"

Mouse::Mouse() {
	m_pos = Point(0,0);
	m_posPrev = Point(0, 0);
	m_butLDown = false;
	m_butMDown = false;
	m_butRDown = false;

}

Mouse::~Mouse() {

}

bool Mouse::Moved(SDL_Event event, Point& point) {
	if (event.type != SDL_MOUSEMOTION)return false;

	m_pos.X = event.motion.x;
	m_pos.Y = event.motion.y;

	point = m_pos;
	m_posPrev = m_pos;
	return true;
}

void Mouse::ProcessButtons(SDL_Event event) {
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) m_butLDown = true;
		if (event.button.button == SDL_BUTTON_MIDDLE) m_butMDown = true;
		if (event.button.button == SDL_BUTTON_RIGHT) m_butRDown = true;
	}
	else if (event.type == SDL_MOUSEBUTTONUP) {
		if (event.button.button == SDL_BUTTON_LEFT) m_butLDown = false;
		if (event.button.button == SDL_BUTTON_MIDDLE) m_butMDown = false;
		if (event.button.button == SDL_BUTTON_RIGHT) m_butRDown = false;

	}
}

