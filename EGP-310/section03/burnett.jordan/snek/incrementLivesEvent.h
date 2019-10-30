#pragma once
#include "event.h"

class IncrementLivesEvent : public Event
{
public:
	IncrementLivesEvent(EventType type, int incr);
	~IncrementLivesEvent();

	int getIncr() const { return mIncrement; }
private:
	int mIncrement;
};