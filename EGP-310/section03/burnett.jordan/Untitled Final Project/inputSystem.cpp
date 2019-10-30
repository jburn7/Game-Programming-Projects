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
}

void InputSystem::init()
{
}

void InputSystem::checkForInput()
{
	sf::Event e;
	while(Game::getInstance()->getGraphics().mDisplay.pollEvent(e))
	{
		if(e.type == sf::Event::Closed)
		{
			gpEventSystem->fireEvent(new QuitEvent(QUIT_EVENT));
		}
		else if(e.type == sf::Event::KeyReleased)
		{
			if(e.key.code == sf::Keyboard::Up)
			{
				gpEventSystem->fireEvent(new InputReleaseEvent(INPUT_RELEASE_EVENT, ATTACK));
			}
			if(e.key.code == sf::Keyboard::Space)
			{
				gpEventSystem->fireEvent(new InputReleaseEvent(INPUT_RELEASE_EVENT, JUMP));
			}
		}
		else if(e.type == sf::Event::MouseButtonPressed)
		{
			if(Game::getInstance()->getGamestate() != PLAYING)
				gpEventSystem->fireEvent(new MouseClickEvent(MOUSE_CLICK_EVENT, sf::Mouse::getPosition(Game::getInstance()->getGraphics().mDisplay)));
			else
			{
				//gpEventSystem->fireEvent(new PlayerInputEvent(PLAYER_INPUT_EVENT, ATTACK));
			}
		}
		else if(e.type == sf::Event::MouseButtonReleased)
		{
 			if(e.mouseButton.button == sf::Mouse::Left)
			{
			}
		}
		else if(e.type == sf::Event::MouseMoved)
		{
			gpEventSystem->fireEvent(new MouseMoveEvent(MOUSE_MOVE_EVENT, sf::Mouse::getPosition(Game::getInstance()->getGraphics().mDisplay)));
		}
	}
	if(Game::getInstance()->getGamestate() == PLAYING)
	{
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			gpEventSystem->fireEvent(new PlayerInputEvent(PLAYER_INPUT_EVENT, MOVE_LEFT));
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			gpEventSystem->fireEvent(new PlayerInputEvent(PLAYER_INPUT_EVENT, MOVE_RIGHT));
		}
		/*if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			gpEventSystem->fireEvent(new PlayerInputEvent(PLAYER_INPUT_EVENT, ATTACK));
		}
		else
		{
			gpEventSystem->fireEvent(new InputReleaseEvent(INPUT_RELEASE_EVENT, ATTACK));
		}*/
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			gpEventSystem->fireEvent(new PlayerInputEvent(PLAYER_INPUT_EVENT, ATTACK));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			gpEventSystem->fireEvent(new PlayerInputEvent(PLAYER_INPUT_EVENT, JUMP));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
		{
			gpEventSystem->fireEvent(new PlayerInputEvent(PLAYER_INPUT_EVENT, PICKUP));
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		if(Game::getInstance()->getGamestate() != PLAYING)
			gpEventSystem->fireEvent(new StartEvent(START_EVENT));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		gpEventSystem->fireEvent(new PauseEvent(PAUSE_EVENT));
	}
}
