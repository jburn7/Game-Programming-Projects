#include "attack.h"
#include "game.h"
#include "weapon.h"

Attack::Attack(float damage, WeaponBehavior *b, Vector2D &startPos, Entity *e, Weapon *w, Animation* a, float degrees) : Unit(startPos, a)
{
	mType = U_ATTACK;
	mVelocity = Vector2D(0, 0);
	mAttackDamage = damage;
	mpBehavior = b;
	mActualStartPos = startPos;
	mpOwner = e;
	mpWeapon = w;
	mDegrees = degrees;
	mFacingLeft = e->isFacingLeft();

	mInactive = false;
}

Attack::~Attack()
{
	mpWeapon->removeAttack(this);

	if(mpBehavior)
	{
		delete mpBehavior;
		mpBehavior = NULL;
	}
}

void Attack::draw()
{
	//DEBUG used to see hitbox locations //////
	//mHitbox.draw();
	///////////////////////////////////////////

	if(!mFacingLeft)
	{
		Game::getInstance()->getGraphics().draw(mPosition, *mAnimationMap[mCurrentAnimation]->getCurrentSprite(), mDegrees);
	}
	else
	{
		Game::getInstance()->getGraphics().drawReflect(mPosition, *mAnimationMap[mCurrentAnimation]->getCurrentSprite(), Vector2D(-1, 1), mDegrees);
	}
}

void Attack::update(int timeElapsed)
{
	bool remove = false;
	mpBehavior->update(timeElapsed, mpOwner, remove, this, mActualStartPos);

	if(remove)
	{
		gpEventSystem->fireEvent(new UnitRemoveEvent(UNIT_REMOVE_EVENT, this));
	}
}

void Attack::update(int timeElapsed, std::vector<std::vector<bool>>& wallMask)
{
	update(timeElapsed);
	//if e collides with wall, SAT and move it
	UnitManager &um = Game::getInstance()->getUnitManager();
	int height = (int)mPosition.getY() + (int)getHeight();
	if(um.queryWall((int)mPosition.getX(), (int)mPosition.getY()) || um.queryWall((int)mPosition.getX() + (int)getWidth(), (int)mPosition.getY()) || um.queryWall((int)mPosition.getX(), height) || um.queryWall((int)mPosition.getX(), height))
	{
		gpEventSystem->fireEvent(new UnitRemoveEvent(UNIT_REMOVE_EVENT, this));
		/*int roundedX = (int)e->mPosition.getX() - (int)e->mPosition.getX() % 40;
		int roundedY = (int)e->mPosition.getY() - (int)e->mPosition.getY() % 40;
		int width = in.getWallWidth(roundedX, roundedY);
		std::vector<Vector2D> hitboxPoints;
		hitboxPoints.push_back(Vector2D(0, 0));
		hitboxPoints.push_back(Vector2D(0, 40));
		hitboxPoints.push_back(Vector2D(width, 40));
		hitboxPoints.push_back(Vector2D(width, 0));
		HitBox h;
		h.create(hitboxPoints, Vector2D(roundedX, roundedY));
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
	/*if(in.queryWall(e->mPosition.getX() + e->getWidth(), e->mPosition.getY()))
	{
		int posX = e->mPosition.getX() + e->getWidth();
		int roundedX = posX - posX % 40;
		int roundedY = (int)e->mPosition.getY() - (int)e->mPosition.getY() % 40;
		int width = in.getWallWidth(roundedX, roundedY);
		std::vector<Vector2D> hitboxPoints;
		hitboxPoints.push_back(Vector2D(0, 0));
		hitboxPoints.push_back(Vector2D(0, 40));
		hitboxPoints.push_back(Vector2D(width, 40));
		hitboxPoints.push_back(Vector2D(width, 0));
		HitBox h;
		h.create(hitboxPoints, Vector2D(roundedX, roundedY));
		Vector2D offset(e->mCollision.SATOverlap(e->mHitbox, h));
		e->move(offset);
		if(offset.getY() == 0)
		{
			e->mVelocity.setX(0);
		}
		if(offset.getX() == 0)
		{
			e->mVelocity.setY(0);
		}
	}
	if(in.queryWall(e->mPosition.getX(), height - 1))
	{
		int roundedX = (int)e->mPosition.getX() - (int)e->mPosition.getX() % 40;
		int roundedY = height - height % 40;
		int width = in.getWallWidth(roundedX, roundedY);
		std::vector<Vector2D> hitboxPoints;
		hitboxPoints.push_back(Vector2D(0, 0));
		hitboxPoints.push_back(Vector2D(0, 40));
		hitboxPoints.push_back(Vector2D(width, 40));
		hitboxPoints.push_back(Vector2D(width, 0));
		HitBox h;
		h.create(hitboxPoints, Vector2D(roundedX, roundedY));
		Vector2D offset(e->mCollision.SATOverlap(e->mHitbox, h));
		e->move(offset);
		if(offset.getY() == 0)
		{
			e->mVelocity.setX(0);
		}
		if(offset.getX() == 0)
		{
			e->mVelocity.setY(0);
		}
	}
	if(in.queryWall(e->mPosition.getX() + e->getWidth(), height - 1))
	{
		int posX = e->mPosition.getX() + e->getWidth();
		int roundedX = posX - posX % 40;
		int roundedY = height - height % 40;
		int width = in.getWallWidth(roundedX, roundedY);
		std::vector<Vector2D> hitboxPoints;
		hitboxPoints.push_back(Vector2D(0, 0));
		hitboxPoints.push_back(Vector2D(0, 40));
		hitboxPoints.push_back(Vector2D(width, 40));
		hitboxPoints.push_back(Vector2D(width, 0));
		HitBox h;
		h.create(hitboxPoints, Vector2D(roundedX, roundedY));
		Vector2D offset(e->mCollision.SATOverlap(e->mHitbox, h));
		e->move(offset);
		if(offset.getY() == 0)
		{
			e->mVelocity.setX(0);
		}
		if(offset.getX() == 0)
		{
			e->mVelocity.setY(0);
		}
	}*/
}

void Attack::setInactive()
{
	mpBehavior->setInactive(this);
}

void Attack::setFacingLeft(bool left)
{
	mFacingLeft = left;
}

void Attack::setPosition(const Vector2D &pos)
{
	mPosition = pos;
	mHitbox.move(mpOwner->getVelocity());
	//mHitbox.setPosition(pos);
	//if(mFacingLeft)
	//{
	//	//mHitbox.move(Vector2D(-getWidth(), 0));
	//}
}

void Attack::setVelocity(const Vector2D &vel)
{
	mpBehavior->setVelocity(vel);
}

void Attack::collide(Unit *o)
{
	o->collide(this);
}

void Attack::collide(Entity *o)
{
	mCollision.resolveCollision(o, this);
}