#pragma once
#include "event.h"

class PlaySoundEvent : public Event
{
public:
	PlaySoundEvent(EventType type, const std::string &sound);
	~PlaySoundEvent();

	std::string getSoundBufferName() const { return mSoundBufferName; }
private:
	std::string mSoundBufferName;
};