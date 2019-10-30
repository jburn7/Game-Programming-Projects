#pragma once
#include "event.h"

class PauseEvent : public Event
{
public:
	PauseEvent(EventType type);
	~PauseEvent();
};