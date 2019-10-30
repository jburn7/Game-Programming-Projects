#pragma once
#include "event.h"
#include "Vector2D.h"

class MouseClickEvent : public Event
{
public:
	MouseClickEvent(EventType type, Vector2D pos);
	~MouseClickEvent();

	Vector2D getPosition() const { return mPosition; }
private:
	Vector2D mPosition;
};