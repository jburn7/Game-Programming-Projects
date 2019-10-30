#include "mouseMoveEvent.h"

MouseMoveEvent::MouseMoveEvent(EventType type, Vector2D pos) : Event(type)
{
	mPos = pos;
}

MouseMoveEvent::~MouseMoveEvent()
{
}
