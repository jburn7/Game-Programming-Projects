#include "game.h"

Game *Game::msInstance = NULL;

//private functions
Game::Game()
{
	mQuit = mJustReset = mCleaned = mLevelEnded = false;
	mGamestate = START;
}

Game::~Game()
{
	cleanup();
}

void Game::restartGame()
{
	mLevelManager.cleanup();
	mUnitManager.clear();
	mUi->reset();
	mLives = JSONData::getInstance()->getJSON()[JSONPATH]["startingLives"].GetInt();
	mUi->setLives(mLives);
	mLevelManager.init();
	mJustReset = true;
}

void Game::restartLevel()
{
	mUnitManager.clear();
	mLevelManager.getCurrentLevel().restart();
	mUi->setScore(0);
}

void Game::saveGame()
{
	rapidjson::Document &doc = JSONData::getInstance()->getSaveDoc();
	rapidjson::Document::AllocatorType &alloc = doc.GetAllocator();
	//for all units
	    //save() (all units store all the stuff they need to inside JsonData::writeSaveFile("units") which then creates a units object with an array and populates that array with objects based on whatever the unit adds
	mUnitManager.save(doc, alloc);
	mLevelManager.save(doc, alloc);

	doc.AddMember("difficulty", mCurrentDifficulty, alloc);

    //save level completion data
    //save score
    //save lives
	JSONData::getInstance()->save();
}

std::map<std::string, Animation*> Game::loadUnitAnimations(rapidjson::Value &a)
{
	std::map<std::string, Animation*> animations;
	for(auto &v : a.GetArray())
	{
		//create animation with given time, using the graphicsBuffer that spritesheet equals, add spritesheets based on the row number * tileSize and the numTiles * tileSize
		//then add it to a map name spritesheet as the key
		std::vector<Sprite*> sprites;
		int y = v["rowNumber"].GetInt();
		float width = v["width"].GetFloat();
		float height = v["height"].GetFloat();
		int xGap = v["xGap"].GetInt();
		int yGap = v["yGap"].GetInt();
		for(int i = 0; i < v["numTiles"].GetInt(); i++)
		{
			Sprite *sprite = new Sprite(*mGraphicsBufferManager.getGraphicsBuffer(v["spritesheet"].GetString()));
			float sourceLocX = i * width;
			float sourceLocY = y * height;
			if(i != 0)
			{
				sourceLocX += xGap;
				sourceLocY += yGap;
			}
			/*sprite->setSourceLoc(i * width, y * height);*/
			sprite->setSourceLoc(sourceLocX, sourceLocY);
			sprite->setWidth(width);
			sprite->setHeight(height);
			sprites.push_back(sprite);
		}
		Animation *animation = new Animation(sprites);
		animation->setSpeed(v["time"].GetInt());
		animations.insert(std::pair<std::string, Animation*>(v["name"].GetString(), animation));
	}

	return animations;
}

//public functions
Game *Game::getInstance()
{
	assert(msInstance != NULL);
	return msInstance;
}

void Game::initInstance()
{
	msInstance = new Game();
}

void Game::cleanupInstance()
{
	if(msInstance)
	{
		delete msInstance;
		msInstance = NULL;
	}
}

void Game::init()
{
	//initialize JSON
	JSONData::init("data/game_data.json");
	rapidjson::Document &doc = JSONData::getInstance()->getJSON();

	mAudioManager.init();

	//init graphics
	mGraphics.init(doc[JSONPATH]["width"].GetFloat(), doc[JSONPATH]["height"].GetFloat());
	mInputSystem.init();

	mLives = doc[JSONPATH]["startingLives"].GetInt();

	mTimeDeathLasts = doc[JSONPATH]["deathTime"].GetInt();
	
	const std::string assetsPath = doc[JSONPATH]["assetsPath"].GetString();

	//add all names for graphics buffers, then loop through and add them
	std::vector<std::string> graphicsBuffersNames;
	rapidjson::Value &a = doc[JSONPATH]["graphicsBuffers"];
	for(auto& v : a.GetArray())
	{
		graphicsBuffersNames.push_back(v.GetString());
	}

	//add all names for animating graphics buffers, loop through and add
	for(unsigned int i = 0; i < graphicsBuffersNames.size(); i++)
	{
		mGraphicsBufferManager.addGraphicsBuffer(graphicsBuffersNames[i], new GraphicsBuffer(assetsPath + graphicsBuffersNames[i]));
	}

	a = doc[JSONPATH]["animations"];

	//load animations for mUnitManager
	mUnitManager.setUnitAnimations(loadUnitAnimations(a));

	ArmorManager::init();
	mLevelManager.init();

	mElapsedTime = 0;

	mUi = new UI;

	mCurrentDifficulty = D_EASY;
	
	gpEventSystem->addListener(DIFFICULTY_CHANGE_EVENT, this);
	gpEventSystem->addListener(LEVEL_END_EVENT, this);
	gpEventSystem->addListener(LOAD_EVENT, this);
	gpEventSystem->addListener(PAUSE_EVENT, this);
	gpEventSystem->addListener(PLAYER_DEATH_EVENT, this);
	gpEventSystem->addListener(QUIT_EVENT, this);
	gpEventSystem->addListener(INCREMENT_LIVES_EVENT, this);
	gpEventSystem->addListener(START_EVENT, this);
	gpEventSystem->addListener(VICTORY_ITEM_FOUND_EVENT, this);
}

void Game::cleanup()
{
	if(!mCleaned){
		msInstance->saveGame();

		if(mUi)
		{
			delete mUi;
			mUi = NULL;
		}

		ArmorManager::cleanup();
		mGraphicsBufferManager.cleanup();
		mGraphics.cleanup();
		mInputSystem.cleanup();
		mLevelManager.cleanup();
		mUnitManager.cleanup();

		mCleaned = true;
	}
}

