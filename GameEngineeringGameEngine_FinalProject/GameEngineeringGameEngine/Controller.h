#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "StandardIncludes.h"

struct ControllerInfo {
	SDL_JoystickID ID = 1;
	SDL_GameController* Controller = nullptr;
	string name;

	vector<SDL_GameControllerButton> Buttons;
	Position LeftAxis = {};
	Position RightAxis = {};


	string ToString() {
		string buttonsString = "Buttons Down: ";

		for (unsigned int count = 0; count < Buttons.size(); count++) {
			buttonsString += to_string(Buttons[count]) + ";";
		}
		buttonsString += "[LX: " + to_string(LeftAxis.X) + ", LY: " + to_string(LeftAxis.Y) + "] ";
		buttonsString += "[RX: " + to_string(RightAxis.X) + ", RY: " + to_string(RightAxis.Y) + "] ";

		return buttonsString;
	}

};

class Controller {
public:
	Controller();
	virtual ~Controller();

	vector<ControllerInfo>& GetControllers() {
		return m_controllers;
	}

	void DetectController();
	bool Added(SDL_Event event);
	bool Removed(SDL_Event event);
	bool ProcessButtons(SDL_Event event);
	bool ProcessMotion(SDL_Event event);
	string ToString();

private:
	void Add(SDL_GameController* _controller, int _controllerID);

	vector<ControllerInfo> m_controllers;



};
#endif // !CONTROLLER_H

