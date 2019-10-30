#pragma once
#include <string>
#include <trackable.h>

enum EventType
{
	INVALID_EVENT_TYPE = -1,
	UNIT_ADD_EVENT,
	UNIT_REMOVE_EVENT,
	ROTATE_SNAKE_EVENT,
	INCREMENT_SCORE_EVENT,
	INCREMENT_LIVES_EVENT,
	UPDATE_FPS_EVENT,
	QUIT_EVENT,
	FOOD_EAT_EVENT,
	START_EVENT,
};

class Event:public Trackable
{
public:
	Event( EventType type );
	virtual ~Event();

	EventType getType() const { return mType; };
private:
	EventType mType;
};