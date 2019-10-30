#pragma once
#include "event.h"
#include "Vector2D.h"

class MouseMoveEvent : public Event
{
public:
	MouseMoveEvent(EventType type, Vector2D pos);
	~MouseMoveEvent();

	Vector2D getPosition() const { return mPos; }
private:
	Vector2D mPos;
};