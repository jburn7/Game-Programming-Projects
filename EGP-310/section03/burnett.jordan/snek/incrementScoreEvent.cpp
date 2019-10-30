#include "incrementScoreEvent.h"

IncrementScoreEvent::IncrementScoreEvent(EventType type, int score) : Event(type)
{
	mDeltaScore = score;
}
