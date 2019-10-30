#include "bowProjectileWeaponBehavior.h"
#include "game.h"

BowProjectileWeaponBehavior::BowProjectileWeaponBehavior()
{
}

BowProjectileWeaponBehavior::~BowProjectileWeaponBehavior()
{
}

Attack *BowProjectileWeaponBehavior::generateAttack(double attackTime, double timeOfAttack, float weaponDamage, float baseDamage, const Vector2D &origin, Vector2D &topLeft, int xDirection, Entity *e, Weapon *w, std::string &weaponTier, const float degrees)
{
	mTimeAttackLasts = attackTime;
	mTimeOfLastAttack = timeOfAttack;

	Vector2D pos(origin);
	pos.setX(pos.getX() + topLeft.getX() * xDirection);
	pos.setY(pos.getY() + topLeft.getY());

	Attack *m = new Attack((weaponDamage + baseDamage) / 2, new BowProjectileWeaponBehavior(*this), pos, e, w, new Animation(*Game::getInstance()->getUnitManager().getUnitAnimations()["bow_projectile_attack_animation"]), degrees);

	m->getHitBox().rotate(degrees);

	return m;
}

void BowProjectileWeaponBehavior::setVelocity(const Vector2D &vel)
{
	mVelocity = vel;
}

void BowProjectileWeaponBehavior::setInactive(Attack *a)
{
	gpEventSystem->fireEvent(new UnitRemoveEvent(UNIT_REMOVE_EVENT, a));
}

void BowProjectileWeaponBehavior::update(int timeElapsed, Entity *owner, bool &remove, Attack *attack, Vector2D &startPos)
{
	if(timeElapsed - mTimeOfLastAttack >= mTimeAttackLasts)
	{
		remove = true;
	}

	attack->setPosition(attack->getPosition() + mVelocity);
	attack->mHitbox.setPosition(attack->getPosition());
}
