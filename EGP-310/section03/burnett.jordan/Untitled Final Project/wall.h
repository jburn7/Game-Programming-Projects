#pragma once
#include "unit.h"

class Wall : public Unit
{
	friend class CollisionComponent;
public:
	Wall(Vector2D pos, Sprite *s, float width);
	~Wall();

	virtual void draw();

	float getWidth() { return mWidth; }

	virtual void collide(Unit *o) { o->collide(this); }
	virtual void collide(Wall *o) {}
	virtual void collide(Entity *o);
private:
	virtual bool tryRemove() { return false; } //returns true if the object is removable such as a powerup, false if it's something like a wall

	float mWidth;
	float mScale;
};