void Game::complete()
{
	mGamestate = COMPLETE;
}

void Game::loop()
{
	double timePerFrame = 16.7777777777; //should be accurate enough
	double timeOfLastFrame = 0;
	mFrames = 0;
	while(!mQuit)
	{
		gpEventSystem->dispatchAllEvents();

		mFpsTimer.start();

		gpPerformanceTracker->startTracking("input");
		processInput();
		gpPerformanceTracker->stopTracking("input");

		gpPerformanceTracker->startTracking("update");
		update((int)mClock.getElapsedTime() + mElapsedTime);
		gpPerformanceTracker->stopTracking("update");

		gpPerformanceTracker->startTracking("render");
		render();
		gpPerformanceTracker->stopTracking("render");

		if(mGamestate == PLAYING)
			mFrames++;
		double fps = (double)mFrames / ((mClock.getElapsedTime() + (double)mElapsedTime)) * 1000.0;
		gpEventSystem->fireEvent(new UpdateFPSEvent(UPDATE_FPS_EVENT, (int)fps));

		gpPerformanceTracker->startTracking("sleep");
		mFpsTimer.sleepUntilElapsed(timePerFrame);
		gpPerformanceTracker->stopTracking("sleep");

		//timeOfLastFrame = mClock.getElapsedTime();
	}
	mClock.stop();
}

void Game::unpauseClock()
{
	mClock.start();
}

void Game::handleEvent(const Event &theEvent)
{
	if(theEvent.getType() == QUIT_EVENT)
	{
		mQuit = true;
	}
	else if(theEvent.getType() == INCREMENT_LIVES_EVENT)
	{
		const IncrementLivesEvent &ev = static_cast<const IncrementLivesEvent&>(theEvent);
		mLives += ev.getIncr();
		if(mLives <= 0)
		{
			std::cout << "Game Over, all lives lost.\n";
			mGamestate = FAILED;
		}
		else if(ev.getIncr() < 0)
		{
			mTimeOfDeath = (int)mClock.getElapsedTime();
			mGamestate = DEATH;
		}
	}
	else if(theEvent.getType() == START_EVENT)
	{
		if(mGamestate != START)
		{
			if(mGamestate == FAILED || mGamestate == COMPLETE)
			{
				restartGame();
			}
			else if(mGamestate == DEATH)
			{
				restartLevel();
			}
			mGamestate = PLAYING;
		}
	}
	else if(theEvent.getType() == LEVEL_END_EVENT)
	{
		mLevelEnded = true;
	}
	else if(theEvent.getType() == LOAD_EVENT)
	{
		rapidjson::Document &doc = JSONData::getInstance()->getLoadedSaveJSON();
		if(doc.IsObject())
		{
			mUnitManager.load(doc);
			mLevelManager.loadFromFile(doc);
			mUi->setScore(mLevelManager.getCurrentLevel().getVictoryItemsFound());
			gpEventSystem->fireEvent(new DifficultyChangeEvent(DIFFICULTY_CHANGE_EVENT, Difficulty(doc["difficulty"].GetInt())));
			mClock.start();
		}
		else
		{
			std::cout << "Save file is corrupt.\n";
		}

		mGamestate = PLAYING;
	}
	else if(theEvent.getType() == PAUSE_EVENT)
	{
		mGamestate = MENU;
		mElapsedTime = (int)mClock.getElapsedTime();
		mClock.stop();
	}
	else if(theEvent.getType() == PLAYER_DEATH_EVENT)
	{
		mLives--;
		if(mLives > 0)
		{
			mGamestate = DEATH;
		}
		else
		{
			mGamestate = FAILED;
		}
		mUi->setLives(mLives);
	}
	else if(theEvent.getType() == VICTORY_ITEM_FOUND_EVENT)
	{
		mLevelManager.getCurrentLevel().incrementVictoryCount();
		mUi->setScore(mUi->getScore() + 1);
	}
	else if(theEvent.getType() == DIFFICULTY_CHANGE_EVENT)
	{
		if(mGamestate != COMPLETE)
		{
			mCurrentDifficulty = static_cast<const DifficultyChangeEvent&>(theEvent).getDifficulty();
			mGamestate = PLAYING;
		}
	}
}

GraphicsBufferManager &Game::getGraphicsBufferManager()
{
	return mGraphicsBufferManager;
}

GraphicsSystem &Game::getGraphics()
{
	return mGraphics;
}

InputManager &Game::getInputManager()
{
	return mInputManager;
}

InputSystem &Game::getInputSystem()
{
	return mInputSystem;
}

UnitManager &Game::getUnitManager()
{
	return mUnitManager;
}

void Game::processInput()
{
	mInputSystem.checkForInput();
}

void Game::update(int timeElapsed)
{
	if(mGamestate == PLAYING && !mLevelEnded)
	{
		mUnitManager.update(timeElapsed);
		mAudioManager.update();
		mGraphics.update();
		mLevelManager.getCurrentLevel().update(timeElapsed);
	}
	if(mLevelEnded)
	{
		mUnitManager.setLevelEnded(false);
		mUnitManager.clear();
		mLevelManager.loadNewLevel();
		gpEventSystem->fireEvent(new DifficultyChangeEvent(DIFFICULTY_CHANGE_EVENT, mCurrentDifficulty));
		mLevelEnded = false;
	}
}

void Game::render()
{
	mGraphics.clear();
	if(mGamestate != START)
	{
		mUnitManager.draw();
	}
	mUi->draw();
	mGraphics.flip();
}