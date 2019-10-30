#pragma once
#include "unit.h"

//adds a snake body part when snake head collides
class SnakeIncrementPowerUp : public Unit
{
public:
	SnakeIncrementPowerUp(Vector2D pos, Animation *a);
	SnakeIncrementPowerUp(Vector2D pos, Sprite *s);
	~SnakeIncrementPowerUp();

	virtual void collide(Snake *o);
};