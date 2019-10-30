#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "unit.h"
#include "EventSystem.h"
#include "wall.h"
#include "unitAddEvent.h"
#include "levelEndEvent.h"
#include "sprite.h"
#include "jsonData.h"
#include "playerInputComponent.h"
#include "meleeEntityInputComponent.h"
#include "Collectible.h"
#include "ItemSpawner.h"

class Level
{
	friend class LevelManager;
public:
	Level(std::string &filename, int levelNumber);
	~Level();

	void update(int timeElapsed);	//For respawning items

	void incrementVictoryCount();

	void restart();

	void save(rapidjson::Document &doc, rapidjson::Document::AllocatorType &alloc);

	std::string &getLevelString(){ return mLevelString; }

	Vector2D &getPlayerSpawn(){ return mPlayerSpawn; }

	int getVictoryItemsFound(){ return mVictoryItemsFound; }
private:
	void addItemSpawn(float x, float y, PICKUP_TYPE type);
	void addWall(float x, float y, float width);
	void addBowEnemy(float x, float y);
	void addMeleeEnemy(float x, float y);
	void addPlayer(float x, float y);
	void addItem(float x, float y, PICKUP_TYPE type);
	void load();
	void loadWallsOnly(); //called when using the load function in-game
	void spawnItem(Unit *item);

	int mTileSize; //width/height of walls
	int mLevelNumber;
	std::string mFilename;
	int mItemFrequency;

	int mVictoryItemsRequired, mVictoryItemsFound;

	std::string mLevelString;

	Vector2D mPlayerSpawn;
	std::vector<ItemSpawner*> mItemSpawns;


	/*
		ideas for adding level specific gameplay:
		have the level hold a container of pointers to Entities,
		and in Level::update() iterate through each of these and destroy them
		if they have fallen past a certain y value (to create a bottom edge of a level that nothing can go past)
		to maintain this collection, just make level (or levelManager) a Listener to Unit Add/Remove events,
		and add/remove pointers in the active Level accordingly

		can be done for collections of other objects as well
	*/
};