#pragma once
#include "unit.h"

//the powerups for speeding up and slowing down will look identical, muah ha ha hah
class SpeedPowerUp : public Unit
{
public:
	SpeedPowerUp(Vector2D pos, Animation *a, int dir);
	~SpeedPowerUp();

	virtual void collide(Snake *o);

	int getDirection() const { return mDirection; }
private:
	int mDirection;
};