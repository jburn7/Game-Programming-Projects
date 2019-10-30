#include "wall.h"
#include "snake.h"

Wall::Wall(Vector2D pos, Sprite *s) : Unit(pos, s)
{
}

Wall::~Wall()
{
}

void Wall::collide(Snake *o)
{
	o->collide(this);
}
