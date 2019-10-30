#pragma once
#include "event.h"

class LevelEndEvent : public Event
{
public:
	LevelEndEvent(EventType type);
	~LevelEndEvent();
};