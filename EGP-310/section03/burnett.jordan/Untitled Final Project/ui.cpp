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

	setFont(new Font(doc["ui"]["fontPath"].GetString(), doc["ui"]["fontSize"].GetInt()));

	mStartString = ui["startString"].GetString();
	mDeathString = ui["deathString"].GetString();
	mDeathString2 = ui["deathString2"].GetString();
	mFailedString = ui["failedString"].GetString();
	mCompleteString = ui["completeString"].GetString();
	mReplayString = ui["replayString"].GetString();
	mReplayStringLine2 = ui["replayStringLine2"].GetString();

	mpMenuShader = new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer(ui["menuShader"].GetString()));
	mpMenuShader->setTransparency(.55f);

	float boxWidth = Game::getInstance()->getGraphics().getWidth() / 3;
	float height = Game::getInstance()->getGraphics().getHeight() / 3;
	float heightIncr = doc["ui"]["fontSize"].GetFloat() + 20;
	rapidjson::Value &arr = doc["ui"]["ui_boxes"];
	Sprite boxBackground = Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer(doc["ui"]["box_background"].GetString()));
	for(auto &a : arr.GetArray())
	{
		std::vector<UIBox> boxes;
		int y = 0;
		for(auto &b : a["boxes"].GetArray())
		{
			Vector2D centerOffset((boxWidth - mFont->getWidth(b["string"].GetString())) / 2, 5);
			boxes.push_back(UIBox(Vector2D(boxWidth, height + heightIncr * y), boxWidth, heightIncr, centerOffset, b["string"].GetString(), b["function"].GetString(), new Sprite(boxBackground)));
			y++;
		}

		mUIBoxes.insert(std::pair<std::string, std::vector<UIBox>>(a["name"].GetString(), boxes));
	}

	mActiveMenu = "main_menu_ui_boxes";

	gpEventSystem->addListener(MOUSE_CLICK_EVENT, this);
	gpEventSystem->addListener(UPDATE_FPS_EVENT, this);
	gpEventSystem->addListener(PLAYER_HEALTH_EVENT, this);
	gpEventSystem->addListener(INCREMENT_LIVES_EVENT, this);
	gpEventSystem->addListener(INCREMENT_SCORE_EVENT, this);
}

UI::~UI()
{
	if(mpMenuShader)
	{
		delete mpMenuShader;
		mpMenuShader = NULL;
	}

	if(mFont)
	{
		delete mFont;
		mFont = NULL;
	}
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
	else if(theEvent.getType() == MOUSE_CLICK_EVENT)
	{
		const MouseClickEvent &ev = static_cast<const MouseClickEvent&>(theEvent);
		Vector2D pos = ev.getPosition();
		Gamestate &gamestate = Game::getInstance()->getGamestate();
		if(gamestate == MENU || gamestate == START)
		{
			//for all menu boxes in active menu, see if they contain mouse position
			//if so, perform their on click action
			for(auto a : mUIBoxes[mActiveMenu])
			{
				if(a.contains(pos))
				{
					gpEventSystem->fireEvent(new PlaySoundEvent(PLAY_SOUND_EVENT, "menu_click_sound"));
					//decide what to do based on box's function
					std::string function = a.getFunction().substr(0, 4); //all commands are 4 chars long, makes it easier to add more and keep them all inside one place
					if(function == "play")
					{
						Game::getInstance()->unpauseClock();
						gamestate = PLAYING;
					}
					else if(function == "menu")
					{
						std::string command = a.getFunction().substr(5, a.getFunction().size() - 5);
						for(auto it = mUIBoxes.begin(); it != mUIBoxes.end(); it++)
						{
							if(command == it->first)
							{
								mActiveMenu = it->first;
							}
						}
					}
					else if(function == "load")
					{
						//handled by all systems that need to manage save data, e.g. UnitManager, whatever system keeps track of victory points and score, etc
						gpEventSystem->fireEvent(new LoadEvent(LOAD_EVENT));
					}
					else if(function == "quit")
					{
						gpEventSystem->fireEvent(new QuitEvent(QUIT_EVENT));
					}
					else if(function == "taud")
					{
						gpEventSystem->fireEvent(new VolumeEvent(VOLUME_EVENT, -1));
					}
					else if(function == "diff")
					{
						std::string command = a.getFunction().substr(5, a.getFunction().size() - 5);
						std::cout << "Setting difficulty to " << command << ".\n";
						if(command == "easy")
						{
							gpEventSystem->fireEvent(new DifficultyChangeEvent(DIFFICULTY_CHANGE_EVENT, D_EASY));
						}
						else if(command == "normal")
						{
							gpEventSystem->fireEvent(new DifficultyChangeEvent(DIFFICULTY_CHANGE_EVENT, D_NORMAL));
						}
						else if(command == "hard")
						{
							gpEventSystem->fireEvent(new DifficultyChangeEvent(DIFFICULTY_CHANGE_EVENT, D_HARD));
						}
						mActiveMenu = "main_menu_ui_boxes";
					}
				}
			}
		}
	}
}

