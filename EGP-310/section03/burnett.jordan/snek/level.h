#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "unit.h"
#include "EventSystem.h"
#include "wall.h"
#include "SnakeIncrementPowerUp.h"
#include "scorePowerUp.h"
#include "speedPowerUp.h"
#include "unitAddEvent.h"
#include "sprite.h"
#include "jsonData.h"

class Level
{
	friend class LevelManager;
public:
	Level(std::string &filename, int levelNumber);
	~Level();

	void spawnFood();

	void update(double timeElapsed);

	//getters
	int getFoodScore(){ return mFoodScore; }
	int getScorePowerupScore(){ return mScorePowerupScore; }
	int getSpeedScore(){ return mSpeedScore; }

	std::string &getLevelString(){ return mLevelString; }

	Vector2D &getSnakeSpawn(){ return mSnakeSpawn; }
private:
	void addItemSpawn(int x, int y);
	void addSnake(int x, int y);
	void addWall(int x, int y);
	void load();
	void spawnItem(Unit *item);

	int mTileSize; //width/height of walls
	int mLevelNumber;
	std::string mFilename;
	int mItemFrequency;

	int mFoodScore;
	int mScorePowerupScore;
	int mSpeedScore;

	int mTargetSegments; //when snake eats this many food pieces, increment level

	std::string mLevelString;

	Vector2D mSnakeSpawn;
	std::vector<Vector2D> mItemSpawns;
	Snake *mpSnake; //for spawning items without spawning on top of snake
};