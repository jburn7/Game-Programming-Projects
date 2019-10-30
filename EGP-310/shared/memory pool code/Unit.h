#pragma once

#include "defines.h"

class Sprite;

class Unit
{
public:
	friend class UnitManager;
private:
	Unit();
	Unit(Uint32 x, Uint32 y);
	~Unit();

	void init(Uint32 x, Uint32 y, Uint32 velX, Uint32 velY, float rotation, Sprite* pSprite);

	Uint32 mX;
	Uint32 mY;
	Uint32 mVelX;
	Uint32 mVelY;
	float mRotation;
	Sprite* mpSprite;
};

