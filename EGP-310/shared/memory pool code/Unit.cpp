#include "Unit.h"
#include <cstdlib>

Unit::Unit() :mX(0), mY(0), mVelX(0), mVelY(0), mRotation(0.0f), mpSprite(NULL)
{
}

Unit::Unit(Uint32 x, Uint32 y) : Unit()
{
	mX = x;
	mY = y;
}


Unit::~Unit()
{
}

void Unit::init(Uint32 x, Uint32 y, Uint32 velX, Uint32 velY, float rotation, Sprite* pSprite)
{
	mX = x;
	mY = y;
	mVelX = velX;
	mVelY = velY;
	mRotation = rotation;
	mpSprite = pSprite;
}
