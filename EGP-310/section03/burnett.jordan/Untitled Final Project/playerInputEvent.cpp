#include "playerInputEvent.h"

PlayerInputEvent::PlayerInputEvent(EventType type, InputState state) : Event(type)
{
	mState = state;
}

PlayerInputEvent::~PlayerInputEvent()
{
}
