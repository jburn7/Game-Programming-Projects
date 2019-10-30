#include "weaponBehavior.h"

WeaponBehavior::WeaponBehavior()
{
}

WeaponBehavior::~WeaponBehavior()
{
}

Attack *WeaponBehavior::generateAttack(double attackTime, double timeOfAttack, float weaponDamage, float baseDamage, const Vector2D & origin, Vector2D & topLeft, int xDirection, Entity * e, Weapon * w, std::string & weaponTier, const float degrees)
{
	return nullptr;
}

void WeaponBehavior::update(int timeElapsed, Entity *owner, bool &remove, Attack *attack, Vector2D &startPos)
{
}
