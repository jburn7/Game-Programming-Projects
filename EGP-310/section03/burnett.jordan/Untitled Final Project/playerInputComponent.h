#pragma once
#include "inputComponent.h"
#include "EventListener.h"
#include "playerInputEvent.h"
#include "inputReleaseEvent.h"
#include "mouseMoveEvent.h"

class PlayerInputComponent :public InputComponent, public EventListener
{
private:
	bool mInputLeft, mInputRight, mInputJump, mInputAttack, mInputPickup;

	//bools that monitor when keys are released to not allow repeated actions
	bool mAttackReleased, mJumpReleased;

	Vector2D mMousePos;
public:
	PlayerInputComponent();
	virtual ~PlayerInputComponent();

	//virtual void save(rapidjson::Value &obj, rapidjson::Value &val, rapidjson::Document::AllocatorType &alloc);

	void update(Entity* u);
	void handleEvent(const Event& theEvent);
};
