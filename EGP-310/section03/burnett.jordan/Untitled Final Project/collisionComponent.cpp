#pragma once
#include "collisionComponent.h"
#include "Entity.h"
#include "wall.h"
#include "attack.h"
#include "Collectible.h"

float Projection::getMin()
{
	return min;
}

float Projection::getMax()
{
	return max;
}

float Projection::getOverlap(Projection &projection)
{
	if(projection.getMin() <= max && max <= projection.getMax())
	{
		return max - projection.getMin();
	}
	else if(min <= projection.getMax() && projection.getMax() <= max)
	{
		return projection.getMax() - min;
	}
	else
	{
		return 0;
	}
}

bool CollisionComponent::SATBool(HitBox & hitBoxOne, HitBox & hitBoxTwo)
{
	gpPerformanceTracker->startTracking("SAT");
	sf::ConvexShape objectOne = hitBoxOne.mOutline;
	sf::ConvexShape objectTwo = hitBoxTwo.mOutline;

	sf::VertexArray one;
	for(unsigned int i = 0; i < objectOne.getPointCount(); i++)
	{
		one.append(sf::Vertex(sf::Vector2f(objectOne.getPosition() - objectOne.getOrigin() + objectOne.getPoint(i))));
	}

	sf::VertexArray two;
	for(unsigned int i = 0; i < objectTwo.getPointCount(); i++)
	{
		two.append(sf::Vertex(sf::Vector2f(objectTwo.getPosition() - objectTwo.getOrigin() + objectTwo.getPoint(i))));
	}

	std::vector<Vector2D> axesOne;
	std::vector<Vector2D> axesTwo;
	//for both shapes, get their axes
	for(unsigned int i = 0; i < one.getVertexCount(); i++)
	{
		axesOne.push_back(getNormalAxis(one, i));
	}
	for(unsigned int i = 0; i < two.getVertexCount(); i++)
	{
		axesTwo.push_back(getNormalAxis(two, i));
	}

	for(unsigned int i = 0; i < axesOne.size(); i++)
	{
		Projection projectionOne(Vector2D(axesOne[i]), one);
		Projection projectionTwo(Vector2D(axesOne[i]), two);

		float o = projectionOne.getOverlap(projectionTwo);
		if(o != 0.f)
		{
			gpPerformanceTracker->stopTracking("SAT");
			return true;
		}
	}

	for(unsigned int i = 0; i < axesTwo.size(); i++)
	{
		Projection projectionOne(Vector2D(axesTwo[i]), one);
		Projection projectionTwo(Vector2D(axesTwo[i]), two);

		float o = projectionOne.getOverlap(projectionTwo);
		if(o != 0.f)
		{
			gpPerformanceTracker->stopTracking("SAT");
			return true;
		}
	}

	gpPerformanceTracker->stopTracking("SAT");
	return false;
}

