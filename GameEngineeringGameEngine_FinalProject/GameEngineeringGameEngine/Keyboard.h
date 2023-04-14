#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "StandardIncludes.h"

class Keyboard {
public:
	Keyboard();
	virtual ~Keyboard();

	bool KeyDown(SDL_Event _event, SDL_Keycode _key);
	bool KeyUp(SDL_Event _event, SDL_Keycode _key);
	string TextInput(SDL_Event _event);

private:
};


#endif // !KEYBOARD_H

