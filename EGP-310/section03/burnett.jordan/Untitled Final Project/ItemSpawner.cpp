#include "ItemSpawner.h"
#include "game.h"

ItemSpawner::ItemSpawner(PICKUP_TYPE type, Vector2D pos)
{
	mPos = pos;
	mType = type;
	spawn();
	mShouldSpawn = false;
	mSpawnProbability = 0.001f;
}

ItemSpawner::~ItemSpawner()
{
}

void ItemSpawner::spawn()
{
	rapidjson::Value &doc = JSONData::getInstance()->getJSON();
	Collectible* spawn;
	WeaponType type;
	int level;
	Armor* armor;
	Weapon* weapon;
	switch (mType) {
	case P_JUMP:
		spawn = new Collectible(mPos, new Sprite(*(Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer(doc["pickup_jumpboost"]["spritesheetName"].GetString()))), P_JUMP, doc["pickup_jumpboost"]["duration"].GetInt(), doc["pickup_jumpboost"]["power"].GetFloat());
		break;
	case P_SPEED:
		spawn = new Collectible(mPos, new Sprite(*(Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer(doc["pickup_speedup"]["spritesheetName"].GetString()))), P_SPEED, doc["pickup_speedup"]["duration"].GetInt(), doc["pickup_speedup"]["power"].GetFloat());
		break;
	case P_ARMOR:
		armor = ArmorManager::getRandomArmor();
		spawn = new ArmorPickup(mPos, new Animation(*Game::getInstance()->getUnitManager().getUnitAnimations()[armor->getType() + "_pickup_animation"]), armor);
		//spawn->mAlternateColor=doc[]
		break;
	case P_WEAPON:
		type = WeaponType(std::rand() % W_NUMTYPES);
		level = std::rand() % 3;	//Magic number bad
		weapon = new Weapon(type, level);
		spawn = new WeaponPickup(mPos, new Animation(*Game::getInstance()->getUnitManager().getUnitAnimations()[weapon->getTypeName() + "_pickup_animation"]), weapon);
		break;
	default:
		spawn = new Collectible(mPos, (Sprite*)NULL, P_INVALID);
		break;
	}
	if(spawn->mAlternateColor)
	{

	}
	spawn->mSpawner = this;
	gpEventSystem->fireEvent(new UnitAddEvent(UNIT_ADD_EVENT, spawn));
	mShouldSpawn = false;
}
