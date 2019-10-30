#include "playerHealthEvent.h"

PlayerHealthEvent::PlayerHealthEvent(EventType type, int newHealth) : Event(type)
{
	mHealth = newHealth;
}
