#include "mouseClickEvent.h"

MouseClickEvent::MouseClickEvent(EventType type, Vector2D pos) : Event(type)
{
	mPosition = pos;
}

MouseClickEvent::~MouseClickEvent()
{
}
