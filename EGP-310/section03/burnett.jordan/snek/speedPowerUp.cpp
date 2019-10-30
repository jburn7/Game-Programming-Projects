#include "speedPowerUp.h"
#include "snake.h"

SpeedPowerUp::SpeedPowerUp(Vector2D pos, Animation *a, int dir) : Unit(pos, a)
{
	mDirection = dir;
}

SpeedPowerUp::~SpeedPowerUp()
{
	cleanup();
}

void SpeedPowerUp::collide(Snake *o)
{
	o->collide(this);
}
