#include "scorePowerUp.h"
#include "snake.h"

ScorePowerUp::ScorePowerUp(Vector2D pos, Animation *a) : Unit(pos, a)
{
}

ScorePowerUp::~ScorePowerUp()
{
	cleanup();
}

void ScorePowerUp::collide(Snake *o)
{
	o->collide(this);
}
