#include "inputComponent.h"
#include "game.h"

int InputComponent::getWallWidth(int &x, int &y)
{
	int yPos = y / 40;
	int xPos = x / 40;
	if((unsigned)yPos >= mWallMask->size() || yPos < 0 || xPos < 0)
	{
		return 0;
	}
	if((unsigned)xPos >= (*mWallMask)[yPos].size())
	{
		return 0;
	}

	int i = x / 40;
	int counterLeft = 0, counterRight = 0;
	while(i >= 0)
	{
		if(!(*mWallMask)[yPos][i])
		{
			break;
		}
		i--;
		counterLeft++;
	}

	i = (x + 40) / 40;
	while((unsigned)i < (*mWallMask)[yPos].size())
	{
		if(!(*mWallMask)[yPos][i])
		{
			break;
		}
		i++;
		counterRight++;
	}

	if(counterLeft > 0)
	{
		x -= counterLeft * 40 - 40;
	}
	else
	{
		y -= 40;
	}

	return (counterLeft + counterRight) * 40;
}

bool InputComponent::queryWall(int x, int y)
{
	/*for (int i = 0; i < mUnits->size(); i++) {
		Vector2D position = (*mUnits)[i]->getPosition();
		if (position.getX() > x) {
			continue;
		}
		else {
			if (position.getY() > y) {
				continue;
			}
			else {
				if (position.getX() + (*mUnits)[i]->getWidth() < x) {
					continue;
				}
				else {
					if (position.getY() + (*mUnits)[i]->getHeight() < y) {
						continue;
					}
					else {
						return true;
					}
				}
			}
		}
	}
	return false;*/
	//Round down to multiple of 40, the width/height of the walls
	if (x < 0 || y < 0 || (int)mWallMask->size() <= y / 40 || (int)(*mWallMask)[y / 40].size() <= x / 40)
		return false;
	else
		return (*mWallMask)[y / 40][x / 40];
}

InputComponent::InputComponent()
{
	//mUnits = &Game::getInstance()->getUnitManager().getUnits();
	mWallMask = Game::getInstance()->getUnitManager().getWallMask();
}

InputComponent::~InputComponent()
{
}

void InputComponent::load(rapidjson::Value &saveData)
{
}

void InputComponent::save(rapidjson::Value &obj, rapidjson::Value &val, rapidjson::Document::AllocatorType &alloc)
{
	obj.AddMember("input_type", mInputType, alloc);
}
