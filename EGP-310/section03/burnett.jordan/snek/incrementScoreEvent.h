#pragma once
#include "event.h"

class IncrementScoreEvent : public Event
{
public:
	IncrementScoreEvent(EventType type, int score);
	~IncrementScoreEvent() {}

	int getScore() const { return mDeltaScore; }
private:
	int mDeltaScore;
};