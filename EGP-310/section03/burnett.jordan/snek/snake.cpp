#include "snake.h"
#include "game.h"

const double pi = 3.14159265358979323846;

//adapted from: http://www.cplusplus.com/forum/general/142134/#msg750411
bool operator<(const Vector2D &lhs, const Vector2D &rhs)
{
	if(lhs.getY() < rhs.getY())  return true;
	if(lhs.getY() > rhs.getY())  return false;
	return (lhs.getX() < rhs.getX());
}

bool operator==(const Vector2D &lhs, const Vector2D &rhs)
{
	return lhs.getX() == rhs.getX() && lhs.getY() == rhs.getY();
}

std::map<Vector2D, Direction> Snake::mPivots = {};
Snake* Snake::mspHead = NULL;
int Snake::msNumBodies = 0;
int Snake::mVelocityTier = 0;
int Snake::mOriginalVelocityTier = 0;
int Snake::mVelocityTierModifier = 0;
int Snake::mTimeOfLastSpeedPowerUp = 0;
double Snake::mTimeToMove = 0;
int Snake::mTimeOfLastMove = 0;
double Snake::mOriginalTimeToMove = 0;

Snake::Snake(Vector2D &pos, Animation *a, int velocityTier) : Unit(pos, a)
{
	init(velocityTier);
}

Snake::Snake(Vector2D &pos, Sprite *s, int velocityTier) : Unit(pos, s)
{
	init(velocityTier);
}

Snake::~Snake()
{
	reset(Vector2D(0, 0));

	if(mspHead == this)
	{
		mspHead = NULL;
	}
}

void Snake::addBodyPart()
{
	Snake *temp = this;
	//find tail of snake, then add a body part next to that based on what direction the tail is moving
	while(temp->mpNext != NULL)
	{
		temp = temp->mpNext;
	}
	if(temp != mspHead)
	{
		temp->setSprite(new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer(mBodyFileName)));
	}
	if(temp->mVelocity.getX() < 0)
	{
		temp->mpNext = new Snake(Vector2D(temp->mPosition.getX() + temp->getWidth(), temp->mPosition.getY()), new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer(mTailFileName)), mVelocityTier);
	}
	else if(temp->mVelocity.getX() > 0)
	{
		temp->mpNext = new Snake(Vector2D(temp->mPosition.getX() - temp->getWidth(), temp->mPosition.getY()), new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer(mTailFileName)), mVelocityTier);
	}
	else if(temp->mVelocity.getY() < 0)
	{
		temp->mpNext = new Snake(Vector2D(temp->mPosition.getX(), temp->mPosition.getY() + temp->getHeight()), new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer(mTailFileName)), mVelocityTier);
	}
	else if(temp->mVelocity.getY() > 0)
	{
		temp->mpNext = new Snake(Vector2D(temp->mPosition.getX(), temp->mPosition.getY() - temp->getHeight()), new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer(mTailFileName)), mVelocityTier);
	}

	temp->mpNext->setVelocity(temp->mVelocity);
	temp->mpNext->mDir = temp->mDir;
}

void Snake::draw()
{
	double theta;
	switch(mDir)
	{
	case RIGHT:
		theta = 0;
		break;
	case UP:
		theta = 3 * pi / 2;
		break;
	case DOWN:
		theta = pi / 2;
		break;
	case LEFT:
		theta = pi;
		break;
	default:
		theta = 0;
		break;
	}
	if(mAnimation)
	{
		Game::getInstance()->getGraphics().draw(mPosition, *mAnimation->getCurrentSprite(), theta);
	}
	else
	{
		Game::getInstance()->getGraphics().draw(mPosition, *mConstantFrame, theta);
	}

	if(mpNext)
	{
		mpNext->draw();
	}
}

void Snake::handleEvent(const Event &theEvent)
{
	if(theEvent.getType() == ROTATE_SNAKE_EVENT)
	{
		const RotateSnakeEvent &ev = static_cast<const RotateSnakeEvent&>(theEvent);
		rotate(ev.getDirection());
		if(this == mspHead && mpNext)
		{
			mPivots.insert(std::pair<Vector2D, Direction>(mPosition, ev.getDirection()));
		}
	}
}

