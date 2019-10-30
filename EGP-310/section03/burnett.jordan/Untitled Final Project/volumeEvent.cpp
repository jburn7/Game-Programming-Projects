#include "volumeEvent.h"

VolumeEvent::VolumeEvent(EventType type, float volume) : Event(type)
{
	mVolume = volume;
}

VolumeEvent::~VolumeEvent()
{
}
