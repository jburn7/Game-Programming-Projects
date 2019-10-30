#pragma once
#include "SFML\Graphics\ConvexShape.hpp"
#include "unit.h"
#include "PerformanceTracker.h"

class Projection
{
public:
	Projection(Vector2D &axis, sf::VertexArray &vertices);
	~Projection() {}

	float getMin();
	float getMax();

	float getOverlap(Projection &projection);
private:
	float min, max;
};

class CollisionComponent
{
	friend class Projection;
public:
	bool SATBool(HitBox &hitBoxOne, HitBox &hitBoxTwo);
	Vector2D SATOverlap(HitBox &hitBoxOne, HitBox &hitBoxTwo, bool xOnly = false, bool yOnly = false);

	bool AABB(Unit *objectOne, Unit *objectTwo);
	//assume hitboxes are axis aligned bounding boxes
	bool AABB(HitBox &hitboxOne, HitBox &hitboxTwo);

	void resolveCollision(Entity *e, Wall *w);
	void resolveCollision(Entity *e, Attack *a);
	void resolveCollision(Entity *e, Collectible *c);
	void resolveCollision(Entity *e1, Entity *e2);
	void resolveCollision(Wall *w, Attack *a);
private:
	static float dotProduct(Vector2D &vectorOne, Vector2D &vectorTwo);

	static Vector2D getNormalAxis(sf::VertexArray &shape, int index);

	static Vector2D &normalize(Vector2D &input);
};