Vector2D CollisionComponent::SATOverlap(HitBox &hitBoxOne, HitBox &hitBoxTwo, bool xOnly, bool yOnly)
{
	gpPerformanceTracker->startTracking("SAT");
	sf::ConvexShape objectOne = hitBoxOne.mOutline;
	sf::ConvexShape objectTwo = hitBoxTwo.mOutline;

	sf::VertexArray one;
	for(unsigned int i = 0; i < objectOne.getPointCount(); i++)
	{
		one.append(sf::Vertex(sf::Vector2f(objectOne.getPosition() - objectOne.getOrigin() + objectOne.getPoint(i))));
	}

	sf::VertexArray two;
	for(unsigned int i = 0; i < objectTwo.getPointCount(); i++)
	{
		two.append(sf::Vertex(sf::Vector2f(objectTwo.getPosition() - objectTwo.getOrigin() + objectTwo.getPoint(i))));
	}

	std::vector<Vector2D> axesOne;
	std::vector<Vector2D> axesTwo;
	//for both shapes, get their axes
	for(unsigned int i = 0; i < one.getVertexCount(); i++)
	{
		axesOne.push_back(getNormalAxis(one, i));
	}
	for(unsigned int i = 0; i < two.getVertexCount(); i++)
	{
		axesTwo.push_back(getNormalAxis(two, i));
	}

	//for both sets of axes
	//project
	float overlap = 5000000;
	Vector2D smallestAxis;

	for(unsigned int i = 0; i < axesOne.size(); i++)
	{
		Projection projectionOne(Vector2D(axesOne[i]), one);
		Projection projectionTwo(Vector2D(axesOne[i]), two);

		float o = projectionOne.getOverlap(projectionTwo);
		if(o < overlap /*&& o != 0.f*/)
		{
			overlap = o;
			smallestAxis = axesOne[i];
		}
	}

	for(unsigned int i = 0; i < axesTwo.size(); i++)
	{
		Projection projectionOne(Vector2D(axesTwo[i]), one);
		Projection projectionTwo(Vector2D(axesTwo[i]), two);

		float o = projectionOne.getOverlap(projectionTwo);
		if(o < overlap && o!= 0.f)
		{
			overlap = o;
			smallestAxis = axesTwo[i];
		}
	}

	Vector2D distance, aPos, bPos;
	aPos = sf::Vector2f(one.getBounds().left + one.getBounds().width / 2, one.getBounds().top + one.getBounds().height / 2);
	bPos = sf::Vector2f(two.getBounds().left + two.getBounds().width / 2, two.getBounds().top + two.getBounds().height / 2);
	distance = aPos - bPos;

	if(xOnly)
	{
		smallestAxis.setY(0);
	}
	else if(yOnly)
	{
		smallestAxis.setX(0);
	}

	if(dotProduct(distance, smallestAxis) < 0.0f)
	{
		smallestAxis.setX(smallestAxis.getX() * -1);
		smallestAxis.setY(smallestAxis.getY() * -1);
	}

	gpPerformanceTracker->stopTracking("SAT");
	return smallestAxis * overlap;
}

bool CollisionComponent::AABB(Unit *objectOne, Unit *objectTwo)
{
	gpPerformanceTracker->startTracking("total AABB");
	gpPerformanceTracker->startTracking("My AABB");

	bool ret = (objectOne->getPosition().getX() < objectTwo->getPosition().getX() + objectTwo->getWidth() && objectOne->getPosition().getX() + objectOne->getWidth() > objectTwo->getPosition().getX() && objectOne->getPosition().getY() < objectTwo->getPosition().getY() + objectTwo->getHeight() && objectOne->getPosition().getY() + objectOne->getHeight() > objectTwo->getPosition().getY());
	gpPerformanceTracker->stopTracking("My AABB");
	gpPerformanceTracker->stopTracking("total AABB");
	return ret;
}

bool CollisionComponent::AABB(HitBox &hitboxOne, HitBox &hitboxTwo)
{
	gpPerformanceTracker->startTracking("SF AABB");
	gpPerformanceTracker->startTracking("total AABB");
	bool ret = hitboxOne.mOutline.getGlobalBounds().intersects(hitboxTwo.mOutline.getGlobalBounds());
	gpPerformanceTracker->stopTracking("SF AABB");
	gpPerformanceTracker->stopTracking("total AABB");
	return ret;
}

void CollisionComponent::resolveCollision(Entity *e, Wall *w)
{
	if(AABB(e, w))
	{
		Vector2D offset;
		
		//find direction in which e is coming from
		//move e towards that direction
		//if(e->mPreviousPosition != e->mPosition)
		//{
		//	//Vector2D movement = e->mPosition - e->mPreviousPosition;
		//	Vector2D movement = e->mPosition - w->mPosition;
		//	if(abs(movement.getX() > abs(movement.getY())))
		//	{
		//		offset = SATOverlap(e->mHitbox, w->mHitbox, true, false);
		//	}
		//	else
		//	{
		//		offset = SATOverlap(e->mHitbox, w->mHitbox, false, true);
		//	}
		//}
		//else
		{
			offset = SATOverlap(e->mHitbox, w->mHitbox);
		}
		//if(offset.getX() < e->mVelocity.getX() && offset.getY() < e->mVelocity.getY())
		{
			//e->move(offset);
		}
		if(offset.getY() == 0)
		{
			e->mVelocity.setX(0);
		}
		if(offset.getX() == 0)
		{
			e->mVelocity.setY(0);
		}
	}

	/*if(AABB(e->mFeetBox, w->getHitBox()))
	{
		e->mIsOnGround = true;
	}*/

	//do nothing to Wall
}

