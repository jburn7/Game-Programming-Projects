#pragma once
#include "event.h"

enum Direction
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

class RotateSnakeEvent : public Event
{
public:
	RotateSnakeEvent(EventType type, Direction dir);
	~RotateSnakeEvent();

	Direction getDirection() const;
private:
	Direction mDir;
};