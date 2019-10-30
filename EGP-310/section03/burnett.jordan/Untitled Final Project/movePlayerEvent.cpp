#include "movePlayerEvent.h"

MovePlayerEvent::MovePlayerEvent(EventType type, Vector2D pos) : Event(type)
{
	mPlayerPosition = pos;
}

MovePlayerEvent::~MovePlayerEvent()
{
}