void UI::draw()
{
	GraphicsSystem &graphics = Game::getInstance()->getGraphics();
	float width = graphics.getWidth();
	float height = graphics.getHeight();
	Vector2D screenTopLeft = graphics.getTopLeft();
	
	Gamestate &gamestate = Game::getInstance()->getGamestate();
	switch(gamestate)
	{
	case START:
		//need to center this
		//graphics.writeText(screenTopLeft + Vector2D(width / 2 - mFont->getWidth(mStartString) / 2, height / 2), *mFont, mUIColor, mStartString);
		graphics.draw(screenTopLeft, *mpMenuShader);
		for(auto &a : mUIBoxes[mActiveMenu])
		{
			a.draw(screenTopLeft, *mFont, mUIColor);
		}
		break;
	case DEATH:
		graphics.writeText(screenTopLeft + Vector2D(width / 2 - mFont->getWidth(mDeathString) / 2, height / 2), *mFont, mUIColor, mDeathString);
		graphics.writeText(screenTopLeft + Vector2D(width / 2 - mFont->getWidth(mDeathString2) / 2, height / 2 + mFont->getFontSize()), *mFont, mUIColor, mDeathString2);
		break;
	case PLAYING:
		graphics.writeText(screenTopLeft + Vector2D(0, 0), *mFont, mUIColor, "FPS: " + std::to_string(mFps));
		graphics.writeText(screenTopLeft + Vector2D(width / 3, 0), *mFont, mUIColor, "Lives: " + std::to_string(mLives));
		graphics.writeText(screenTopLeft + Vector2D((2 * width / 3), 0), *mFont, mUIColor, "Score: " + std::to_string(mScore));
		break;
	case MENU:
		graphics.draw(screenTopLeft, *mpMenuShader);
		for(auto &a : mUIBoxes[mActiveMenu])
		{
			a.draw(screenTopLeft, *mFont, mUIColor);
		}
		break;
	case FAILED:
		graphics.writeText(screenTopLeft + Vector2D(width / 2 - mFont->getWidth(mFailedString) / 2, height / 2), *mFont, mUIColor, mFailedString);
		graphics.writeText(screenTopLeft + Vector2D(width / 2 - mFont->getWidth(mReplayString) / 2, height / 2 + mFont->getFontSize()), *mFont, mUIColor, mReplayString);
		graphics.writeText(screenTopLeft + Vector2D(width / 2 - mFont->getWidth(mReplayStringLine2) / 2, height / 2 + 2 * mFont->getFontSize()), *mFont, mUIColor, mReplayStringLine2);
		break;
	case COMPLETE:
		graphics.writeText(screenTopLeft + Vector2D(width / 2 - mFont->getWidth(mCompleteString) / 2, height / 2), *mFont, mUIColor, mCompleteString);
		graphics.writeText(screenTopLeft + Vector2D(width / 2 - mFont->getWidth(mReplayString) / 2, height / 2 + mFont->getFontSize()), *mFont, mUIColor, mReplayString);
		graphics.writeText(screenTopLeft + Vector2D(width / 2 - mFont->getWidth(mReplayStringLine2) / 2, height / 2 + 2 * mFont->getFontSize()), *mFont, mUIColor, mReplayStringLine2);
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
	case MENU:
		//probably just move this to handle event (mouse click)
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

void UI::setScore(int newScore)
{
	mScore = newScore;
}

int UI::getScore()
{
	return mScore;
}
