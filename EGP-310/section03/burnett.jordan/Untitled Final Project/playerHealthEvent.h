#pragma once
#include "event.h"

class PlayerHealthEvent : public Event
{
public:
	PlayerHealthEvent(EventType type, int newHealth);
	PlayerHealthEvent();
	
	int getHealth() { return mHealth; }
private:
	int mHealth;
};