void Snake::update(double timeElapsed)
{
	Unit::update(timeElapsed);

	if(timeElapsed > mTimeOfLastSpeedPowerUp + mTimeSpeedPowerUpLasts)
	{
		mTimeToMove = mOriginalTimeToMove;
	}

	if(mspHead != this)
	{
		int i = 0;
	}

	if(timeElapsed > mTimeOfLastMove + mTimeToMove)
	{
		mPosition += mVelocity;
		if(!mpNext)
		{
			mTimeOfLastMove = timeElapsed;
		}
	}

	mSnakeHitSpeedPowerUp = false;

	std::vector<Vector2D> toErase;

	std::map<Vector2D, Direction>::iterator iter = mPivots.begin();
	while(iter != mPivots.end())
	{
		if(mPosition == iter->first)
		{
			rotate(iter->second);
			if(!mpNext) //if tail of snake, no more rotations to complete
			{
				toErase.push_back(mPosition);
			}
		}
		iter++;
	}

	for(unsigned int i = 0; i < toErase.size(); i++)
	{
		mPivots.erase(toErase[i]);
	}

	if(mpNext)
	{
		mpNext->update(timeElapsed);
	}

	//loop through body parts, send a reset event if the head is in contact with any others
	if(mspHead == this)
	{
		Snake *temp = mspHead;
		while(temp->mpNext)
		{
			temp = temp->mpNext;
			if(Unit::squareCollision(temp))
			{
				gpEventSystem->fireEvent(IncrementLivesEvent(INCREMENT_LIVES_EVENT, -1));
				break;
			}
		}
	}

	//head is the only body part that needs to check for collision
	//the snake is the only moving object, so we'll just have it check itself against all other objects, yielding a linear collision time
	if(mspHead == this)
	{
		for(int i = Game::getInstance()->getUnitManager().getUnits().size() - 1; i >= 0; i--)
		{
			if(headCollision(Game::getInstance()->getUnitManager().getUnits()[i]))
			{
				Unit::collide(Game::getInstance()->getUnitManager().getUnits()[i]);
			}
		}

		if(mSnakeHitSpeedPowerUp)
		{
			mTimeOfLastSpeedPowerUp = timeElapsed;
		}
	}
}

void Snake::reset(Vector2D &snakeSpawn)
{
	mPosition = snakeSpawn;
	mVelocityTierModifier = 0;
	mDir = RIGHT;
	mVelocity.setY(0);
	mVelocityTier = mOriginalVelocityTier;
	mVelocity.setX(JSONData::getInstance()->getJSON()["game"]["velocityTier"].GetArray()[mVelocityTier].GetInt());

	mPosition -= mVelocity; //to account for the snake updating once without the user seeing it

	if(mpNext)
	{
		delete mpNext;
		mpNext = NULL;
	}
	mPivots.clear();
}

void Snake::setVelocity(const Vector2D &vel)
{
	mVelocity = vel;
}

void Snake::setVelocityTier(int tier)
{
	mVelocityTier = tier;
}

void Snake::setTimeToMove(int time)
{
	if(mTimeToMove == 0)
	{
		mOriginalTimeToMove = time;
	}
	mTimeToMove = time;
}

bool Snake::squareCollision(Unit *other)
{
	Snake *temp = mspHead;
	if(temp->mPosition.getX() < other->getPosition().getX() + other->getWidth() && temp->mPosition.getX() + getWidth() > other->getPosition().getX() && temp->mPosition.getY() < other->getPosition().getY() + other->getHeight() && temp->mPosition.getY() + getHeight() > other->getPosition().getY())
	{
		return true;
	}
	while(temp->mpNext)
	{
		temp = temp->mpNext;
		if(temp->mPosition.getX() < other->getPosition().getX() + other->getWidth() && temp->mPosition.getX() + getWidth() > other->getPosition().getX() && temp->mPosition.getY() < other->getPosition().getY() + other->getHeight() && temp->mPosition.getY() + getHeight() > other->getPosition().getY())
		{
			return true;
		}
	}

	return false;
}

