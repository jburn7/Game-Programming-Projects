#pragma once
#include "unit.h"

class ScorePowerUp : public Unit
{
public:
	ScorePowerUp(Vector2D pos, Animation *a);
	~ScorePowerUp();

	virtual void collide(Snake *o);
};