#include "difficultyChangeEvent.h"

bool DifficultyChangeEvent::msInit = false;
std::map<Difficulty, float> DifficultyChangeEvent::msDifficultyMap = {};

DifficultyChangeEvent::DifficultyChangeEvent(EventType type, Difficulty dif) : Event(type)
{
	if(!msInit)
	{
		std::map<std::string, Difficulty> tempMap;
		tempMap["easy"] = D_EASY;
		tempMap["normal"] = D_NORMAL;
		tempMap["hard"] = D_HARD;

		msInit = true;
		rapidjson::Value &doc = JSONData::getInstance()->getJSON()["difficulty"];
		for(auto &a : doc.GetArray())
		{
			msDifficultyMap[tempMap[a["name"].GetString()]] = a["multiplier"].GetFloat();
		}
	}

	mMultiplier = msDifficultyMap[dif];

	mDifficulty = dif;
}

DifficultyChangeEvent::~DifficultyChangeEvent()
{
}