void CollisionComponent::resolveCollision(Entity *e, Attack *a)
{
	if(!a->mpOwner->mIsPlayer && !e->mIsPlayer)
	{
		return;
	}
	if(a->mpOwner == e || a->mInactive)
	{
		return;
	}

	for(unsigned int i = 0; i < a->mEntitiesHit.size(); i++)
	{
		if(a->mEntitiesHit[i] == e)
		{
			return;
		}
	}

	if(AABB(e->getHitBox(), a->getHitBox()) && SATBool(e->mHitbox, a->mHitbox))
	{
		e->takeDamage(a->mAttackDamage);
		a->mEntitiesHit.push_back(e);
		a->setInactive();
	}
}

void CollisionComponent::resolveCollision(Entity *e, Collectible *c)
{
	WeaponPickup *test = dynamic_cast<WeaponPickup*>(c);
	if(test)
	{
		//(test->mWidth != 0)
		{
			if(e->mIsPlayer && AABB(e, c))
			{
				if(!c->getInputRequired() || e->mItemPickup)
				{
					e->applyCollectible(c);
					gpEventSystem->fireEvent(new UnitRemoveEvent(UNIT_REMOVE_EVENT, c));
				}
			}
		}
		return;
	}

	if(e->mIsPlayer && AABB(e, c))
	{
		if(!c->getInputRequired() || e->mItemPickup)
		{
			e->applyCollectible(c);
			gpEventSystem->fireEvent(new UnitRemoveEvent(UNIT_REMOVE_EVENT, c));
		}
	}
}

void CollisionComponent::resolveCollision(Entity *e1, Entity *e2)
{
	if(e1 != e2)
	{
		if(AABB(e1, e2))
		{
			if(e1->mIsPlayer)
			{
				e1->takeDamage(e2->mDamageVal);
			}
			else if(e2->mIsPlayer)
			{
				e2->takeDamage(e1->mDamageVal);
			}
		}
	}
}

void CollisionComponent::resolveCollision(Wall *w, Attack *a)
{
	if(AABB(w, a))
	{
		gpEventSystem->fireEvent(new PlaySoundEvent(PLAY_SOUND_EVENT, "attack_collide_sound"));
		gpEventSystem->fireEvent(new UnitRemoveEvent(UNIT_REMOVE_EVENT, a));
	}
}

float CollisionComponent::dotProduct(Vector2D &vectorOne, Vector2D &vectorTwo)
{
	return vectorOne.getX() * vectorTwo.getX() + vectorOne.getY() * vectorTwo.getY();
}

Vector2D CollisionComponent::getNormalAxis(sf::VertexArray &shape, int index)
{
	sf::Vector2f vectorOne = shape[index].position;
	sf::Vector2f vectorTwo;
	if(index >= (int)shape.getVertexCount() - 1)
		vectorTwo = shape[0].position;
	else
		vectorTwo = shape[index + 1].position;

	Vector2D normalAxis(-(vectorTwo.y - vectorOne.y), vectorTwo.x - vectorOne.x);
	normalAxis = normalize(normalAxis);
	return normalAxis;
}

Vector2D &CollisionComponent::normalize(Vector2D &input)
{
	if(sqrt(input.getX() * input.getX() + input.getY() * input.getY()) == 0)
	{
		input.setX(1);
		return input;
	}
	float length = sqrt(input.getX() * input.getX() + input.getY() * input.getY());

	input.setX(input.getX() / length);
	input.setY(input.getY() / length);
	return input;
}

Projection::Projection(Vector2D &axis, sf::VertexArray &vertices)
{
	min = CollisionComponent::dotProduct(axis, Vector2D(vertices[0].position));
	max = min;

	for(unsigned int i = 1; i < vertices.getVertexCount(); i++)
	{
		float proj = CollisionComponent::dotProduct(axis, Vector2D(vertices[i].position));
		if(proj < min)
		{
			min = proj;
		}
		else if(proj > max)
		{
			max = proj;
		}
	}
}