void Snake::collide(Unit *o)
{
	if(o != this)
	{
		o->collide(this);
	}
}

void Snake::collide(ScorePowerUp *o)
{
	gpEventSystem->fireEvent(UnitRemoveEvent(UNIT_REMOVE_EVENT, o));
	gpEventSystem->fireEvent(IncrementScoreEvent(INCREMENT_SCORE_EVENT, Game::getInstance()->getLevelManager().getCurrentLevel().getScorePowerupScore()));
}

void Snake::collide(SnakeIncrementPowerUp *o)
{
	addBodyPart();
	gpEventSystem->fireEvent(UnitRemoveEvent(UNIT_REMOVE_EVENT, o));
	gpEventSystem->fireEvent(FoodEatEvent(FOOD_EAT_EVENT, getNumSegments()));
	gpEventSystem->fireEvent(IncrementScoreEvent(INCREMENT_SCORE_EVENT, Game::getInstance()->getLevelManager().getCurrentLevel().getFoodScore()));
}

void Snake::collide(SpeedPowerUp *o)
{
	rapidjson::Value &arr = JSONData::getInstance()->getJSON()["game"]["velocityTier"];
	if(o->getDirection() < 0)
	{
		mTimeToMove = (2 * mTimeToMove / 3);
		if(mTimeToMove < .2 * mOriginalTimeToMove)
		{
			mTimeToMove = .2 * mOriginalTimeToMove;
		}
	}
	else
	{
		mTimeToMove = (4 * mTimeToMove / 3);
		if(mTimeToMove > 3.5 * mOriginalTimeToMove)
		{
			mTimeToMove = 3.5 * mOriginalTimeToMove;
		}
	}
	
	gpEventSystem->fireEvent(UnitRemoveEvent(UNIT_REMOVE_EVENT, o));
	gpEventSystem->fireEvent(IncrementScoreEvent(INCREMENT_SCORE_EVENT, Game::getInstance()->getLevelManager().getCurrentLevel().getSpeedScore()));

	mSnakeHitSpeedPowerUp = true;
}

void Snake::collide(Wall *o)
{
	gpEventSystem->fireEvent(IncrementLivesEvent(INCREMENT_LIVES_EVENT, -1));
}

void Snake::rotate(Direction dir)
{
	Direction old = mDir;
	mDir = dir;
	if(dir == UP && mVelocity.getY() == 0)
	{
		mVelocity.setY(mVelocity.getX());
		if(mVelocity.getX() > 0)
		{
			mVelocity.setY(-1 * mVelocity.getX());
		}
		mVelocity.setX(0);
	}
	else if(dir == DOWN && mVelocity.getY() == 0)
	{
		mVelocity.setY(mVelocity.getX());
		if(mVelocity.getX() < 0)
		{
			mVelocity.setY(-1 * mVelocity.getX());
		}
		mVelocity.setX(0);
	}
	else if(dir == LEFT && mVelocity.getX() == 0)
	{
		mVelocity.setX(mVelocity.getY());
		if(mVelocity.getY() > 0)
		{
			mVelocity.setX(-1 * mVelocity.getX());
		}
		mVelocity.setY(0);
	}
	else if(dir == RIGHT && mVelocity.getX() == 0)
	{
		mVelocity.setX(mVelocity.getY());
		if(mVelocity.getY() < 0)
		{
			mVelocity.setX(-1 * mVelocity.getX());
		}
		mVelocity.setY(0);
	}
	else
	{
		mDir = old;
	}
}

void Snake::init(int velocityTier)
{
	if(!mspHead)
	{
		mspHead = this;
		gpEventSystem->addListener(ROTATE_SNAKE_EVENT, this);
		mOriginalVelocityTier = mVelocityTier = velocityTier;
		mTimeOfLastSpeedPowerUp = 0;
	}

	mNumBody = msNumBodies++;
	rapidjson::Document &doc = JSONData::getInstance()->getJSON();

	mTailFileName = doc[JSONPATH]["tailFileName"].GetString();
	mBodyFileName = doc[JSONPATH]["bodyFileName"].GetString();

	mTimeSpeedPowerUpLasts = doc[JSONPATH]["timeSpeedPowerUpLasts"].GetInt();
	mSnakeHitSpeedPowerUp = false;

	mpNext = NULL;

	mVelocityTierModifier = 0;
	//mMaxVelocity = doc[JSONPATH]["maxVelocity"].GetInt();
	mVelocity.setX(doc["game"]["velocityTier"].GetArray()[velocityTier].GetInt());
	mDir = RIGHT;
	mVelocity.setY(0);
}

