#include "playSoundEvent.h"

PlaySoundEvent::PlaySoundEvent(EventType type, const std::string &sound) : Event(type)
{
	mSoundBufferName = sound;
}

PlaySoundEvent::~PlaySoundEvent()
{
}
