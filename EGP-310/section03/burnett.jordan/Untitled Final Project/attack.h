#pragma once
#include "unit.h"
#include "WeaponBehavior.h"

class Weapon;

class Attack : public Unit
{
	friend class CollisionComponent;
	friend class Weapon;
	friend class MeleeWeaponBehavior;
	friend class BowProjectileWeaponBehavior;
public:
	Attack(float damage, WeaponBehavior *b, Vector2D &startPos, Entity *e, Weapon *w, Animation *a, float degrees = 0.f);
	virtual ~Attack();

	virtual void draw();

	virtual void update(int timeElapsed);

	virtual void update(int timeElapsed, std::vector<std::vector<bool>> &wallMask);

	virtual void save(rapidjson::Value &obj, rapidjson::Value &val, rapidjson::Document::AllocatorType &alloc) {}

	void setInactive();

	void setFacingLeft(bool left);
	virtual void setPosition(const Vector2D &pos);
	void setVelocity(const Vector2D &vel);

	virtual void collide(Unit *o);
	virtual void collide(Entity *o);
protected:
	float mAttackDamage;
	Vector2D mVelocity;
	Vector2D mActualStartPos;
	//bool mDealtDamage; //if true, don't deal damage to recipient twice. might want to consider making this Entity::receivedDamage to allow for splash damage
	std::vector<Entity*> mEntitiesHit;

	float mDegrees;

	bool mInactive; //used to prevent unwanted splash damage

	Entity *mpOwner;
	Weapon *mpWeapon;
	WeaponBehavior *mpBehavior;
};
