#pragma once
#include "EventSystem.h"
#include "EventListener.h"
#include "playSoundEvent.h"
#include "volumeEvent.h"
#include "SFML\Audio.hpp"
#include "irrKlang.h"
#include "jsonData.h"
#include <string>
#include <map>
#include <vector>

class AudioManager : public EventListener
{
public:
	AudioManager();
	~AudioManager();

	void init();

	void cleanup();

	void addSoundBuffer(const std::string &name, const std::string &filename);

	virtual void handleEvent(const Event &theEvent);

	void update();

	void playMusic(const std::string &musicName);

	void playSound(std::string &soundName);

	void removeSoundBuffer(std::string &soundName);
private:
	irrklang::ISoundEngine *mpSoundEngine;

	float mMasterVolume;

	std::map<std::string, irrklang::ISoundSource*> mSoundBuffers;
	std::vector<irrklang::ISound> mPlayingSounds;
};