void Snake::correctAndRotate(const Vector2D &position, Direction dir)
{
	switch(mDir)
	{
	case RIGHT:
		mPosition.setY(position.getY());
		break;
	case UP:
		mPosition.setX(position.getX());
		break;
	case DOWN:
		mPosition.setX(position.getX());
		break;
	case LEFT:
		mPosition.setY(position.getY());
		break;
	}
	rotate(dir);
}

void Snake::deleteBody(Snake *s)
{
	if(s->mpNext)
	{
		deleteBody(s->mpNext);
	}
	delete s;
	s->mpNext = NULL;
}

bool Snake::headCollision(Unit *other)
{
	return (mPosition.getX() < other->getPosition().getX() + other->getWidth() && mPosition.getX() + getWidth() > other->getPosition().getX() && mPosition.getY() < other->getPosition().getY() + other->getHeight() && mPosition.getY() + getHeight() > other->getPosition().getY());
}

int Snake::getNumSegments()
{
	int numSegments = 1;
	Snake *temp = mspHead;
	while(temp->mpNext)
	{
		numSegments++;
		temp = temp->mpNext;
	}
	
	return numSegments;
}

bool Snake::snakeCollision(Unit *other)
{
	std::vector<Vector2D> collisionRectPoints; //must be defined clockwise starting with top left
	if(mVelocity.getX() < 0)
	{
		collisionRectPoints.push_back(Vector2D(mPosition.getX() + mVelocity.getX(), mPosition.getY()));
		collisionRectPoints.push_back(mPosition);
		collisionRectPoints.push_back(Vector2D(mPosition.getX(), mPosition.getY() + getHeight()));
		collisionRectPoints.push_back(Vector2D(mPosition.getX() + mVelocity.getX(), mPosition.getY() + getHeight()));
	}
	else if(mVelocity.getX() > 0)
	{
		collisionRectPoints.push_back(Vector2D(mPosition.getX() + getWidth(), mPosition.getY()));
		collisionRectPoints.push_back(Vector2D(mPosition.getX() + mVelocity.getX() + getWidth(), mPosition.getY()));
		collisionRectPoints.push_back(Vector2D(mPosition.getX() + mVelocity.getX() + getWidth(), mPosition.getY() + getHeight()));
		collisionRectPoints.push_back(Vector2D(mPosition.getX() + getWidth(), mPosition.getY() + getHeight()));
	}
	else if(mVelocity.getY() < 0)
	{
		collisionRectPoints.push_back(Vector2D(mPosition.getX(), mPosition.getY() + mVelocity.getY()));
		collisionRectPoints.push_back(Vector2D(mPosition.getX() + getWidth(), mPosition.getY() + mVelocity.getY()));
		collisionRectPoints.push_back(Vector2D(mPosition.getX() + getWidth(), mPosition.getY()));
		collisionRectPoints.push_back(mPosition);
	}
	else
	{
		collisionRectPoints.push_back(Vector2D(mPosition.getX(), mPosition.getY() + getHeight()));
		collisionRectPoints.push_back(Vector2D(mPosition.getX() + getWidth(), mPosition.getY() + getHeight()));
		collisionRectPoints.push_back(Vector2D(mPosition.getX() + getWidth(), mPosition.getY() + getHeight() + mVelocity.getY()));
		collisionRectPoints.push_back(Vector2D(mPosition.getX(), mPosition.getY() + getHeight() + mVelocity.getY()));
	}

	for(unsigned int i = 0; i < collisionRectPoints.size(); i++)
	{
		if(other->contains(collisionRectPoints[i]))
		{
			return true;
		}
	}

	return false;
}