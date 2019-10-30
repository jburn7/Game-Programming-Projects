#pragma once
#include "unit.h"

class Wall : public Unit
{
public:
	Wall(Vector2D pos, Sprite *s);
	~Wall();

	virtual void collide(Snake *o);
private:
	virtual bool tryRemove() { return false; } //returns true if the object is removable such as a powerup, false if it's something like a wall
};