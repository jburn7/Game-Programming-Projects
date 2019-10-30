#pragma once
#include "hitBox.h"
#include "Vector2D.h"

class Attack;
class Entity;
class Weapon;

class WeaponBehavior : public Trackable
{
public:
	WeaponBehavior();
	virtual ~WeaponBehavior();

	virtual Attack *generateAttack(double attackTime, double timeOfAttack, float weaponDamage, float baseDamage, const Vector2D &origin, Vector2D &topLeft, int xDirection, Entity *e, Weapon *w, std::string &weaponTier, const float degrees = 0.f);

	virtual void setVelocity(const Vector2D &vel) {}

	virtual void setInactive(Attack *a) {}

	virtual void update(int timeElapsed, Entity *owner, bool &remove, Attack *attack, Vector2D &startPos);
protected:
	double mTimeAttackLasts;
	double mTimeOfLastAttack;
};
