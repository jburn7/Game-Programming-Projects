#include "physicsComponent.h"
#include "Entity.h"
#include "game.h"

PhysicsComponent::PhysicsComponent()
{
	float pixelsPerMeter = 15;
	mGravity = 9.81f / pixelsPerMeter;
}

PhysicsComponent::~PhysicsComponent()
{
}

void PhysicsComponent::update(Entity *e)
{
	if(!e->mIsOnGround)
	{
		e->mVelocity.setY(e->mVelocity.getY() + mGravity);
	}
	else
	{
		e->mVelocity.setY(0);
	}

	if(e->mMoveLeft)
		e->mVelocity.setX(-e->mMaxHorizVelocity);
	if(e->mMoveRight)
		e->mVelocity.setX(e->mMaxHorizVelocity);
	if(!e->mMoveLeft && !e->mMoveRight)
		e->mVelocity.setX(0);
	if(e->mJump && e->mIsOnGround)
		e->mVelocity.setY(-e->mJumpVelocity);

	if(e->mVelocity.getY() > e->mMaxVerticalVelocity)
	{
		e->mVelocity.setY(e->mMaxVerticalVelocity);
	}

	e->move(e->mVelocity);
}

void PhysicsComponent::update(Entity * e, std::vector<std::vector<bool>>& wallMap)
{
	InputComponent &in = *e->mInput;
	int height = (int)e->mPosition.getY() + (int)e->getHeight();
	if(!in.queryWall((int)e->mPosition.getX(), height) && !in.queryWall((int)e->mPosition.getX() + (int)e->getWidth() / 2, height) && !in.queryWall((int)e->mPosition.getX() + (int)e->getWidth() - 1, height))
	{
		e->mIsOnGround = false;
	}
	else
	{
		e->mIsOnGround = true;
	}

	if(!e->mIsOnGround)
	{
		e->mVelocity.setY(e->mVelocity.getY() + mGravity);
	}
	else
	{
		e->mVelocity.setY(0);
	}

	if(e->mMoveLeft)
		e->mVelocity.setX(-e->mMaxHorizVelocity);
	if(e->mMoveRight)
		e->mVelocity.setX(e->mMaxHorizVelocity);
	if(!e->mMoveLeft && !e->mMoveRight)
		e->mVelocity.setX(0);
	if(e->mJump && e->mIsOnGround)
		e->mVelocity.setY(-e->mJumpVelocity);

	if(e->mVelocity.getY() > e->mMaxVerticalVelocity)
	{
		e->mVelocity.setY(e->mMaxVerticalVelocity);
	}

	e->move(e->mVelocity);

	//if e collides with wall, SAT and move it
	if(in.queryWall((int)e->mPosition.getX(), (int)e->mPosition.getY()))
	{
		int roundedX = (int)e->mPosition.getX() - (int)e->mPosition.getX() % 40;
		int roundedY = (int)e->mPosition.getY() - (int)e->mPosition.getY() % 40;
		if(Game::getInstance()->getUnitManager().getWallMaskPointers()[roundedY / 40][roundedX / 40])
		{
			e->move(e->mCollision.SATOverlap(e->mHitbox, Game::getInstance()->getUnitManager().getWallMaskPointers()[roundedY / 40][roundedX / 40]->getHitBox()));
		}
		/*int width = in.getWallWidth(roundedX, roundedY);
		std::vector<Vector2D> hitboxPoints;
		hitboxPoints.push_back(Vector2D(0, 0));
		hitboxPoints.push_back(Vector2D(0, 40));
		hitboxPoints.push_back(Vector2D((float)width, 40));
		hitboxPoints.push_back(Vector2D((float)width, 0));
		HitBox h;
		h.create(hitboxPoints, Vector2D((float)roundedX, (float)roundedY));
		Vector2D offset(e->mCollision.SATOverlap(e->mHitbox, h));
		e->move(offset);
		if(offset.getY() == 0)
		{
			e->mVelocity.setX(0);
		}
		if(offset.getX() == 0)
		{
			e->mVelocity.setY(0);
		}*/
	}
	if(in.queryWall((int)e->mPosition.getX() + (int)e->getWidth(), (int)e->mPosition.getY()))
	{
		int posX = (int)e->mPosition.getX() + (int)e->getWidth();
		int roundedX = posX - posX % 40;
		int roundedY = (int)e->mPosition.getY() - (int)e->mPosition.getY() % 40;
		if(Game::getInstance()->getUnitManager().getWallMaskPointers()[roundedY / 40][roundedX / 40])
		{
			e->move(e->mCollision.SATOverlap(e->mHitbox, Game::getInstance()->getUnitManager().getWallMaskPointers()[roundedY / 40][roundedX / 40]->getHitBox()));
		}
		/*int width = in.getWallWidth(roundedX, roundedY);
		std::vector<Vector2D> hitboxPoints;
		hitboxPoints.push_back(Vector2D(0, 0));
		hitboxPoints.push_back(Vector2D(0, 40));
		hitboxPoints.push_back(Vector2D((float)width, 40));
		hitboxPoints.push_back(Vector2D((float)width, 0));
		HitBox h;
		h.create(hitboxPoints, Vector2D((float)roundedX, (float)roundedY));
		Vector2D offset(e->mCollision.SATOverlap(e->mHitbox, h));
		e->move(offset);
		if(offset.getY() == 0)
		{
			e->mVelocity.setX(0);
		}
		if(offset.getX() == 0)
		{
			e->mVelocity.setY(0);
		}*/
	}
	if(in.queryWall((int)e->mPosition.getX(), height - 1))
	{
		int roundedX = (int)e->mPosition.getX() - (int)e->mPosition.getX() % 40;
		int roundedY = height - height % 40;
		if(!Game::getInstance()->getUnitManager().getWallMaskPointers()[roundedY / 40][roundedX / 40])
		{
			roundedY -= 40;
		}
		Vector2D overlap;
		if(Game::getInstance()->getUnitManager().getWallMaskPointers()[roundedY / 40][roundedX / 40])
			overlap = (e->mCollision.SATOverlap(e->mHitbox, Game::getInstance()->getUnitManager().getWallMaskPointers()[roundedY / 40][roundedX / 40]->getHitBox()));
		if(overlap.getX() == 0 && overlap.getY() == 0)
		{
			roundedY -= 40;
		}
		if(Game::getInstance()->getUnitManager().getWallMaskPointers()[roundedY / 40][roundedX / 40])
		{
			overlap = e->mCollision.SATOverlap(e->mHitbox, Game::getInstance()->getUnitManager().getWallMaskPointers()[roundedY / 40][roundedX / 40]->getHitBox());
		}
		e->move(overlap);
		/*int width = in.getWallWidth(roundedX, roundedY);
		std::vector<Vector2D> hitboxPoints;
		hitboxPoints.push_back(Vector2D(0, 0));
		hitboxPoints.push_back(Vector2D(0, 40));
		hitboxPoints.push_back(Vector2D((float)width, 40));
		hitboxPoints.push_back(Vector2D((float)width, 0));
		HitBox h;
		h.create(hitboxPoints, Vector2D((float)roundedX, (float)roundedY));
		Vector2D offset(e->mCollision.SATOverlap(e->mHitbox, h));
		e->move(offset);
		if(offset.getY() == 0)
		{
			e->mVelocity.setX(0);
		}
		if(offset.getX() == 0)
		{
			e->mVelocity.setY(0);
		}*/
	}
	if(in.queryWall((int)e->mPosition.getX() + (int)e->getWidth(), height))
	{
		int posX = (int)e->mPosition.getX() + (int)e->getWidth();
		int roundedX = posX - posX % 40;
		int roundedY = height - height % 40;
		if(!Game::getInstance()->getUnitManager().getWallMaskPointers()[roundedY / 40][roundedX / 40])
		{
			roundedY -= 40;
		}
		Vector2D overlap;
		if(Game::getInstance()->getUnitManager().getWallMaskPointers()[roundedY / 40][roundedX / 40])
			overlap = (e->mCollision.SATOverlap(e->mHitbox, Game::getInstance()->getUnitManager().getWallMaskPointers()[roundedY / 40][roundedX / 40]->getHitBox()));
		if(overlap.getX() == 0 && overlap.getY() == 0)
		{
			roundedY -= 40;
		}
		if(Game::getInstance()->getUnitManager().getWallMaskPointers()[roundedY / 40][roundedX / 40])
		{
			overlap = e->mCollision.SATOverlap(e->mHitbox, Game::getInstance()->getUnitManager().getWallMaskPointers()[roundedY / 40][roundedX / 40]->getHitBox());
			e->move(overlap);
			if(overlap.getX() != 0)
			{
				e->mIsOnGround = false;
			}
		}
		/*int width = in.getWallWidth(roundedX, roundedY);
		if(width == 0)
		{
			width = 40;
			roundedX -= 40;
		}
		std::vector<Vector2D> hitboxPoints;
		hitboxPoints.push_back(Vector2D(0, 0));
		hitboxPoints.push_back(Vector2D(0, 40));
		hitboxPoints.push_back(Vector2D((float)width, 40));
		hitboxPoints.push_back(Vector2D((float)width, 0));
		HitBox h;
		h.create(hitboxPoints, Vector2D((float)roundedX, (float)roundedY));
		Vector2D offset(e->mCollision.SATOverlap(e->mHitbox, h));
		e->move(offset);
		if(offset.getY() == 0)
		{
			e->mVelocity.setX(0);
		}
		if(offset.getX() == 0)
		{
			e->mVelocity.setY(0);
		}*/
	}
	if(in.queryWall((int)e->mPosition.getX(), (int)e->mPosition.getY() + (int)e->getHeight() / 2))
	{
		int roundedX = (int)e->mPosition.getX() - (int)e->mPosition.getX() % 40;
		int roundedY = (int)e->mPosition.getY() + (int)e->getHeight() / 2 - (int)((int)e->mPosition.getY() + (int)e->getHeight() / 2) % 40;
		/*if(!Game::getInstance()->getUnitManager().getWallMaskPointers()[roundedY / 40][roundedX / 40])
		{
			roundedY -= 40;
		}
		Vector2D overlap(e->mCollision.SATOverlap(e->mHitbox, Game::getInstance()->getUnitManager().getWallMaskPointers()[roundedY / 40][roundedX / 40]->getHitBox()));
		if(overlap.getX() == 0 && overlap.getY() == 0)
		{
			roundedY -= 40;
		}*/
		Vector2D overlap;
		if(Game::getInstance()->getUnitManager().getWallMaskPointers()[roundedY / 40][roundedX / 40])
		{
			overlap = e->mCollision.SATOverlap(e->mHitbox, Game::getInstance()->getUnitManager().getWallMaskPointers()[roundedY / 40][roundedX / 40]->getHitBox(), false, false);
		}
		e->move(overlap);
	}
	if(in.queryWall((int)e->mPosition.getX() + (int)e->getWidth(), (int)e->mPosition.getY() + (int)e->getHeight() / 2))
	{
		int roundedX = ((int)e->mPosition.getX() + (int)e->getWidth()) - ((int)e->mPosition.getX() + (int)e->getWidth()) % 40;
		int roundedY = ((int)e->mPosition.getY() + (int)e->getHeight() / 2) - (int)((int)e->mPosition.getY() + (int)e->getHeight() / 2) % 40;
		/*if(!Game::getInstance()->getUnitManager().getWallMaskPointers()[roundedY / 40][roundedX / 40])
		{
		roundedY -= 40;
		}
		Vector2D overlap(e->mCollision.SATOverlap(e->mHitbox, Game::getInstance()->getUnitManager().getWallMaskPointers()[roundedY / 40][roundedX / 40]->getHitBox()));
		if(overlap.getX() == 0 && overlap.getY() == 0)
		{
		roundedY -= 40;
		}*/
		Vector2D overlap;
		if(Game::getInstance()->getUnitManager().getWallMaskPointers()[roundedY / 40][roundedX / 40])
		{
			overlap = e->mCollision.SATOverlap(e->mHitbox, Game::getInstance()->getUnitManager().getWallMaskPointers()[roundedY / 40][roundedX / 40]->getHitBox(), false, false);
		}
		e->move(overlap);
	}
}
