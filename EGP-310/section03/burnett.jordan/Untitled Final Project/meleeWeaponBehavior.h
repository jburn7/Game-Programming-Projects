#pragma once
#include "weaponBehavior.h"

class MeleeWeaponBehavior : public WeaponBehavior
{
public:
	MeleeWeaponBehavior();
	virtual ~MeleeWeaponBehavior();

	virtual Attack *generateAttack(double attackTime, double timeOfAttack, float weaponDamage, float baseDamage, const Vector2D &origin, Vector2D &topLeft, int xDirection, Entity *e, Weapon *w, std::string &weaponTier, const float degrees = 0.f);

	virtual void update(int timeElapsed, Entity *owner, bool &remove, Attack *attack, Vector2D &startPos);

	virtual void setInactive(Attack *a);
};