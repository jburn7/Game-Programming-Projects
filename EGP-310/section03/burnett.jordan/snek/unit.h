#pragma once
#include "animation.h"

class Snake;
class Wall;
class SnakeIncrementPowerUp;
class ScorePowerUp;
class SpeedPowerUp;

class Unit : public Trackable
{
	friend class UnitManager;
public:
	Unit();

	Unit(Vector2D pos, Animation *a);

	Unit(Vector2D pos, Sprite *s);

	virtual ~Unit();

	void cleanup();

	virtual void draw();

	virtual void update(double timeElapsed);

	//collision functions
	bool contains(Vector2D &loc);

	//setters
	void setAnimating(bool torf);

	void setPosition(Vector2D pos);

	void setSprite(Sprite *s);

	//getters
	Vector2D &getPosition();
	int getWidth();
	int getHeight();

	//collision functions, using double dispatch because I despise static casting
	virtual void collide(Unit *o);
	virtual void collide(ScorePowerUp *o) {}
	virtual void collide(Snake *o) {}
	virtual void collide(SpeedPowerUp *o) {}
	virtual void collide(SnakeIncrementPowerUp *o) {}
	virtual void collide(Wall *o) {}

	bool squareCollision(Unit *other);
protected:
	virtual bool tryRemove(){ return true; } //returns true if the object is removable such as a powerup, false if it's something like a wall

	Vector2D mPosition;
	bool mAnimating;
	Animation *mAnimation;
	Sprite *mConstantFrame; //either sprite* or animation* will == NULL, depending on whether the Unit has an animation sequence or not
};