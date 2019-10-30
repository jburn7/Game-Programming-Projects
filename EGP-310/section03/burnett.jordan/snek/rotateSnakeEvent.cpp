#include "rotateSnakeEvent.h"

RotateSnakeEvent::RotateSnakeEvent(EventType type, Direction dir) : Event(type)
{
	mDir = dir;
}

RotateSnakeEvent::~RotateSnakeEvent()
{
}

Direction RotateSnakeEvent::getDirection() const
{
	return mDir;
}
