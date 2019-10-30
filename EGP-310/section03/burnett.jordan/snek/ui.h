#pragma once
#include "EventListener.h"
#include "event.h"
#include "incrementLivesEvent.h"
#include "incrementScoreEvent.h"
#include "updateFPSEvent.h"
#include "Vector2D.h"
#include "font.h"
#include "color.h"

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
private:
	int mFps, mLives, mScore;
	Vector2D mPos;
	Font *mFont;
	Color mUIColor;

	std::string mStartString, mDeathString, mFailedString, mCompleteString, mReplayString, mReplayStringLine2;
};