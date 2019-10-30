#include "game.h"

//snake and apple spritesheet credits: http://rembound.com/files/creating-a-snake-game-tutorial-with-html5/snake-graphics.png
//https://d30y9cdsu7xlg0.cloudfront.net/png/12193-200.png

Game *Game::msInstance = NULL;

//private functions
Game::Game()
{
	mQuit = mJustReset = false;
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

std::map<std::string, Animation*> Game::loadUnitAnimations(rapidjson::Value &a, int tileSize)
{
	std::map<std::string, Animation*> animations;
	for(auto &v : a.GetArray())
	{
		//create animation with given time, using the graphicsBuffer that spritesheet equals, add spritesheets based on the row number * tileSize and the numTiles * tileSize
		//then add it to a map name spritesheet as the key
		std::vector<Sprite*> sprites;
		int y = v["rowNumber"].GetInt();
		for(int i = 0; i < v["numTiles"].GetInt(); i++)
		{
			Sprite *sprite = new Sprite(*mGraphicsBufferManager.getGraphicsBuffer(v["spritesheet"].GetString()));
			sprite->setSourceLoc(i * tileSize, y * tileSize);
			sprite->setWidth(tileSize);
			sprite->setHeight(tileSize);
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
	JSONData::init("data/snek_data.json");
	rapidjson::Document &doc = JSONData::getInstance()->getJSON();

	mGraphics.init(doc[JSONPATH]["width"].GetInt(), doc[JSONPATH]["height"].GetInt());
	mInputSystem.init();

	mLives = doc[JSONPATH]["startingLives"].GetInt();

	mTimeDeathLasts = doc[JSONPATH]["deathTime"].GetInt();
	
	mUi = new UI;

	const std::string assetsPath = doc[JSONPATH]["assetsPath"].GetString();

	//add all names for graphics buffers, then loop through and add them
	std::vector<std::string> graphicsBuffersNames;
	//http://rapidjson.org/md_doc_tutorial.html#QueryObject 
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

	//http://rapidjson.org/md_doc_tutorial.html#QueryObject 
	a = doc[JSONPATH]["animations"];
	int tileSize = doc[JSONPATH]["tileSize"].GetInt();

	//load animations for mUnitManager
	mUnitManager.setUnitAnimations(loadUnitAnimations(a, tileSize));

	mLevelManager.init();

	mFont.loadFont(std::string(doc[JSONPATH]["fontPath"].GetString()) + "cour.ttf", doc[JSONPATH]["fontSize"].GetInt());
	mUi->setFont(&mFont);

	gpEventSystem->addListener(QUIT_EVENT, this);
	gpEventSystem->addListener(INCREMENT_LIVES_EVENT, this);
	gpEventSystem->addListener(START_EVENT, this);
}

void Game::cleanup()
{
	if(mUi)
	{
		delete mUi;
		mUi = NULL;
	}

	mGraphicsBufferManager.cleanup();
	mGraphics.cleanup();
	mInputSystem.cleanup();
	mLevelManager.cleanup();
	mUnitManager.cleanup();
}

void Game::complete()
{
	mGamestate = COMPLETE;
}

void Game::loop()
{
	double timePerFrame = 16.7777777777; //should be accurate enough
	double timeOfLastFrame = 0;
	int frames = 0;
	mClock.start();
	Timer fpsTimer;
	while(!mQuit)
	{
		fpsTimer.start();
		if(mGamestate == DEATH && mClock.getElapsedTime() > mTimeOfDeath + mTimeDeathLasts)
		{
			mUnitManager.resetLevel(mLevelManager.getCurrentLevel().getSnakeSpawn());
			mLevelManager.getCurrentLevel().spawnFood();
			mGamestate = PLAYING;
		}
		processInput();
		update(mClock.getElapsedTime());
		render();
		frames++;
		double fps = frames / mClock.getElapsedTime() * 1000;
		gpEventSystem->fireEvent(UpdateFPSEvent(UPDATE_FPS_EVENT, fps));
		fpsTimer.sleepUntilElapsed(timePerFrame);
		timeOfLastFrame = mClock.getElapsedTime();
	}
	mClock.stop();
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
			mTimeOfDeath = mClock.getElapsedTime();
			mGamestate = DEATH;
		}
	}
	else if(theEvent.getType() == START_EVENT)
	{
		if(mGamestate == FAILED || mGamestate == COMPLETE)
		{
			restartGame();
		}
		mGamestate = PLAYING;
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

void Game::update(double timeElapsed)
{
	if(mGamestate == PLAYING && !mJustReset)
	{
		mLevelManager.update(timeElapsed);
		mUnitManager.update(timeElapsed);
	}
}

void Game::render()
{
	mGraphics.clear();
	//mGraphics.draw(Vector2D(0, 0), mBackgroundImage);
	mUnitManager.draw();
	mUi->draw();
	mGraphics.flip();
	if(mJustReset)
	{
		//simulate the snake pause so the first movement of the new level is visible
		Sleep(JSONData::getInstance()->getJSON()[mLevelManager.getCurrentLevel().getLevelString().c_str()]["timeToMoveSnake"].GetInt());
		mJustReset = false;
	}
}