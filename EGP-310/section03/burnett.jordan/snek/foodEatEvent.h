#pragma once
#include "event.h"

class FoodEatEvent : public Event
{
public:
	FoodEatEvent(EventType type, int segments);
	
	~FoodEatEvent();

	int getSegments() const { return mSegments; }
private:
	int mSegments; //num of body segments of snake = food pieces that snake has eaten
};