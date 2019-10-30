#include "levelManager.h"
#include "game.h"

LevelManager::LevelManager()
{
	mCurrentLevel = 0;
	mMaxLevels = 0;

	//gpEventSystem->addListener(DIFFICULTY_CHANGE_EVENT, this);
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

	loadLevel();
}

void LevelManager::cleanup()
{
	mLevels.clear();
}

void LevelManager::update(double timeElapsed)
{
	//mLevels[mCurrentLevel].update(timeElapsed);
}

void LevelManager::handleEvent(const Event &theEvent)
{
}

void LevelManager::save(rapidjson::Document &doc, rapidjson::Document::AllocatorType &alloc)
{
	doc.AddMember("level_number", mCurrentLevel, alloc);
	mLevels[mCurrentLevel].save(doc, alloc);
}

bool LevelManager::loadNewLevel()
{
	if(mCurrentLevel + 1 < mMaxLevels)
	{
		mCurrentLevel++;
		Game::getInstance()->getUnitManager().setLevelEnded(true);
		loadLevel();
		return true;
	}
	else
	{
		Game::getInstance()->complete();
	}

	return false;
}

void LevelManager::loadFromFile(rapidjson::Document &doc)
{
	if(doc["level_number"].GetInt() <= mMaxLevels)
	{
		mCurrentLevel = doc["level_number"].GetInt();
		mLevels[mCurrentLevel].loadWallsOnly();
		mLevels[mCurrentLevel].mVictoryItemsFound = doc["victory_items_found"].GetInt();
		mLevels[mCurrentLevel].mVictoryItemsRequired = doc["victory_items_required"].GetInt();
	}
	else
	{
		cout << "Error loading level.\n";
	}
}

void LevelManager::loadLevel()
{
	mLevels[mCurrentLevel].load();
}
