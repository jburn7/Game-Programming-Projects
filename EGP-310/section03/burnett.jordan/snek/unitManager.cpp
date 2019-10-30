#include "unitManager.h"
#include "game.h"

UnitManager::UnitManager()
{
	gpEventSystem->addListener(UNIT_ADD_EVENT, this);
	gpEventSystem->addListener(UNIT_REMOVE_EVENT, this);
}

UnitManager::~UnitManager()
{
	cleanup();
}

void UnitManager::setUnitAnimations(std::map<std::string, Animation*> &animations)
{
	mUnitAnimations = animations;
}

std::map<std::string, Animation*> &UnitManager::getUnitAnimations()
{
	return mUnitAnimations;
}

void UnitManager::addUnit(Unit *unit)
{
	mUnits.push_back(unit);
}

void UnitManager::clear()
{
	for(unsigned int i = 0; i < mUnits.size(); i++)
	{
		if(mUnits[i])
		{
			delete mUnits[i];
			mUnits[i] = NULL;
		}
	}

	mUnits.clear();
}

void UnitManager::deleteUnit(int index)
{
	delete mUnits[index];
	mUnits[index] = NULL;
	mUnits.erase(mUnits.begin() + index);
}

Unit *UnitManager::getUnit(int index)
{
	return mUnits[index];
}

void UnitManager::draw()
{
	for(unsigned int i = 0; i < mUnits.size(); i++)
	{
		mUnits[i]->draw();
	}
}

void UnitManager::resetLevel(Vector2D &snakeSpawn)
{
	for(int i = mUnits.size() - 1; i >= 0; i--)
	{
		Snake *s = dynamic_cast<Snake*>(mUnits[i]);
		if(s)
		{
			s->reset(snakeSpawn);
		}
		if(mUnits[i]->tryRemove() && !s)
		{
			delete mUnits[i];
			mUnits.erase(mUnits.begin() + i);
		}
	}
}

void UnitManager::update(double timeElapsed)
{
	mLevelEnded = false;
	for(unsigned int i = 0; i < mUnits.size(); i++)
	{
		if(!mLevelEnded)
			mUnits[i]->update(timeElapsed);
	}
	if(mLevelEnded)
	{
		clear();
		Game::getInstance()->getLevelManager().loadLevel();
	}
}

void UnitManager::handleEvent(const Event &theEvent)
{
	if(theEvent.getType() == UNIT_ADD_EVENT)
	{
		const UnitAddEvent &ev = static_cast<const UnitAddEvent&>(theEvent);
		Unit *unit = ev.getUnit();
		assert(unit);
		addUnit(unit);
	}
	else if(theEvent.getType() == UNIT_REMOVE_EVENT)
	{
		const UnitRemoveEvent &ev = static_cast<const UnitRemoveEvent&>(theEvent);
		std::vector<int> removables;
		for(int i = mUnits.size() - 1; i >= 0; i--) //walk backwards to easily remove Units
		{
			if(mUnits[i] == ev.getToRemove()) //this code for deciding removal belongs in the function calling this one
			{
				delete mUnits[i];
				mUnits.erase(mUnits.begin() + i);
			}
		}
	}
}

void UnitManager::setLevelEnded(bool torf)
{
	mLevelEnded = torf;
}

void UnitManager::cleanup()
{
	clear();

	for(auto it = mUnitAnimations.begin(); it != mUnitAnimations.end(); it++)
	{
		if(it->second)
		{
			delete it->second;
			it->second = NULL;
		}
	}

	mUnitAnimations.clear();
}