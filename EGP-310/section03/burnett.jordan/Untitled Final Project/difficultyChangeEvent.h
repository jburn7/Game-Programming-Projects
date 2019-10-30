#pragma once
#include "event.h"
#include "jsonData.h"
#include <map>

enum Difficulty
{
	D_EASY,
	D_NORMAL,
	D_HARD
};

class DifficultyChangeEvent : public Event
{
public:
	DifficultyChangeEvent(EventType type, Difficulty dif);
	~DifficultyChangeEvent();

	Difficulty getDifficulty() const { return mDifficulty; }
	float getMultiplier() const { return mMultiplier; }
private:
	float mMultiplier;
	Difficulty mDifficulty;
	static std::map<Difficulty, float> msDifficultyMap;
	static bool msInit;
};