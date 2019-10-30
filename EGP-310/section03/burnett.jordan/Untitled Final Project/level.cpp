#include "level.h"
#include "game.h"

Level::Level(std::string &filename, int levelNumber)
{
	mLevelNumber = levelNumber;
	mFilename = filename;
	mVictoryItemsFound = 0;
	mVictoryItemsRequired = 0;

	//rapidjson::Document &doc = JSONData::getInstance()->getJSON();
	std::string jsonLevelPath = "level" + std::to_string(mLevelNumber + 1);
}

Level::~Level()
{
	for(int i = mItemSpawns.size() - 1; i >= 0; i--)
	{
		//delete mItemSpawns[i];
	}
}

void Level::update(int timeElapsed)
{
	for (ItemSpawner* x : mItemSpawns)
	{
		if (x->getCanSpawn() && (float)(rand() % 100) / 100 < x->getSpawnProbability())
			x->spawn();
	}
}

void Level::incrementVictoryCount()
{
	mVictoryItemsFound++;
	if(mVictoryItemsFound >= mVictoryItemsRequired)
	{
		gpEventSystem->fireEvent(new LevelEndEvent(LEVEL_END_EVENT));
	}
}

void Level::restart()
{
	mVictoryItemsFound = 0;
	for(int i = mItemSpawns.size() - 1; i >= 0; i--)
	{
		delete mItemSpawns[i];
	}
	load();
}

void Level::save(rapidjson::Document &doc, rapidjson::Document::AllocatorType &alloc)
{
	doc.AddMember("victory_items_found", mVictoryItemsFound, alloc);
	doc.AddMember("victory_items_required", mVictoryItemsRequired, alloc);
}

void Level::addItemSpawn(float x, float y, PICKUP_TYPE type)
{
	mItemSpawns.push_back(new ItemSpawner(type, Vector2D(x * mTileSize, y * mTileSize)));
}

void Level::addWall(float x, float y, float width)
{
	gpEventSystem->fireEvent(new UnitAddEvent(UNIT_ADD_EVENT, new Wall(Vector2D(x * mTileSize, y * mTileSize), new Sprite(*(Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer(JSONData::getInstance()->getJSON()["wall"]["spritesheetName"].GetString()))), width)));
}

void Level::addBowEnemy(float x, float y)
{
	gpEventSystem->fireEvent(new UnitAddEvent(UNIT_ADD_EVENT, new Entity(Vector2D(x * mTileSize, y * mTileSize), Game::getInstance()->getInputManager().getNewProjectileEntityInputComponent(), "bow enemy")));
}

void Level::addMeleeEnemy(float x, float y)
{
	gpEventSystem->fireEvent(new UnitAddEvent(UNIT_ADD_EVENT, new Entity(Vector2D(x * mTileSize, y * mTileSize), Game::getInstance()->getInputManager().getNewMeleeEntityInputComponent(), "melee enemy")));
}

void Level::addPlayer(float x, float y)
{
	PlayerInputComponent *p = Game::getInstance()->getInputManager().getNewPlayerInputComponent();
	Entity *e = new Entity(Vector2D(x * mTileSize, y * mTileSize), p, "melee enemy");
	gpEventSystem->fireEvent(new UnitAddEvent(UNIT_ADD_EVENT, e));
	mPlayerSpawn = e->getPosition();
	//gpEventSystem->fireEvent(UnitAddEvent(UNIT_ADD_EVENT, new Entity(Vector2D(x * mTileSize, y * mTileSize), new Animation(*Game::getInstance()->getUnitManager().getUnitAnimations()[JSONData::getInstance()->getJSON()["melee enemy"]["spritesheetName"].GetString()]), new PlayerInputComponent())));
}
void Level::addItem(float x, float y, PICKUP_TYPE type)
{
	// Deprecated except for Victory Items, add a spawner instead
	rapidjson::Value &doc = JSONData::getInstance()->getJSON();
	switch (type) {
	/*case P_JUMP:
		item = new Collectible(Vector2D(x * mTileSize, y * mTileSize), new Sprite(*(Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer(doc["pickup_jumpboost"]["spritesheetName"].GetString()))), P_JUMP, doc["pickup_jumpboost"]["duration"].GetInt(), doc["pickup_jumpboost"]["power"].GetFloat());
		addItemSpawn(x*mTileSize, y*mTileSize, type);
		item->mSpawner = mItemSpawns[mItemSpawns.size() - 1];
		gpEventSystem->fireEvent(new UnitAddEvent(UNIT_ADD_EVENT, item));
		break;
	case P_SPEED:
		item = new Collectible(Vector2D(x * mTileSize, y * mTileSize), new Sprite(*(Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer(doc["pickup_speedup"]["spritesheetName"].GetString()))), P_SPEED, doc["pickup_speedup"]["duration"].GetInt(), doc["pickup_speedup"]["power"].GetFloat());
		addItemSpawn(x*mTileSize, y*mTileSize, type);
		item->mSpawner = mItemSpawns[mItemSpawns.size() - 1];
		gpEventSystem->fireEvent(new UnitAddEvent(UNIT_ADD_EVENT, item));
		break;
	case P_ARMOR:
		item = new ArmorPickup(Vector2D(x * mTileSize, y * mTileSize), new Sprite((*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer(doc["pickup_armor"]["spritesheetName"].GetString()))), ArmorManager::getRandomArmor());
		addItemSpawn(x*mTileSize, y*mTileSize, type);
		item->mSpawner = mItemSpawns[mItemSpawns.size() - 1];
		gpEventSystem->fireEvent(new UnitAddEvent(UNIT_ADD_EVENT, item));
		break;*/
	case P_VICTORY_ITEM:
		gpEventSystem->fireEvent(new UnitAddEvent(UNIT_ADD_EVENT, new Collectible(Vector2D(x * mTileSize, y * mTileSize), new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer(doc["pickup_victory_item"]["spritesheetName"].GetString())), P_VICTORY_ITEM, 0, 0)));
	}
}

