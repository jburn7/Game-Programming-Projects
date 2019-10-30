#include "unit.h"
#include "game.h"

CollisionComponent Unit::mCollision = CollisionComponent();

Unit::Unit()
{
	mType = UNDEF;
	mPosition.setX(0);
	mPosition.setY(0);
	mAnimationMap["default"] = NULL;
	mCurrentAnimation = "default";
	mAlternateColor = NULL;
	//mAnimating = true;
	mRemove = false;
}

Unit::Unit(Vector2D pos)
{
	mType = UNDEF;
	mPosition = pos;
	mAlternateColor = NULL;
	//mAnimating = true;
	mRemove = false;
}

Unit::Unit(Vector2D pos, Animation *a)
{
	mType = UNDEF;
	mPosition = pos;
	mAnimationMap["default"] = a;
	mCurrentAnimation = "default";
	mAlternateColor = NULL;
	//mAnimating = true;
	mFacingLeft = true;
	mRemove = false;
	loadHitbox();
}

Unit::Unit(Vector2D pos, Sprite *s)
{
	mType = UNDEF;
	mPosition = pos;
	if(mAnimationMap["default"])
	{
		delete mAnimationMap["default"];
	}
	mAnimationMap["default"] = new Animation(std::vector<Sprite*>(1, s));
	mCurrentAnimation = "default";
	mAlternateColor = NULL;
	//mAnimating = false;
	mFacingLeft = true;
	mRemove = false;
	loadHitbox();
}

Unit::Unit(rapidjson::Value &saveData)
{
	if(saveData.HasMember("color"))
	{
		if(mAlternateColor)
		{
			delete mAlternateColor;
		}
		int tempColor = saveData["color"]["color_r"].GetInt();
		mAlternateColor = new Color(saveData["color"]["color_r"].GetInt(), saveData["color"]["color_g"].GetInt(), saveData["color"]["color_b"].GetInt());
	}
	mCurrentAnimation = saveData["current_animation"].GetString();
	mFacingLeft = saveData["facing_left"].GetBool();
	mPosition = Vector2D(saveData["positionX"].GetFloat(), saveData["positionY"].GetFloat());
	mType = UnitType(saveData["type"].GetInt());
}

Unit::~Unit()
{
	cleanup();
}

void Unit::cleanup()
{
	for (auto x : mAnimationMap)
		delete x.second;
	mAnimationMap.clear();

	if(mAlternateColor)
	{
		delete mAlternateColor;
		mAlternateColor = NULL;
	}
}

void Unit::draw()
{
	//DEBUG
	//mHitbox.draw();
	if(mAnimationMap[mCurrentAnimation])
	{
		if(!mFacingLeft)
		{

			Game::getInstance()->getGraphics().draw(mPosition, *mAnimationMap[mCurrentAnimation]->getCurrentSprite());
		}
		else
		{

			Game::getInstance()->getGraphics().drawReverse(mPosition, *mAnimationMap[mCurrentAnimation]->getCurrentSprite(), Vector2D(-1, 1));
		}
	}
}

void Unit::save(rapidjson::Value &obj, rapidjson::Value &val, rapidjson::Document::AllocatorType &alloc)
{
	if(mAlternateColor)
	{
		rapidjson::Value temp(rapidjson::kObjectType);
		int tempColor = mAlternateColor->getRed();
		temp.AddMember("color_r", mAlternateColor->getRed(), alloc);
		temp.AddMember("color_g", mAlternateColor->getGreen(), alloc);
		temp.AddMember("color_b", mAlternateColor->getBlue(), alloc);
		obj.AddMember("color", temp, alloc);
	}
	val.SetString(mCurrentAnimation.c_str(), alloc);
	obj.AddMember("current_animation", val, alloc);
	val.SetBool(mFacingLeft);
	obj.AddMember("facing_left", val, alloc);
	obj.AddMember("positionX", mPosition.getX(), alloc);
	obj.AddMember("positionY", mPosition.getY(), alloc);
	obj.AddMember("type", mType, alloc); //in loadFromSave(), the caller will check the type of the object and create an appropriate Unit, which will then have its own loadFromSave function
}

void Unit::update(int timeElapsed)
{
	mAnimationMap[mCurrentAnimation]->update(timeElapsed);
}

void Unit::update(int timeElapsed, std::vector<std::vector<bool>>& wallMap)
{
	if(mAnimationMap[mCurrentAnimation])
		mAnimationMap[mCurrentAnimation]->update(timeElapsed);
}

bool Unit::contains(Vector2D &loc)
{
	return mPosition.getX() < loc.getX() && mPosition.getX() + getWidth() > loc.getX() && mPosition.getY() < loc.getY() && mPosition.getY() + getHeight() > loc.getY();
}

void Unit::setAnimating(bool torf)
{
	//mAnimating = torf;
}

void Unit::setColor(const Color &color)
{
	if(mAlternateColor != &color)
	{
		if(mAlternateColor)
		{
			delete mAlternateColor;
		}
		mAlternateColor = new Color(color);
	}

	for(auto a : mAnimationMap)
	{
		a.second->setColor(mAlternateColor);
	}
}

void Unit::setPosition(const Vector2D &pos)
{
	mPosition = pos;
	mHitbox.setPosition(pos);
}

void Unit::setAnimation(std::string anim)
{
	if (mAnimationMap[anim])
		mCurrentAnimation = anim;
}

Vector2D &Unit::getPosition()
{
	return mPosition;
}

float Unit::getWidth()
{
	if (mAnimationMap[mCurrentAnimation])
		return mAnimationMap[mCurrentAnimation]->getCurrentSprite()->getWidth();
	return 0;
}

float Unit::getHeight()
{
	if (mAnimationMap[mCurrentAnimation])
		return mAnimationMap[mCurrentAnimation]->getCurrentSprite()->getHeight();
	return 0;
}

Vector2D Unit::getCenter()
{
	//assuming mPosition is top left corner
 	return Vector2D(mPosition.getX() + getWidth() / 2, mPosition.getY() + getHeight() / 2);
}

void Unit::loadHitbox()
{
	std::vector<Vector2D> hitboxCoords;
	hitboxCoords.push_back(Vector2D(0, 0));
	hitboxCoords.push_back(Vector2D(0, getHeight()));
	hitboxCoords.push_back(Vector2D(getWidth(), getHeight()));
	hitboxCoords.push_back(Vector2D(getWidth(), 0));

	mHitbox.create(hitboxCoords, mPosition);
}

void Unit::modifyDifficulty(float multiplier)
{
}
