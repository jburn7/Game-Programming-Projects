#pragma once
//#include "jsonData.h"
#include "graphicsSystem.h"
//#include "EventListener.h"
#include "unitManager.h"
#include "graphicsBufferManager.h"
#include "levelManager.h"
//#include "event.h"
#include "snake.h"
#include "ui.h"
#include "inputSystem.h"
#include "Timer.h"
#include "PerformanceTracker.h"
//#include "incrementLivesEvent.h"

enum Gamestate
{
	START,
	PLAYING,
	DEATH,
	FAILED,
	COMPLETE
};

class Game : public EventListener
{
public:
	static Game *getInstance();
	static void initInstance();
	static void cleanupInstance();

	void init();

	void cleanup();

	void complete();

	void loop();

	//event functions
	virtual void handleEvent(const Event& theEvent);
	
	//getters
	Gamestate &getGamestate(){ return mGamestate; }
	GraphicsBufferManager &getGraphicsBufferManager();
	GraphicsSystem &getGraphics();
	InputSystem &getInputSystem();
	LevelManager &getLevelManager(){ return mLevelManager; }
	UnitManager &Game::getUnitManager();
private:
	Game();
	~Game();

	void restartGame();

	Gamestate mGamestate;

	std::map<std::string, Animation*> loadUnitAnimations(rapidjson::Value &a, int tileSize);

	void processInput();
	void update(double timeElapsed);
	void render();

	GraphicsBufferManager mGraphicsBufferManager;
	InputSystem mInputSystem;
	GraphicsSystem mGraphics;
	LevelManager mLevelManager;
	UnitManager mUnitManager;

	Font mFont;
	UI *mUi;

	bool mQuit, mJustReset;
	int mLives;
	int mTimeOfDeath, mTimeDeathLasts;
	Timer mClock;

	const char *JSONPATH = "game";

	static Game *msInstance;
};