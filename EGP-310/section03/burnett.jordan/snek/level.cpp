#include "level.h"
#include "game.h"

Level::Level(std::string &filename, int levelNumber)
{
	mLevelNumber = levelNumber;
	mFilename = filename;

	rapidjson::Document &doc = JSONData::getInstance()->getJSON();
	std::string jsonLevelPath = "level" + std::to_string(mLevelNumber + 1);
	mFoodScore = doc[jsonLevelPath.c_str()]["foodScore"].GetInt();
	mScorePowerupScore = doc[jsonLevelPath.c_str()]["scorePowerUpScore"].GetInt();
	mSpeedScore = doc[jsonLevelPath.c_str()]["speedPowerUpScore"].GetInt();
}

Level::~Level()
{
}

void Level::update(double timeElapsed)
{
	if(rand() % 1000 < mItemFrequency)
	{
		int randItem = rand() % 3;
		Unit *item = NULL;
		switch(randItem)
		{
		case 0:
			item = new ScorePowerUp(Vector2D(0, 0), new Animation(*Game::getInstance()->getUnitManager().getUnitAnimations()[JSONData::getInstance()->getJSON()["scorePowerUp"]["spritesheetName"].GetString()]));
			break;
		case 1:
			item = new SpeedPowerUp(Vector2D(0, 0), new Animation(*Game::getInstance()->getUnitManager().getUnitAnimations()[JSONData::getInstance()->getJSON()["speedPowerUp"]["spritesheetName"].GetString()]), 1);
			break;
		case 2:
			item = new SpeedPowerUp(Vector2D(0, 0), new Animation(*Game::getInstance()->getUnitManager().getUnitAnimations()[JSONData::getInstance()->getJSON()["speedPowerUp"]["spritesheetName"].GetString()]), -1);
			break;
		default:
			item = new ScorePowerUp(Vector2D(0, 0), new Animation(*Game::getInstance()->getUnitManager().getUnitAnimations()[JSONData::getInstance()->getJSON()["scorePowerUp"]["spritesheetName"].GetString()]));
			break;
		}
		spawnItem(item);
	}
}

void Level::addItemSpawn(int x, int y)
{
	mItemSpawns.push_back(Vector2D(x * mTileSize, y * mTileSize));
}

void Level::addSnake(int x, int y)
{
	rapidjson::Document &doc = JSONData::getInstance()->getJSON();
	mSnakeSpawn = Vector2D(x * mTileSize, y * mTileSize);
	mpSnake = new Snake(mSnakeSpawn, new Animation(*Game::getInstance()->getUnitManager().getUnitAnimations()[JSONData::getInstance()->getJSON()["snake"]["spritesheetName"].GetString()]), doc[mLevelString.c_str()]["startVelocityTier"].GetInt());
	gpEventSystem->fireEvent(UnitAddEvent(UNIT_ADD_EVENT, mpSnake));
	mpSnake->setTimeToMove(doc[mLevelString.c_str()]["timeToMoveSnake"].GetInt());
}

void Level::addWall(int x, int y)
{
	gpEventSystem->fireEvent(UnitAddEvent(UNIT_ADD_EVENT, new Wall(Vector2D(x * mTileSize, y * mTileSize), new Sprite(*(Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer(JSONData::getInstance()->getJSON()["wall"]["spritesheetName"].GetString()))))));
}

void Level::load()
{
	rapidjson::Document &doc = JSONData::getInstance()->getJSON();
	mTileSize = doc["game"]["tileSize"].GetInt();

	mLevelString = "level" + std::to_string(mLevelNumber + 1);

	mTargetSegments = doc[mLevelString.c_str()]["targetSegments"].GetInt();

	mItemFrequency = doc[mLevelString.c_str()]["itemFrequency"].GetInt();

	std::ifstream fin(mFilename);
	std::string line;
	int y = 0;
	while(std::getline(fin, line))
	{
		for(unsigned int i = 0; i < line.length(); i++)
		{
			if(line[i] == 'w')
			{
				addWall(i, y);
			}
			else if(line[i] == 's')
			{
				addSnake(i, y);
				addItemSpawn(i, y);
			}
			else if(line[i] == '.')
			{
				addItemSpawn(i, y);
			}
			//for debugging purposes
			else if(line[i] == 'l')
			{
				gpEventSystem->fireEvent(UnitAddEvent(UNIT_ADD_EVENT, new SnakeIncrementPowerUp(Vector2D(i * mTileSize, y * mTileSize), new Animation(*Game::getInstance()->getUnitManager().getUnitAnimations()[JSONData::getInstance()->getJSON()["food"]["spritesheetName"].GetString()]))));
			}
			else if(line[i] == 'u')
			{
				gpEventSystem->fireEvent(UnitAddEvent(UNIT_ADD_EVENT, new SpeedPowerUp(Vector2D(i * mTileSize, y * mTileSize), new Animation(*Game::getInstance()->getUnitManager().getUnitAnimations()[JSONData::getInstance()->getJSON()["speedPowerUp"]["spritesheetName"].GetString()]), 1)));
			}
			else if(line[i] == 'd')
			{
				gpEventSystem->fireEvent(UnitAddEvent(UNIT_ADD_EVENT, new SpeedPowerUp(Vector2D(i * mTileSize, y * mTileSize), new Animation(*Game::getInstance()->getUnitManager().getUnitAnimations()[JSONData::getInstance()->getJSON()["speedPowerUp"]["spritesheetName"].GetString()]), -1)));
			}
		}
		y++;
	}

	spawnFood();

	fin.close();
}

void Level::spawnItem(Unit *item)
{
	std::vector<Vector2D> tempItemSpawns = mItemSpawns;
	bool found = false;
	while(tempItemSpawns.size() > 0)
	{
		int randIndex = rand() % tempItemSpawns.size();
		Vector2D randSpawn = tempItemSpawns[randIndex];
		item->setPosition(randSpawn);
		bool collided = false;
		for(unsigned int i = 0; i < Game::getInstance()->getUnitManager().getUnits().size(); i++)
		{
			if(mpSnake == Game::getInstance()->getUnitManager().getUnits()[i])
			{
				if(mpSnake->squareCollision(item))
				{
					tempItemSpawns.erase(tempItemSpawns.begin() + randIndex);
					collided = true;
					break;
				}
			}
			else if(item->squareCollision(Game::getInstance()->getUnitManager().getUnits()[i]))
			{
				tempItemSpawns.erase(tempItemSpawns.begin() + randIndex);
				collided = true;
				break;
			}
		}
		if(!collided)
		{
			gpEventSystem->fireEvent(UnitAddEvent(UNIT_ADD_EVENT, item));
			found = true;
			break;
		}
	}
	if(!found)
	{
		delete item;
	}
}

void Level::spawnFood()
{
	spawnItem(new SnakeIncrementPowerUp(Vector2D(0, 0), new Animation(*Game::getInstance()->getUnitManager().getUnitAnimations()[JSONData::getInstance()->getJSON()["food"]["spritesheetName"].GetString()])));
}