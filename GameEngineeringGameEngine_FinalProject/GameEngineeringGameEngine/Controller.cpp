#include "Controller.h"

Controller::Controller() {

}

Controller::~Controller() {

}

void Controller::DetectController() {
	m_controllers.clear();

	int numControllers = SDL_NumJoysticks();
	if (numControllers == 0)return;

	for (int count = 0; count < numControllers; count++) {
		if (!SDL_IsGameController(count))continue;
		Add(SDL_GameControllerOpen(count), count);
	}
}

void Controller::Add(SDL_GameController* controller, int controllerID) {
	if (controller == nullptr)return;

	SDL_Joystick* j = SDL_GameControllerGetJoystick(controller);
	SDL_JoystickID jID = SDL_JoystickInstanceID(j);

	for (ControllerInfo c : m_controllers) {
		if (c.ID == jID)return;
	}

	ControllerInfo c = ControllerInfo();
	c.Controller = controller;
	c.name = string(SDL_GameControllerName(controller));
	c.ID = jID;
	m_controllers.push_back(c);

}

bool Controller::Added(SDL_Event event) {
	if (event.type != SDL_CONTROLLERDEVICEADDED) {
		return false;
	}

	if (SDL_IsGameController(event.cdevice.which))
	{
		SDL_GameController* controller = SDL_GameControllerOpen(event.cdevice.which);
		Add(controller, event.cdevice.which);
	}

	return true;
}

bool Controller::Removed(SDL_Event event) {

	if (event.type != SDL_CONTROLLERDEVICEREMOVED) {
		return false;
	}

	for (unsigned int count = 0; count < m_controllers.size(); count++) {
		if (m_controllers[count].ID != event.cdevice.which)continue;
		m_controllers.erase(m_controllers.begin() + count);
		break;
	}

	return true;
}

bool Controller::ProcessButtons(SDL_Event event) {
	if (event.type == SDL_CONTROLLERBUTTONDOWN) {
		for (unsigned int count = 0; count < m_controllers.size(); count++) {
			if (m_controllers[count].ID != event.cdevice.which) {
				continue;
			}
			auto v = m_controllers[count].Buttons;
			if (std::find(v.begin(), v.end(), event.cbutton.button) == v.end()) {
				m_controllers[count].Buttons.push_back(SDL_GameControllerButton(event.cbutton.button));
			}
			break;
		}
		return true;
	}
	else if (event.type == SDL_CONTROLLERBUTTONUP) {
		for (unsigned int count = 0; count < m_controllers.size(); count++) {
			vector<SDL_GameControllerButton>* v = &m_controllers[count].Buttons;
			for (unsigned int button = 0; button < v->size(); button++) {
				if ((*v)[button] == event.cbutton.button) {
					v->erase(v->begin() + button);
					break;
				}
			}
		}
		return true;
	}
	return false;

}

bool Controller::ProcessMotionForLeftStick(SDL_Event event)
{
	if (event.type != SDL_CONTROLLERAXISMOTION) return false;

	for (unsigned int count = 0; count < m_controllers.size(); count++) {
		if (m_controllers[count].ID != event.cdevice.which) {
			continue;
		}

		//This is the left analog stick
		if (event.caxis.axis == 0) { // x axis motion
			m_controllers[count].LeftAxis.X = event.caxis.value;
			LeftStickXValue = event.caxis.value;
		}
		else if (event.caxis.axis == 1) {
			m_controllers[count].LeftAxis.Y = event.caxis.value;
			LeftStickYValue = event.caxis.value;
		}
	}
	return true;
}

bool Controller::ProcessMotionForRightStick(SDL_Event event)
{
	if (event.type != SDL_CONTROLLERAXISMOTION) return false;

	for (unsigned int count = 0; count < m_controllers.size(); count++) {
		if (m_controllers[count].ID != event.cdevice.which) {
			continue;
		}
		//This is the Right analog stick
		if (event.caxis.axis == 2) {
			m_controllers[count].RightAxis.X = event.caxis.value;
			RightStickXValue = event.caxis.value;

		}
		else if (event.caxis.axis == 3) {
			m_controllers[count].RightAxis.Y = event.caxis.value;
			RightStickYValue = event.caxis.value;

		}
	}
	return true;
}

bool Controller::ProcessMotion(SDL_Event event) {
	if (event.type != SDL_CONTROLLERAXISMOTION) return false;

	for (unsigned int count = 0; count < m_controllers.size(); count++) {
		if (m_controllers[count].ID != event.cdevice.which) {
			continue;
		}


		//This is the left analog stick
		if (event.caxis.axis == 0) { // x axis motion
			m_controllers[count].LeftAxis.X = event.caxis.value;
		}
		else if (event.caxis.axis == 1) {
			m_controllers[count].LeftAxis.Y = event.caxis.value;

		}

		//This is the Right analog stick
		if (event.caxis.axis == 2) {
			m_controllers[count].RightAxis.X = event.caxis.value;

		}
		else if (event.caxis.axis == 3) {
			m_controllers[count].RightAxis.Y = event.caxis.value;

		}

	}

	return true;
}

string Controller::ToString() {
	int cc = 0;
	string s = "Controllers: ";

	for (ControllerInfo c : m_controllers) {
		cc++;
		s += to_string(cc) + " - " + c.name + ": ";
		s += c.ToString();
	}
	return s;
}