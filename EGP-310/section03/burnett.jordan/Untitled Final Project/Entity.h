#pragma once
#include "unit.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"
#include "PlayerinputComponent.h"
#include "meleeEntityInputComponent.h"
#include "Weapon.h"
#include "EventSystem.h"
#include "unitRemoveEvent.h"
#include "movePlayerEvent.h"
#include "victoryItemFoundEvent.h"
#include "Timer.h"
#include "Armor.h"

enum EntityType
{
	E_MELEE_ENEMY,
	E_BOW_ENEMY
};

class Entity : public Unit
{
	friend class PhysicsComponent;
	friend class CollisionComponent;
	friend class PlayerInputComponent;
	friend class MeleeEntityInputComponent;
	friend class ProjectileEntityInputComponent;
public:
	Entity(Vector2D pos, InputComponent* input, const std::string &entityType);
	Entity(rapidjson::Value &saveData);
	~Entity();

	virtual void draw();

	virtual void save(rapidjson::Value &obj, rapidjson::Value &val, rapidjson::Document::AllocatorType &alloc);

	virtual void update(int timeElapsed);

	virtual void update(int timeElapsed, std::vector<std::vector<bool>> &wallMap);

	virtual void takeDamage(float damage);

	float getCollisionDamageValue();

	//move back to protected if necessary
	void entityAttack(int timeElapsed);

	virtual void setPosition(const Vector2D &pos);

	//returns coordinates of entity's hand in relation to its center
	//will return different values whether the entity is facing left or right
	Vector2D getHandCoord();
	Vector2D &getVelocity(){ return mVelocity; }

	virtual void collide(Unit *g);
	virtual void collide(Entity *g) {}
	virtual void collide(Wall *o);
	/*virtual void collide(Endpoint *g) {}
	virtual void collide(WeaponPickup *g) {}
	virtual void collide(Platform *g);
	virtual void collide(Jumper *g);*/
	virtual void collide(Attack *g);
protected:
	void die();
	virtual void modifyDifficulty(float multiplier);
	void move(Vector2D &vel);

	void loadEntityJSON(const std::string &entityType);

	Vector2D mVelocity;
	float mMaxHorizVelocity, mMaxVerticalVelocity;
	float mJumpVelocity;

	//move vairables, modified by the Entity's InputComponent
	bool mMoveLeft, mMoveRight, mJump, mAttack; //add more as needed

	//to handle jumping, calculate after SAT if the player was on something solid in the previous frame. if so, the the player can jump
	bool mCanJump;
	//another bool to handle jumping, and probably other stuff
	bool mCollided;
	bool mIsOnGround;

	bool mIsPlayer;

	EntityType mEntityType;

	HitBox mFeetBox; //for use with testing ground collision

	Sprite *mpHealthBar;
	Sprite *mpHealthBarOutline;

	//Entity stuff
	int mWidth;
	int mHeight;
	int mMaxYValue; //entity dies after falling out of the level
	float mHealth, mMaxHealth;
	float mDamageVal;
	Vector2D mHandRight; //assuming shapes are horizontally symmetrical, this hand coord can identify both the left and right hand by multiplying the x value in
							//relation to the object's center by -1
	Weapon *mpWeapon;
	std::map<std::string, Armor*> mArmor;
	Vector2D mPreviousPosition;
	Timer mTimer; //used for tracking invincibility times, since getting timeElapsed to the takeDamage() function would be difficult
	std::map<std::string, std::pair<int, float>> mPowerupRemaining;	//Time remaining on the various powerups
	double mHitRegenTime, mTimeLastHit; //time an entity is invincible after being hurt
	bool mHitRecovery;
	bool mItemPickup = false;

	PhysicsComponent mPhysics;
	//CollisionComponent mCollision;
	InputComponent* mInput;

	static std::map<EntityType, std::string> msEntityTypeMap;
	static bool msInit;
private:
	void applyCollectible(Collectible *c);
};