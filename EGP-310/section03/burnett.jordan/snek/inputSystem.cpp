#include "inputSystem.h"
#include "game.h"

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
	cleanup();
}

void InputSystem::cleanup()
{
	if(mEventQueue)
	{
		al_destroy_event_queue(mEventQueue);
		mEventQueue = NULL;
	}

	al_uninstall_keyboard();
}

void InputSystem::init()
{
	if(!al_install_keyboard())
	{
		std::cout << "Could not install keyboard.\n";
	}

	mEventQueue = al_create_event_queue();
	al_register_event_source(mEventQueue, al_get_keyboard_event_source());
}

void InputSystem::checkForInput()
{
	ALLEGRO_EVENT alEvent;
	ALLEGRO_TIMEOUT timeout;
	al_init_timeout(&timeout, 0.01);

	if(al_wait_for_event_until(mEventQueue, &alEvent, &timeout))
	{
		if(alEvent.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if(alEvent.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{
				gpEventSystem->fireEvent(QuitEvent(QUIT_EVENT));
			}
			if(Game::getInstance()->getGamestate() == PLAYING)
			{
				if(alEvent.keyboard.keycode == ALLEGRO_KEY_LEFT)
				{
					gpEventSystem->fireEvent(RotateSnakeEvent(ROTATE_SNAKE_EVENT, LEFT));
				}
				if(alEvent.keyboard.keycode == ALLEGRO_KEY_RIGHT)
				{
					gpEventSystem->fireEvent(RotateSnakeEvent(ROTATE_SNAKE_EVENT, RIGHT));
				}
				if(alEvent.keyboard.keycode == ALLEGRO_KEY_UP)
				{
					gpEventSystem->fireEvent(RotateSnakeEvent(ROTATE_SNAKE_EVENT, UP));
				}
				if(alEvent.keyboard.keycode == ALLEGRO_KEY_DOWN)
				{
					gpEventSystem->fireEvent(RotateSnakeEvent(ROTATE_SNAKE_EVENT, DOWN));
				}
				if(alEvent.keyboard.keycode == ALLEGRO_KEY_D)
				{
					int i = 0;
				}
			}
			if(alEvent.keyboard.keycode == ALLEGRO_KEY_SPACE)
			{
				gpEventSystem->fireEvent(StartEvent(START_EVENT));
			}
		}
	}
}
