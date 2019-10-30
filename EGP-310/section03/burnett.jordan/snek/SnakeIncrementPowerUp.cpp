#include "SnakeIncrementPowerUp.h"
#include "snake.h"

SnakeIncrementPowerUp::SnakeIncrementPowerUp(Vector2D pos, Animation *a) : Unit(pos, a)
{
}

SnakeIncrementPowerUp::SnakeIncrementPowerUp(Vector2D pos, Sprite *s) : Unit(pos, s)
{
}

SnakeIncrementPowerUp::~SnakeIncrementPowerUp()
{
	cleanup();
}

void SnakeIncrementPowerUp::collide(Snake *o)
{
	o->collide(this);
}
