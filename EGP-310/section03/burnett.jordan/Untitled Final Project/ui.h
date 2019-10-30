#pragma once
#include "EventListener.h"
#include "event.h"
#include "incrementLivesEvent.h"
#include "difficultyChangeEvent.h"
#include "incrementScoreEvent.h"
#include "updateFPSEvent.h"
#include "mouseClickEvent.h"
#include "playerHealthEvent.h"
#include "loadEvent.h"
#include "Vector2D.h"
#include "font.h"
#include "color.h"
#include "uiBox.h"
#include <map>

class UI : public EventListener
{
public:
	UI();
	~UI();

	virtual void handleEvent(const Event& theEvent);

	void draw();

	void update(); //checks the game's gamestate and draws appropriate screens

	void reset();

	//setters
	void setLives(int lives);
	void setFont(Font *font);
	void setScore(int newScore);

	//getters
	int getScore();
private:
	int mFps, mLives, mScore;
	Vector2D mPos;
	Font *mFont;
	Color mUIColor;

	std::map<std::string, std::vector<UIBox>> mUIBoxes;
	std::string mActiveMenu;
	Sprite *mpMenuShader; //darken whole screen when menu is up

	std::string mStartString, mDeathString, mDeathString2, mFailedString, mCompleteString, mReplayString, mReplayStringLine2; // change to map of strings
};