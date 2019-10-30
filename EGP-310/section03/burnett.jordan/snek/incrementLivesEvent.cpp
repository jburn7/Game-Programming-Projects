#include "incrementLivesEvent.h"

IncrementLivesEvent::IncrementLivesEvent(EventType type, int incr) : Event(type)
{
	//game would potentially not function correctly if I allowed multiple lives to be incremented at once
	if(incr > 1)
	{
		incr = 1;
	}
	else if(incr < -1)
	{
		incr = -1;
	}
	mIncrement = incr;
}

IncrementLivesEvent::~IncrementLivesEvent()
{
}
