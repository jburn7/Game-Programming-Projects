#pragma once
#include "meleeWeaponBehavior.h"
#include "Entity.h"
#include "attack.h"
#include "game.h"
#include "weapon.h"

MeleeWeaponBehavior::MeleeWeaponBehavior()
{
}

MeleeWeaponBehavior::~MeleeWeaponBehavior()
{
}

Attack *MeleeWeaponBehavior::generateAttack(double attackTime, double timeOfAttack, float weaponDamage, float baseDamage, const Vector2D &origin, Vector2D &topLeft, int xDirection, Entity *e, Weapon *w, std::string &weaponTier, const float degrees)
{
	mTimeAttackLasts = attackTime;
	mTimeOfLastAttack = timeOfAttack;

	if(xDirection != 1 && xDirection != -1)
	{
		xDirection = 1;
	}

	Vector2D pos(origin);
	pos.setX(pos.getX() + topLeft.getX() * xDirection);
	pos.setY(pos.getY() + topLeft.getY());

	//maybe make a weaponDamageCalculate function to have this calculation in only one spot
	//select animation based on weapon tier
	Attack *m = new Attack(weaponDamage + baseDamage / 2, new MeleeWeaponBehavior(*this), pos, e, w, w->getAttackAnimation());

	if(xDirection == -1)
	{
		m->getHitBox().move(-m->getHitBox().getWidth());
	}

	return m;
}

void MeleeWeaponBehavior::update(int timeElapsed, Entity *owner, bool &remove, Attack *attack, Vector2D &startPos)
{
	if(timeElapsed - mTimeOfLastAttack >= mTimeAttackLasts)
	{
		remove = true;
	}

	//attack->setPosition(owner->getHandCoord());
	attack->mPosition = owner->getHandCoord();
	attack->mHitbox.move(owner->getVelocity());
	if(owner->isFacingLeft() != attack->isFacingLeft())
	{
		if(owner->isFacingLeft())
		{
			attack->getHitBox().move(-owner->getWidth());
		}
		else
		{
			attack->getHitBox().move(owner->getWidth());
		}
	}

	attack->setFacingLeft(owner->isFacingLeft());
}

void MeleeWeaponBehavior::setInactive(Attack *a)
{
	a->mInactive = true;
}
