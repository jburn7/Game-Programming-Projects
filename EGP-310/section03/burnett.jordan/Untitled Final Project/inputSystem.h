#pragma once
#include "SFML\Window\Keyboard.hpp"
#include "SFML\Window\Event.hpp"
#include "EventSystem.h"
#include "quitEvent.h"
#include "unitAddEvent.h"
#include "unitRemoveEvent.h"
#include "mouseMoveEvent.h"
#include "startEvent.h"
#include "playerInputEvent.h"
#include "inputReleaseEvent.h"
#include "pauseEvent.h"

class InputSystem
{
public:
	InputSystem();

	~InputSystem();


	void cleanup();

	void init();


	void checkForInput();
private:
	sf::Event mEvent;
	bool mMouseReleased, mKeyReleased;
};