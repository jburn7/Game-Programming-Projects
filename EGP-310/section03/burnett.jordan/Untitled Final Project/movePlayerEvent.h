#pragma once
#include "event.h"
#include "Vector2D.h"

//have any AI that will track the player be a listener for this
class MovePlayerEvent : public Event
{
public:
	MovePlayerEvent(EventType type, Vector2D pos);
	~MovePlayerEvent();

	Vector2D getPlayerPosition() const { return mPlayerPosition; }
private:
	Vector2D mPlayerPosition;
};