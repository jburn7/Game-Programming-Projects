#include "inputManager.h"

InputManager::InputManager()
{
	gpEventSystem->addListener(MOVE_PLAYER_EVENT, this);
}

InputManager::~InputManager()
{
}

void InputManager::handleEvent(const Event &theEvent)
{
	if(theEvent.getType() == MOVE_PLAYER_EVENT)
	{
		mPlayerPos = static_cast<const MovePlayerEvent&>(theEvent).getPlayerPosition();
	}
}

ProjectileEntityInputComponent *InputManager::getNewProjectileEntityInputComponent()
{
	return new ProjectileEntityInputComponent(&mPlayerPos);;
}

PlayerInputComponent *InputManager::getNewPlayerInputComponent()
{
	return new PlayerInputComponent();
}

MeleeEntityInputComponent *InputManager::getNewMeleeEntityInputComponent()
{
	return new MeleeEntityInputComponent(&mPlayerPos);
}