void Level::load()
{
	rapidjson::Document &doc = JSONData::getInstance()->getJSON();
	mTileSize = doc["game"]["tileSize"].GetInt();

	mLevelString = "level" + std::to_string(mLevelNumber + 1);

	//mItemFrequency = doc[mLevelString.c_str()]["itemFrequency"].GetInt();

	/*std::ifstream fin(mFilename);
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
		}
		y++;
	}

	fin.close();
	*/
	mVictoryItemsRequired = 0;
	int y = 0, wallBegin = 0;
	std::string test = "levels/" + mLevelString + doc[mLevelString.c_str()]["levelMask"].GetString();
	std::ifstream fin(test);
	std::string line;
	std::vector<std::vector<bool>> wallMask;
	std::vector<Wall*> walls;
	while(std::getline(fin, line))
	{
		int numWalls = 0;
		wallMask.push_back(std::vector<bool>());
		for(unsigned int i = 0; i < line.length(); i++)
		{
			wallMask[y].push_back(false);
			if(line[i] != 'w' && numWalls > 0)
			{
				//addWall((float)wallBegin, (float)y, (float)numWalls);
				walls.push_back(new Wall(Vector2D((float)wallBegin * mTileSize, (float)y * mTileSize), new Sprite(*(Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer(JSONData::getInstance()->getJSON()["wall"]["spritesheetName"].GetString()))), (float)numWalls));
				numWalls = 0;
			}
			switch (line[i])
			{
			case 'w':
				if(i != 0 && line[i - 1] != 'w')
				{
					wallBegin = i;
				}
				wallMask[y][i] = true;
				numWalls++;
				//addWall(i, y);
				break;
			case 'm':
				addMeleeEnemy((float)i, (float)y);
				break;
			case 'b':
				addBowEnemy((float)i, (float)y);
				break;
			case 'p':
				addPlayer((float)i, (float)y);
				break;
			case 'j':
				addItemSpawn((float)i, (float)y, P_JUMP);
				break;
			case 's':
				addItemSpawn((float)i, (float)y, P_SPEED);
				break;
			case 'a':
				addItemSpawn((float)i, (float)y, P_ARMOR);
				break;
			case 'W':
				addItemSpawn((float)i, (float)y, P_WEAPON);
				break;
			case 'v':
				addItem((float)i, (float)y, P_VICTORY_ITEM);
				mVictoryItemsRequired++;
				break;
			}
		}
		y++;
		wallBegin = 0;
	}

	Game::getInstance()->getUnitManager().setWallMask(wallMask, walls);
}

void Level::loadWallsOnly()
{
	rapidjson::Document &doc = JSONData::getInstance()->getJSON();
	mTileSize = doc["game"]["tileSize"].GetInt();

	mLevelString = "level" + std::to_string(mLevelNumber + 1);

	int y = 0, wallBegin = 0;
	std::string test = "levels/" + mLevelString + doc[mLevelString.c_str()]["levelMask"].GetString();
	std::ifstream fin(test);
	std::string line;
	std::vector<std::vector<bool>> wallMask;
	std::vector<Wall*> walls;
	while(std::getline(fin, line))
	{
		int numWalls = 0;
		wallMask.push_back(std::vector<bool>());
		for(unsigned int i = 0; i < line.length(); i++)
		{
			wallMask[y].push_back(false);
			if(line[i] != 'w' && numWalls > 0)
			{
				//addWall((float)wallBegin, (float)y, (float)numWalls);
				walls.push_back(new Wall(Vector2D((float)wallBegin * mTileSize, (float)y * mTileSize), new Sprite(*(Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer(JSONData::getInstance()->getJSON()["wall"]["spritesheetName"].GetString()))), (float)numWalls));
				numWalls = 0;
			}
			switch(line[i])
			{
			case 'w':
				if(i != 0 && line[i - 1] != 'w')
				{
					wallBegin = i;
				}
				wallMask[y][i] = true;
				numWalls++;
				//addWall(i, y);
				break;
			}
		}
		y++;
		wallBegin = 0;
	}

	Game::getInstance()->getUnitManager().setWallMask(wallMask, walls);
}

void Level::spawnItem(Unit *item)
{
	/*std::vector<Vector2D> tempItemSpawns = mItemSpawns;
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
	}*/
}