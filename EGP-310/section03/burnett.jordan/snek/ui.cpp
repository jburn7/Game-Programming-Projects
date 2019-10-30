#include "ui.h"
#include "game.h"

UI::UI()
{
	mPos.setX(200);
	mPos.setY(200);
	mFps = mScore = 0;
	rapidjson::Document &doc = JSONData::getInstance()->getJSON();
	mLives = doc["game"]["startingLives"].GetInt();
	int r, g, b;
	rapidjson::Value &ui = doc["ui"];
	r = ui["rColor"].GetInt();
	g = ui["gColor"].GetInt();
	b = ui["bColor"].GetInt();
	mUIColor = Color(r, g, b);

	mStartString = ui["startString"].GetString();
	mDeathString = ui["deathString"].GetString();
	mFailedString = ui["failedString"].GetString();
	mCompleteString = ui["completeString"].GetString();
	mReplayString = ui["replayString"].GetString();
	mReplayStringLine2 = ui["replayStringLine2"].GetString();

	gpEventSystem->addListener(UPDATE_FPS_EVENT, this);
	gpEventSystem->addListener(INCREMENT_LIVES_EVENT, this);
	gpEventSystem->addListener(INCREMENT_SCORE_EVENT, this);
}

UI::~UI()
{
}

void UI::handleEvent(const Event& theEvent)
{
	if(theEvent.getType() == INCREMENT_LIVES_EVENT)
	{
		const IncrementLivesEvent &ev = static_cast<const IncrementLivesEvent&>(theEvent);
		mLives += ev.getIncr();
	}
	else if(theEvent.getType() == INCREMENT_SCORE_EVENT)
	{
		const IncrementScoreEvent &ev = static_cast<const IncrementScoreEvent&>(theEvent);
		mScore += ev.getScore();
	}
	else if(theEvent.getType() == UPDATE_FPS_EVENT)
	{
		const UpdateFPSEvent &ev = static_cast<const UpdateFPSEvent&>(theEvent);
		mFps = ev.getFPS();
	}
}

void UI::draw()
{
	GraphicsSystem &graphics = Game::getInstance()->getGraphics();
	int width = graphics.getWidth();
	int height = graphics.getHeight();
	graphics.writeText(Vector2D(0, 0), *mFont, mUIColor, "FPS: " + std::to_string(mFps));
	graphics.writeText(Vector2D(width / 3, 0), *mFont, mUIColor, "Lives: " + std::to_string(mLives));
	graphics.writeText(Vector2D((2 * width / 3), 0), *mFont, mUIColor, "Score: " + std::to_string(mScore));
	
	Gamestate &gamestate = Game::getInstance()->getGamestate();
	switch(gamestate)
	{
	case START:
		//need to center this
		graphics.writeText(Vector2D(width / 2 - mFont->getWidth(mStartString) / 2, height / 2), *mFont, mUIColor, mStartString);
		break;
	case DEATH:
		graphics.writeText(Vector2D(width / 2 - mFont->getWidth(mDeathString) / 2, height / 2), *mFont, mUIColor, mDeathString);
		break;
	case FAILED:
		graphics.writeText(Vector2D(width / 2 - mFont->getWidth(mFailedString) / 2, height / 2), *mFont, mUIColor, mFailedString);
		graphics.writeText(Vector2D(width / 2 - mFont->getWidth(mReplayString) / 2, height / 2 + mFont->getFontSize()), *mFont, mUIColor, mReplayString);
		graphics.writeText(Vector2D(width / 2 - mFont->getWidth(mReplayStringLine2) / 2, height / 2 + 2 * mFont->getFontSize()), *mFont, mUIColor, mReplayStringLine2);
		break;
	case COMPLETE:
		graphics.writeText(Vector2D(width / 2 - mFont->getWidth(mCompleteString) / 2, height / 2), *mFont, mUIColor, mCompleteString);
		graphics.writeText(Vector2D(width / 2 - mFont->getWidth(mReplayString) / 2, height / 2 + mFont->getFontSize()), *mFont, mUIColor, mReplayString);
		graphics.writeText(Vector2D(width / 2 - mFont->getWidth(mReplayStringLine2) / 2, height / 2 + 2 * mFont->getFontSize()), *mFont, mUIColor, mReplayStringLine2);
		break;
	}
}

void UI::update()
{
	Gamestate &gamestate = Game::getInstance()->getGamestate();
	switch(gamestate)
	{
	case START:
		break;
	case DEATH:
		break;
	case PLAYING:
		break;
	case FAILED:
		break;
	case COMPLETE:
		break;
	}
}

void UI::reset()
{
	mFps = 0;
	mLives = 0;
	mScore = 0;
}

void UI::setLives(int lives)
{
	mLives = lives;
}

void UI::setFont(Font *font)
{
	mFont = font;
}
