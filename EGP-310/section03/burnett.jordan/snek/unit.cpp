#include "unit.h"
#include "game.h"

Unit::Unit()
{
	mPosition.setX(0);
	mPosition.setY(0);
	mAnimation = NULL;
	mConstantFrame = NULL;
	mAnimating = true;
}

Unit::Unit(Vector2D pos, Animation *a)
{
	mPosition = pos;
	mAnimation = a;
	mAnimating = true;
}

Unit::Unit(Vector2D pos, Sprite *s)
{
	mPosition = pos;
	mConstantFrame = s;
	mAnimating = false;
}

Unit::~Unit()
{
	cleanup();
}

void Unit::cleanup()
{
	if(mAnimation)
	{
		delete mAnimation;
		mAnimation = NULL;
	}
	if(mConstantFrame)
	{
		delete mConstantFrame;
		mConstantFrame = NULL;
	}
}

void Unit::draw()
{
	if(mAnimation)
	{
		Game::getInstance()->getGraphics().draw(mPosition , *mAnimation->getCurrentSprite());
	}
	else
	{
		Game::getInstance()->getGraphics().draw(mPosition , *mConstantFrame);
	}
}

void Unit::update(double timeElapsed)
{
	if(mAnimating && mAnimation)
	{
		mAnimation->update(timeElapsed);
	}
}

bool Unit::contains(Vector2D &loc)
{
	return mPosition.getX() < loc.getX() && mPosition.getX() + getWidth() > loc.getX() && mPosition.getY() < loc.getY() && mPosition.getY() + getHeight() > loc.getY();
}

void Unit::setAnimating(bool torf)
{
	mAnimating = torf;
}

void Unit::setPosition(Vector2D pos)
{
	mPosition = pos;
}

void Unit::setSprite(Sprite *s)
{
	if(mConstantFrame)
	{
		delete mConstantFrame;
	}
	mConstantFrame = s;
}

Vector2D &Unit::getPosition()
{
	return mPosition;
}

int Unit::getWidth()
{
	if(mAnimation)
	{
		return mAnimation->getCurrentSprite()->getWidth();
	}
	else if(mConstantFrame)
	{
		return mConstantFrame->getWidth();
	}

	return 0;
}

int Unit::getHeight()
{
	if(mAnimation)
	{
		return mAnimation->getCurrentSprite()->getHeight();
	}
	else if(mConstantFrame)
	{
		return mConstantFrame->getHeight();
	}

	return 0;
}

void Unit::collide(Unit * o)
{
	o->collide(this);
}

bool Unit::squareCollision(Unit *other)
{
	return (mPosition.getX() < other->getPosition().getX() + other->getWidth() && mPosition.getX() + getWidth() > other->getPosition().getX() && mPosition.getY() < other->getPosition().getY() + other->getHeight() && mPosition.getY() + getHeight() > other->getPosition().getY());

	/*std::vector<Vector2D> thisVertices;
	thisVertices.push_back(mPosition);
	thisVertices.push_back(Vector2D(mPosition.getX(), mPosition.getY() + getHeight()));
	thisVertices.push_back(Vector2D(mPosition.getX() + getWidth(), mPosition.getY() + getHeight()));
	thisVertices.push_back(Vector2D(mPosition.getX() + getWidth(), mPosition.getY()));

	for(unsigned int i = 0; i < thisVertices.size(); i++)
	{
		if(other->contains(thisVertices[i]))
		{
			return true;
		}
	}

	std::vector<Vector2D> otherVertices;
	otherVertices.push_back(other->mPosition);
	otherVertices.push_back(Vector2D(other->mPosition.getX(), other->mPosition.getY() + other->getHeight()));
	otherVertices.push_back(Vector2D(other->mPosition.getX() + other->getWidth(), other->mPosition.getY() + other->getHeight()));
	otherVertices.push_back(Vector2D(other->mPosition.getX() + other->getWidth(), other->mPosition.getY()));

	for(unsigned int i = 0; i < otherVertices.size(); i++)
	{
		if(contains(otherVertices[i]))
		{
			return true;
		}
	}

	return false;*/
}