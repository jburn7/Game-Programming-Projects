#include "meleeEntityInputComponent.h"

MeleeEntityInputComponent::MeleeEntityInputComponent(Vector2D *playerPos)
{
	mMovingLeft = true;
	mWallLeft = mWallRight = mStanding = mPlayerInRadius = false;
	mPlayerPos = playerPos;
	mInputType = IN_MELEE_ENEMY;
}

MeleeEntityInputComponent::~MeleeEntityInputComponent()
{
}

//void MeleeEntityInputComponent::save(rapidjson::Value &obj, rapidjson::Value &val, rapidjson::Document::AllocatorType &alloc)
//{
//}

void MeleeEntityInputComponent::setDetectionRadius(float radius)
{
	mDetectionRadius = radius;
}

void MeleeEntityInputComponent::update(Entity *e)
{
	e->mAttack = false;
	mWallLeft = mWallRight = mStanding = false;
	//Will check below it in the direction of movement to make sure there is a block there, will reverse direction if there isn't
	if(!queryWall((int)e->mPosition.getX() - 1, (int)e->mPosition.getY() + (int)e->mHeight + 1) ||
	   !queryWall((int)e->mPosition.getX() + (int)e->getWidth() + 1, (int)e->mPosition.getY() + (int)e->mHeight + 1))
	{
        //reverse direction
		//mMovingLeft = !mMovingLeft;
		if(!mMovingLeft)
		{
			mWallRight = true;
		}
		else
		{
			mWallLeft = true;
		}
	}
	/*for (int i = 0; i < (*mWallMask).size(); i++) {
		for (int j = 0; j < (*mWallMask)[i].size(); j++) {
			std::cout << (*mWallMask)[i][j];
		}
		std::cout << std::endl;
	}*/

	//check to see if player is near. if so, set direction to playerPos and start swinging weapon like a madman
	if(abs(mPlayerPos->getX() - e->mPosition.getX()) < mDetectionRadius)
	{
		mPlayerInRadius = true;
		if(abs(mPlayerPos->getY() - e->mPosition.getY()) < e->mHeight)
		{
			e->mAttack = true;
		}
		if(abs(mPlayerPos->getX() - e->mPosition.getX()) < 4)
		{
			mStanding = true;
		}
		else if(mPlayerPos->getX() < e->mPosition.getX())
		{
			mMovingLeft = true;
		}
		else if(mPlayerPos->getX() > e->mPosition.getX())
		{
			mMovingLeft = false;
		}
	}
	else
	{
		if(mPlayerInRadius)
		{
			mPlayerInRadius = false;
			mMovingLeft = !mMovingLeft;
		}
	}

	if(mMovingLeft && mWallLeft)
	{
		mStanding = true;
	}
	if(!mMovingLeft && mWallRight)
	{
		mStanding = true;
	}

	if(!mStanding)
	{
		e->mMoveLeft = mMovingLeft;
		e->mMoveRight = !mMovingLeft;
	}
	else
	{
		e->mMoveLeft = e->mMoveRight = false;
	}
}
