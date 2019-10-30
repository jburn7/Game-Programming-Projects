#pragma once
#include "event.h"

class VolumeEvent : public Event
{
public:
	VolumeEvent(EventType type, float volume);
	~VolumeEvent();

	float getVolume() const { return mVolume; }
private:
	float mVolume;
};