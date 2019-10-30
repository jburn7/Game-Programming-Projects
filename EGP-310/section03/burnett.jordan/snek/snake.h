#pragma once
#include "unit.h"
#include "jsonData.h"
#include "EventSystem.h"
#include "unitRemoveEvent.h"
#include "EventListener.h"
#include "foodEatEvent.h"
#include "rotateSnakeEvent.h"

class Snake : public Unit, public EventListener
{
public:
	Snake(Vector2D &pos, Animation *a, int velocityTier);
	Snake(Vector2D &pos, Sprite *s, int velocityTier);
	~Snake();

	//operator overloads
	//bool operator>(const Vector2D &pivot);

	void addBodyPart();

	virtual void draw();

	virtual void handleEvent(const Event &theEvent);

	virtual void update(double timeElapsed);

	void reset(Vector2D &snakeSpawn);

	void setVelocity(const Vector2D &vel);

	void setVelocityTier(int tier);

	void setTimeToMove(int time);

	//collision functions
	bool squareCollision(Unit *other);

	virtual void collide(Unit *o);
	virtual void collide(ScorePowerUp *o);
	virtual void collide(SnakeIncrementPowerUp *o);
	virtual void collide(SpeedPowerUp *o);
	virtual void collide(Wall *o);
private:
	void correctAndRotate(const Vector2D &position, Direction dir);
	void deleteBody(Snake *s);
	bool headCollision(Unit *other);
	int getNumSegments();
	bool snakeCollision(Unit *other);
	void rotate(Direction dir);
	void init(int velocityTier);

	//must be hardcoded so that all other data isn't hardcoded
	const char *JSONPATH = "snake";

	std::string mTailFileName, mBodyFileName;

	static int mTimeOfLastSpeedPowerUp;
	int mTimeSpeedPowerUpLasts;
	bool mSnakeHitSpeedPowerUp; //use this to avoid passing timeElapsed to my collision functions

	Vector2D mVelocity;

	//used when the game wasn't based on grid movment, but I'm keeping it for fear of breaking the movement code
	static int mVelocityTier, mOriginalVelocityTier;
	static int mVelocityTierModifier;

	Direction mDir;
	static int mTimeOfLastMove;
	static double mTimeToMove, mOriginalTimeToMove;
	
	static std::map<Vector2D, Direction> mPivots;
	int mNumBody;
	static int msNumBodies;

	Snake *mpNext;
	static Snake *mspHead;
};