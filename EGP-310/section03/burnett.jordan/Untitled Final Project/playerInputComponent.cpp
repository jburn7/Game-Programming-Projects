#include "playerInputComponent.h"
#include "Entity.h"

PlayerInputComponent::PlayerInputComponent()
{
	mInputLeft = mInputRight = mInputJump = mInputAttack = false;

	mAttackReleased = mJumpReleased = true;

	mInputType = IN_PLAYER;

	gpEventSystem->addListener(PLAYER_INPUT_EVENT, this);
	gpEventSystem->addListener(INPUT_RELEASE_EVENT, this);
	gpEventSystem->addListener(MOUSE_MOVE_EVENT, this);
}

PlayerInputComponent::~PlayerInputComponent()
{
	gpEventSystem->removeListenerFromAllEvents(this);
}

//void PlayerInputComponent::save(rapidjson::Value &obj, rapidjson::Value &val, rapidjson::Document::AllocatorType &alloc)
//{
//}

void PlayerInputComponent::update(Entity* u)
{
	if(mInputLeft && mInputRight)
	{
		u->mMoveLeft = u->mMoveRight = false;
	}
	else
	{
		u->mMoveLeft = mInputLeft;
		u->mMoveRight = mInputRight;
	}
	if(!mJumpReleased)
	{
		u->mJump = false;
	}
	else
	{
		u->mJump = mInputJump;
		if(u->mJump)
		{
			mJumpReleased = false;
		}
	}
	if(!mAttackReleased)
	{
		u->mAttack = false;
	}
	else
	{
		u->mAttack = mInputAttack;
		if(u->mAttack)
		{
			mAttackReleased = false;
		}
	}
	u->mItemPickup = mInputPickup;
	//clear inputs
	mInputLeft = mInputRight = mInputJump = mInputAttack = mInputPickup = false;
}

void PlayerInputComponent::handleEvent(const Event& theEvent)
{
	if (theEvent.getType() == PLAYER_INPUT_EVENT) {
		const PlayerInputEvent &ev = static_cast<const PlayerInputEvent&>(theEvent);
		switch (ev.getInput()) {
		case MOVE_LEFT:
			mInputLeft = true;
			break;
		case MOVE_RIGHT:
			mInputRight = true;
			break;
		case JUMP:
			mInputJump = true;
			break;
		case ATTACK:
			mInputAttack = true;
			break;
		case PICKUP:
			mInputPickup = true;
			break;
		}
	}
	else if(theEvent.getType() == INPUT_RELEASE_EVENT)
	{
		const InputReleaseEvent &ev = static_cast<const InputReleaseEvent&>(theEvent);
		if(ev.getState() == ATTACK)
		{
			mAttackReleased = true;
		}
		else if(ev.getState() == JUMP)
		{
			mJumpReleased = true;
		}
	}
	else if(theEvent.getType() == MOUSE_MOVE_EVENT)
	{
		mMousePos = static_cast<const MouseMoveEvent&>(theEvent).getPosition();
	}
}
