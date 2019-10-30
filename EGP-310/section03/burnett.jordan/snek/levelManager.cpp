#include "levelManager.h"
#include "game.h"

LevelManager::LevelManager()
{
	mCurrentLevel = 0;
	mMaxLevels = 0;

	gpEventSystem->addListener(FOOD_EAT_EVENT, this);
}

LevelManager::~LevelManager()
{
}

void LevelManager::init()
{
	rapidjson::Document &doc = JSONData::getInstance()->getJSON();
	mMaxLevels = doc[JSONPATH]["maxLevels"].GetInt();
	mCurrentLevel = doc[JSONPATH]["startingLevel"].GetInt() - 1; //will be incremented in loadnewLevel()
	mLevelsPath = doc[JSONPATH]["levelsPath"].GetString();

	for(int i = 0; i < mMaxLevels; i++)
	{
		mLevels.push_back(Level(mLevelsPath + std::to_string(i + 1) + ".txt", i));
	}

	mLevels[mCurrentLevel].load();
}

void LevelManager::cleanup()
{
}

void LevelManager::update(double timeElapsed)
{
	mLevels[mCurrentLevel].update(timeElapsed);
}

void LevelManager::handleEvent(const Event &theEvent)
{
	if(theEvent.getType() == FOOD_EAT_EVENT)
	{
		const FoodEatEvent &ev = static_cast<const FoodEatEvent&>(theEvent);
		mLevels[mCurrentLevel].spawnFood();
		if(ev.getSegments() >= mLevels[mCurrentLevel].mTargetSegments)
		{
			loadNewLevel();
		}
	}
}

bool LevelManager::loadNewLevel()
{
	if(mCurrentLevel + 1 < mMaxLevels)
	{
		mCurrentLevel++;
		Game::getInstance()->getUnitManager().setLevelEnded(true);
		return true;
	}
	else
	{
		Game::getInstance()->complete();
	}

	return false;
}

void LevelManager::loadLevel()
{
	mLevels[mCurrentLevel].load();
}
