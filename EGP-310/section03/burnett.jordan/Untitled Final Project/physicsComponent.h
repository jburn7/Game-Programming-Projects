#pragma once
#include "Vector2D.h"
#include "jsonData.h"
#include <vector>

class Entity;

class PhysicsComponent
{
public:
	PhysicsComponent();
	~PhysicsComponent();

	void update(Entity *e);

	void update(Entity *e, std::vector<std::vector<bool>> &wallMap);
private:
	float mGravity;
};