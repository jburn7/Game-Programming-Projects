#pragma once
#include "Vector2D.h"
#include "playerInputComponent.h"
#include "meleeEntityInputComponent.h"
#include "projectileEntityInputComponent.h"
#include "eventListener.h"
#include "movePlayerEvent.h"

class InputManager : public EventListener
{
public:
	InputManager();
	~InputManager();

	virtual void handleEvent(const Event &theEvent);

	ProjectileEntityInputComponent *getNewProjectileEntityInputComponent();
	PlayerInputComponent *getNewPlayerInputComponent();
	MeleeEntityInputComponent *getNewMeleeEntityInputComponent();
private:
	Vector2D mPlayerPos;
};