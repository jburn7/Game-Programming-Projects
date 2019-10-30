#pragma once
#include "allegro5\allegro.h"
#include "allegro5\keyboard.h"
#include "EventSystem.h"
#include "quitEvent.h"
#include "unitAddEvent.h"
#include "unitRemoveEvent.h"
#include "rotateSnakeEvent.h"
#include "startEvent.h"

class InputSystem
{
public:
	InputSystem();

	~InputSystem();


	void cleanup();

	void init();


	void checkForInput();
private:
	ALLEGRO_EVENT_QUEUE *mEventQueue;
	bool mMouseReleased, mKeyReleased;
};