#pragma once
#include "animation.h"
#include "hitBox.h"
#include "jsonData.h"
#include <map>
#include <string>

class Entity;
class Wall;
class Attack;
class Collectible;

enum UnitType
{
	UNDEF = -1,
	U_ATTACK,
	U_ENTITY,
	U_WALL,
	U_COLLECTIBLE
};

class Unit : public Trackable
{
	friend class UnitManager;
public:
	Unit();

	Unit(Vector2D pos);

	Unit(Vector2D pos, Animation *a);

	Unit(Vector2D pos, Sprite *s);

	Unit(rapidjson::Value &saveData);

	virtual ~Unit();

	void cleanup();

	virtual void draw();

	virtual void save(rapidjson::Value &obj, rapidjson::Value &val, rapidjson::Document::AllocatorType &alloc);

	virtual void update(int timeElapsed);

	virtual void update(int timeElapsed, std::vector<std::vector<bool>> &wallMap);

	//collision functions
	bool contains(Vector2D &loc);

	//setters
	void setAnimating(bool torf);

	void setColor(const Color &color);

	virtual void setPosition(const Vector2D &pos);

	void setSprite(Sprite* s);	//Backwards-compatibility, need to change all calls to this into animation names
	void setAnimation(std::string anim);

	//getters
	Vector2D getCenter();
	bool isFacingLeft(){ return mFacingLeft; }
	Vector2D &getPosition();
	virtual float getWidth();
	virtual float getHeight();
	HitBox &getHitBox(){ return mHitbox; }

	//collision functions, using double dispatch because I despise static casting
	//each function will handle the resolution for both objects, e.g. Entity::collide(Wall *o) will move the Entity and then do whatever needs to be done to the wall
	virtual void collide(Unit *o) {}
	virtual void collide(Attack *g) {}
	virtual void collide(Entity *g) {}
	virtual void collide(Wall *o) {}
protected:
	virtual void loadHitbox();
	virtual void modifyDifficulty(float multiplier);
	virtual bool tryRemove(){ return true; } //returns true if the object is removable such as a powerup, false if it's something like a wall

	Vector2D mPosition;
	std::string mCurrentAnimation;
	std::map<std::string, Animation*> mAnimationMap;
	std::map<std::string, std::string> mSoundMap;
	//bool mAnimating;
	bool mFacingLeft; //used for flipping or not flipping the sprites
	bool mRemove;
	HitBox mHitbox;
	UnitType mType;

	Color *mAlternateColor; //if null, draw normally, else call the draw function that takes a color
	
	static CollisionComponent mCollision;
};