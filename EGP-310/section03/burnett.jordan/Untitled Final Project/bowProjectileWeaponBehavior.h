#pragma once
#include "weaponBehavior.h"
#include "attack.h"

class BowProjectileWeaponBehavior : public WeaponBehavior
{
public:
	BowProjectileWeaponBehavior();
	virtual ~BowProjectileWeaponBehavior();

	virtual Attack *generateAttack(double attackTime, double timeOfAttack, float weaponDamage, float baseDamage, const Vector2D &origin, Vector2D &topLeft, int xDirection, Entity *e, Weapon *w, std::string &weaponTier, const float degrees = 0.f);

	virtual void setVelocity(const Vector2D &vel);

	virtual void setInactive(Attack *a);

	virtual void update(int timeElapsed, Entity *owner, bool &remove, Attack *attack, Vector2D &startPos);
private:
	Vector2D mVelocity;
	float mDegrees;
};