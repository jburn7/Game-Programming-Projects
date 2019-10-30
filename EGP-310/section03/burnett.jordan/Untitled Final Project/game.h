#pragma once
#include "graphicsSystem.h"
#include "unitManager.h"
#include "graphicsBufferManager.h"
#include "playerDeathEvent.h"
#include "audioManager.h"
#include "levelManager.h"
#include "ui.h"
#include "inputSystem.h"
#include "inputManager.h"
#include "ArmorManager.h"
#include "Timer.h"
#include "PerformanceTracker.h"

enum Gamestate
{
	START,
	PLAYING,
	MENU,
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

	void unpauseClock();

	//event functions
	virtual void handleEvent(const Event& theEvent);
	
	//getters
	Gamestate &getGamestate(){ return mGamestate; }
	GraphicsBufferManager &getGraphicsBufferManager();
	GraphicsSystem &getGraphics();
	InputManager &getInputManager();
	InputSystem &getInputSystem();
	LevelManager &getLevelManager(){ return mLevelManager; }
	UnitManager &Game::getUnitManager();
private:
	Game();
	~Game();

	void restartGame();
	void restartLevel();
	void saveGame();

	Gamestate mGamestate;

	std::map<std::string, Animation*> loadUnitAnimations(rapidjson::Value &a);

	void processInput();
	void update(int timeElapsed);
	void render();

	AudioManager mAudioManager;
	GraphicsBufferManager mGraphicsBufferManager;
	InputManager mInputManager;
	InputSystem mInputSystem;
	GraphicsSystem mGraphics;
	LevelManager mLevelManager;
	UnitManager mUnitManager;

	Font mFont;
	UI *mUi;

	bool mQuit, mJustReset, mCleaned, mLevelEnded;
	int mLives;
	int mTimeOfDeath, mTimeDeathLasts;
	Timer mClock;
	Timer mFpsTimer;
	int mElapsedTime;
	int mFrames;
	Difficulty mCurrentDifficulty;

	const char *JSONPATH = "game";

	static Game *msInstance;
};