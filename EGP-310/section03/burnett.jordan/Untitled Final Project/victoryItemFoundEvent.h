#pragma once
#include "event.h"

class VictoryItemFoundEvent : public Event
{
public:
	VictoryItemFoundEvent(EventType type);
	~VictoryItemFoundEvent();
};