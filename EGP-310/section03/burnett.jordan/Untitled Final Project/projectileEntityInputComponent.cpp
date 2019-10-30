#include "projectileEntityInputComponent.h"

ProjectileEntityInputComponent::ProjectileEntityInputComponent(Vector2D *playerPos)
{
	mPlayerPos = playerPos;

	mInputType = IN_BOW_ENEMY;
}

ProjectileEntityInputComponent::~ProjectileEntityInputComponent()
{
}

void ProjectileEntityInputComponent::update(Entity *e)
{
	e->mAttack = false;
	if(abs(mPlayerPos->getX() - e->mPosition.getX()) < mDetectionRadius && abs(mPlayerPos->getY() - e->mPosition.getY()) - e->mHeight)
	{
		e->mAttack = true;
		mTargetAttackLoc = *mPlayerPos;
	}

	if(mPlayerPos->getX() < e->getCenter().getX())
	{
		mMovingLeft = true;
		e->mFacingLeft = true;
	}
	else
	{
		mMovingLeft = false;
		e->mFacingLeft = false;
	}

	e->mMoveLeft = mMovingLeft;
	e->mMoveRight = !mMovingLeft;
}

void ProjectileEntityInputComponent::setDetectionRadius(float radius)
{
	mDetectionRadius = radius;
}
