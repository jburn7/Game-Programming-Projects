#pragma once
#include "event.h"

class LoadEvent : public Event
{
public:
	LoadEvent(EventType type);
	~LoadEvent();
};