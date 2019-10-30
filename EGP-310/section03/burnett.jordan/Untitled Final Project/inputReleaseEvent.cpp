#include "inputReleaseEvent.h"

InputReleaseEvent::InputReleaseEvent(EventType type, InputState state) : Event(type)
{
	mState = state;
}

InputReleaseEvent::~InputReleaseEvent()
{
}
