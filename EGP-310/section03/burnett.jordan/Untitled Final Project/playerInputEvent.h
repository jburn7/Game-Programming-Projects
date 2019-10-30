#pragma once
#include "event.h"

enum InputState
{
	MOVE_LEFT,
	MOVE_RIGHT,
	JUMP,
	ATTACK,
	PICKUP
};

class PlayerInputEvent : public Event
{
public:
	PlayerInputEvent(EventType type, InputState state);
	~PlayerInputEvent();
	InputState getInput() const { return mState; };
private:
	InputState mState;
};