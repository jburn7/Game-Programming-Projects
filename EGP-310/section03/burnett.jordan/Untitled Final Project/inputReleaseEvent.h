#pragma once
#include "event.h"
#include "playerInputEvent.h"

class InputReleaseEvent : public Event
{
public:
	InputReleaseEvent(EventType type, InputState state);
	~InputReleaseEvent();

	InputState getState() const { return mState; }
private:
	InputState mState;
};