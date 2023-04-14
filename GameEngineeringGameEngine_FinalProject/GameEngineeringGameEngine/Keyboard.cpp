#include "Keyboard.h"

Keyboard::Keyboard() {

}

Keyboard::~Keyboard()
{}


bool Keyboard::KeyDown(SDL_Event event, SDL_Keycode key)
{
	if (event.type != SDL_KEYDOWN) {
		return false;
	}

	return key == event.key.keysym.sym;
}

bool Keyboard::KeyUp(SDL_Event event, SDL_Keycode key)
{
	if (event.type != SDL_KEYUP) {
		return false;
	}
	return key == event.key.keysym.sym;

}

string Keyboard::TextInput(SDL_Event event) {
	if (event.type != SDL_TEXTINPUT) { return ""; }

	return string(event.text.text);
}
