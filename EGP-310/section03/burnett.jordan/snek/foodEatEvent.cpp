#include "foodEatEvent.h"

FoodEatEvent::FoodEatEvent(EventType type, int segments) : Event(type)
{
	mSegments = segments;
}

FoodEatEvent::~FoodEatEvent()
{
}
