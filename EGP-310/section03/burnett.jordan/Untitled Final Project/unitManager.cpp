#include "unitManager.h"
#include "game.h"

UnitManager::UnitManager()
{
	gpEventSystem->addListener(UNIT_ADD_EVENT, this);
	gpEventSystem->addListener(UNIT_REMOVE_EVENT, this);
	gpEventSystem->addListener(DIFFICULTY_CHANGE_EVENT, this);
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
	if(unit->mType == U_ATTACK || unit->mType == U_ENTITY || unit->mType == U_COLLECTIBLE)
	{
		mMovingUnits.push_back(unit);
	}
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

	mMovingUnits.clear();

	mWallMask.clear();
	mWallMaskPointers.clear();
}

void UnitManager::deleteUnit(int index)
{
	if(mUnits[index]->mType == U_ATTACK || mUnits[index]->mType == U_ENTITY || mUnits[index]->mType == U_COLLECTIBLE)
	{
		for(unsigned int i = 0; i < mMovingUnits.size(); i++)
		{
			if(mMovingUnits[i] == mUnits[index])
			{
				mMovingUnits.erase(mMovingUnits.begin() + i);
				break;
			}
		}
	}
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

void UnitManager::load(rapidjson::Document &doc)
{
	rapidjson::Value &arr = doc["units"];
	for(int i = mUnits.size() - 1; i >= 0; i--)
	{
		deleteUnit(i);
	}
	for(auto &a : arr.GetArray())
	{
		switch(a["type"].GetInt())
		{
		case U_ATTACK:
			//don't worry about loading attacks since they need to know their owner, which now doesn't exist
			break;
		case U_COLLECTIBLE:
			if(a.HasMember("armor_type"))
			{
				gpEventSystem->fireEvent(new UnitAddEvent(UNIT_ADD_EVENT, new ArmorPickup(a)));
			}
			else
			{
				gpEventSystem->fireEvent(new UnitAddEvent(UNIT_ADD_EVENT, new Collectible(a)));
			}
			break;
		case U_ENTITY:
			gpEventSystem->fireEvent(new UnitAddEvent(UNIT_ADD_EVENT, new Entity(a)));
			break;
		case U_WALL:
			//let level handle this one since it already has the code for creating the wall mask and walls are constant anyways
			break;
		}
	}
}

void UnitManager::resetLevel(Vector2D &snakeSpawn)
{
	for(int i = mUnits.size() - 1; i >= 0; i--)
	{
		if(mUnits[i]->tryRemove())
		{
			//might want to fire a remove event if other classes become listeners to that
			delete mUnits[i];
			mUnits.erase(mUnits.begin() + i);
		}
	}
}

void UnitManager::save(rapidjson::Document &doc, rapidjson::Document::AllocatorType &alloc)
{
	rapidjson::Value units(rapidjson::kArrayType);
	for(auto &a : mUnits)
	{
		rapidjson::Value obj(rapidjson::kObjectType);
		rapidjson::Value val(rapidjson::kObjectType);
		a->save(obj, val, alloc);
		if(obj.HasMember("facing_left"))
		{
			units.PushBack(obj, alloc);
		}
	}
	doc.AddMember("units", units, alloc);
}

void UnitManager::update(int timeElapsed)
{
	gpPerformanceTracker->startTracking("unit update");
	for(unsigned int i = 0; i < mUnits.size(); i++)
	{
		if(!mUnits[i]->mRemove)
		{
			mUnits[i]->update(timeElapsed, mWallMask);
		}
	}
	gpPerformanceTracker->stopTracking("unit update");

	gpPerformanceTracker->startTracking("collision update");
	if(mMovingUnits.size() > 0)
	{
		CollisionComponent &collision = mUnits[0]->mCollision;
		for(unsigned int i = 0; i < mMovingUnits.size() - 1; i++)
		{
			for(unsigned int j = i + 1; j < mMovingUnits.size(); j++)
			{
				{
					if(mMovingUnits[i]->mType == U_ENTITY)
					{
						if(mMovingUnits[j]->mType == U_ATTACK)
						{
							collision.resolveCollision(static_cast<Entity*>(mMovingUnits[i]), static_cast<Attack*>(mMovingUnits[j]));
						}
						else if(mMovingUnits[j]->mType == U_COLLECTIBLE)
						{
							collision.resolveCollision(static_cast<Entity*>(mMovingUnits[i]), static_cast<Collectible*>(mMovingUnits[j]));
						}
						else if(mMovingUnits[j]->mType == U_ENTITY)
						{
							collision.resolveCollision(static_cast<Entity*>(mMovingUnits[i]), static_cast<Entity*>(mMovingUnits[j]));
						}
					}
					else if(mMovingUnits[i]->mType == U_ATTACK)
					{
						if(mMovingUnits[j]->mType == U_ENTITY)
						{
							collision.resolveCollision(static_cast<Entity*>(mMovingUnits[j]), static_cast<Attack*>(mMovingUnits[i]));
						}
					}
					else if(mMovingUnits[i]->mType == U_COLLECTIBLE)
					{
						if(mMovingUnits[j]->mType == U_ENTITY)
						{
							collision.resolveCollision(static_cast<Entity*>(mMovingUnits[j]), static_cast<Collectible*>(mMovingUnits[i]));
						}
					}
				}
			}
		}
	}
	gpPerformanceTracker->stopTracking("collision update");
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
		for(int i = mUnits.size() - 1; i >= 0; i--)
		{
			if(mUnits[i] == ev.getToRemove())
			{
				deleteUnit(i);
			}
		}
	}
	else if(theEvent.getType() == DIFFICULTY_CHANGE_EVENT)
	{
		//loop through all objects, tell them to modify themselves by the multipliers
		float multiplier = static_cast<const DifficultyChangeEvent&>(theEvent).getMultiplier();
		for(unsigned int i = 0; i < mUnits.size(); i++)
		{
			mUnits[i]->modifyDifficulty(multiplier);
		}
	}
}

bool UnitManager::queryWall(int x, int y)
{
	if(x < 0 || y < 0 || (int)mWallMask.size() <= y / 40 || (int)mWallMask[y / 40].size() <= x / 40)
		return false;
	else
		return mWallMask[y / 40][x / 40];
}

void UnitManager::setLevelEnded(bool torf)
{
	mLevelEnded = torf;
}

void UnitManager::setWallMask(const std::vector<std::vector<bool>> &wallMask, std::vector<Wall*> &walls)
{
	mWallMask.clear();
	mWallMask = wallMask;

	//mWallMaskPointers.reserve(mWallMask.size());
	mWallMaskPointers.resize(mWallMask.size());
	for(unsigned int i = 0; i < mWallMaskPointers.size(); i++)
	{
		mWallMaskPointers[i].resize(mWallMask[i].size());
	}

	for(unsigned int i = 0; i < mWallMaskPointers.size(); i++)
	{
		for(unsigned int j = 0; j < mWallMaskPointers[i].size(); j++)
		{
			mWallMaskPointers[i][j] = NULL;
		}
	}

	for(unsigned int i = 0; i < walls.size(); i++)
	{
		int x = (int)walls[i]->mPosition.getX() / 40;
		int y = (int)walls[i]->mPosition.getY() / 40;
		int j = x;
		while((unsigned)j < mWallMask[y].size())
		{
			if(mWallMask[y][j])
			{
				mWallMaskPointers[y][j] = walls[i];
			}
			j++;
		}
	}

	for(unsigned int i = 0; i < walls.size(); i++)
	{
		addUnit(walls[i]);
	}
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

	gpEventSystem->removeListenerFromAllEvents(this);
}