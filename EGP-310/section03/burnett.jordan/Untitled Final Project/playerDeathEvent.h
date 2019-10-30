#pragma once
#include "event.h"

class PlayerDeathEvent : public Event
{
public:
	PlayerDeathEvent(EventType type);
	~PlayerDeathEvent();
};