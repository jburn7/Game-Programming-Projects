#include "audioManager.h"

AudioManager::AudioManager()
{
	gpEventSystem->addListener(PLAY_SOUND_EVENT, this);
	gpEventSystem->addListener(VOLUME_EVENT, this);

	mMasterVolume = 1.f;
}

AudioManager::~AudioManager()
{
	cleanup();
}

void AudioManager::init()
{
	if(!mpSoundEngine)
	{
		cout << "Initializing sound engine.\n";

		mpSoundEngine = irrklang::createIrrKlangDevice();

		rapidjson::Value &doc = JSONData::getInstance()->getJSON();
		std::string soundPath = doc["game"]["soundPath"].GetString();
		for(auto &a : doc["game"]["soundBuffers"].GetArray())
		{
			addSoundBuffer(a["name"].GetString(), soundPath + a["filename"].GetString());
		}

		playMusic("default_background_music");
	}
	else
	{
		cout << "Sound Engine already initialized.\n";
	}
}

void AudioManager::cleanup()
{
	mSoundBuffers.clear();
	mpSoundEngine->stopAllSounds();
	mpSoundEngine->drop();
}

void AudioManager::addSoundBuffer(const std::string &name, const std::string &filename)
{
	irrklang::ISoundSource* temp = mpSoundEngine->addSoundSourceFromFile(filename.c_str());
	mSoundBuffers.insert(std::pair<std::string, irrklang::ISoundSource*>(name, temp));
}

void AudioManager::handleEvent(const Event &theEvent)
{
	if(theEvent.getType() == PLAY_SOUND_EVENT)
	{
		const PlaySoundEvent &ev = static_cast<const PlaySoundEvent&>(theEvent);
		playSound(ev.getSoundBufferName());
	}
	else if(theEvent.getType() == VOLUME_EVENT)
	{
		const VolumeEvent &ev = static_cast<const VolumeEvent&>(theEvent);
		float temp = ev.getVolume();
		if(temp == -1)
		{
			if(mMasterVolume == 0)
			{
				temp = 1;
			}
			else
			{
				temp = 0;
			}
		}
		else
		{
			if(temp < 0)
			{
				temp = 0;
			}
			else if(temp > 1)
			{
				temp = 1;
			}
		}

		mMasterVolume = temp;

		mpSoundEngine->setSoundVolume(mMasterVolume);
	}
}

void AudioManager::update()
{
	//moved to irrk's event system now
}

void AudioManager::playMusic(const std::string &musicName)
{
	mSoundBuffers[musicName]->setDefaultVolume(.3f);
	mpSoundEngine->play2D(mSoundBuffers[musicName], true);
}

void AudioManager::playSound(std::string &soundName)
{
	mpSoundEngine->play2D(mSoundBuffers[soundName]);
}

void AudioManager::removeSoundBuffer(std::string &soundName)
{
	mSoundBuffers.erase(soundName);
}
