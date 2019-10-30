#include "weapon.h"
#include "game.h"

Weapon::Weapon(WeaponType wType, int wScale)
{
	if((unsigned int)wScale > mWeaponMap.size() - 1)
	{
		wScale = mWeaponMap.size() - 1;
	}
	mWeaponScale = wScale;
	mWeaponType = wType;

	loadJSON();

	switch(mWeaponType)
	{
	case W_LANCE:
	case W_SWORD:
	case W_AXE:
		mpBehavior = new MeleeWeaponBehavior();
		break;
	case W_BOW:
		mpBehavior = new BowProjectileWeaponBehavior();
		break;
	}
}

Weapon::~Weapon()
{
	for(auto &a : mpAttacks)
	{
		gpEventSystem->fireEvent(new UnitRemoveEvent(UNIT_REMOVE_EVENT, a));
		a->mRemove = true;
	}

	mpAttacks.clear();

	if(mpBehavior)
	{
		delete mpBehavior;
		mpBehavior = NULL;
	}
}

Attack *Weapon::generateAttack(int timeElapsed, float baseDamage, const Vector2D &origin, Vector2D &topLeft, int xDirection, Entity *e, const Vector2D &targetLoc)
{
	mTimeOfAttack = timeElapsed;

	if(xDirection != 1 && xDirection != -1)
	{
		xDirection = 1;
	}

	//float degrees = atan2(targetLoc.getY() - origin.getY(), targetLoc.getX() - origin.getX()) * 180.f / 3.1415926f;
	float degrees = atan((targetLoc.getY() - e->getCenter().getY()) / (targetLoc.getX() - e->getCenter().getX())) * 180.f / 3.1415926f;

	//maybe make a weaponDamageCalculate function to have this calculation in only one spot
	//select animation based on weapon tier
	Attack *m = mpBehavior->generateAttack(mWeaponTime, mTimeOfAttack, mWeaponDamage, baseDamage, origin, topLeft, xDirection, e, this, mWeaponMap[mWeaponScale], degrees);
	//might be slow to access the json for objects that spawn frequently like attacks
	rapidjson::Value &val = JSONData::getInstance()->getJSON()[std::string(mWeaponMap[mWeaponScale] + mWeaponTypeMap[mWeaponType]).c_str()]["color"];
	m->setColor(Color(val["r"].GetInt(), val["g"].GetInt(), val["b"].GetInt()));
	rapidjson::Value &doc = JSONData::getInstance()->getJSON()[std::string(mWeaponMap[mWeaponScale] + mWeaponTypeMap[mWeaponType]).c_str()];
	if(doc.HasMember("velocity"))
	{
		float radians = degrees * 3.1415926f / 180.f;
		m->setVelocity(Vector2D(doc["velocity"].GetFloat() * cos(radians) * xDirection, xDirection * doc["velocity"].GetFloat() * sin(radians)));
	}
	gpEventSystem->fireEvent(new UnitAddEvent(UNIT_ADD_EVENT, m));
	gpEventSystem->fireEvent(new PlaySoundEvent(PLAY_SOUND_EVENT, "default_attack_sound"));
	mpAttacks.push_back(m);
	return m;
}

void Weapon::save(rapidjson::Value &obj, rapidjson::Value &val, rapidjson::Document::AllocatorType &alloc)
{
	rapidjson::Value temp(rapidjson::kObjectType);
	temp.AddMember("weapon_type", mWeaponType, alloc);
	temp.AddMember("weapon_scale", mWeaponScale, alloc);
	obj.AddMember("weapon", temp, alloc);
}

bool Weapon::isWeaponReady(double timeElapsed)
{
	//return true if time since attack is greater than weapon time
	return (timeElapsed - mTimeOfAttack >= mWeaponReset);
}

Animation* Weapon::getAttackAnimation()
{
	std::string animation;
	switch (mWeaponType) {
	case W_SWORD:
		animation = "sword_attack_animation";
		break;
	case W_LANCE:
		animation = "lance_attack_animation";
		break;
	case W_BOW:
		animation = "bow_projectile_attack_animation";
		break;
	case W_AXE:
		animation = "axe_attack_animation";
		break;
	}
	return new Animation(*Game::getInstance()->getUnitManager().getUnitAnimations()[animation]);
}

std::string Weapon::getTypeName()
{
	return mWeaponTypeMap[mWeaponType];
}

void Weapon::loadJSON()
{
	rapidjson::Value &doc = JSONData::getInstance()->getJSON();

	for(auto &a : doc["weaponScale"].GetArray())
	{
		mWeaponMap.push_back(a.GetString());
	}
	for(auto &a : doc["weapon_types"].GetArray())
	{
		mWeaponTypeMap.push_back(a.GetString());
	}

	mWidth = doc[std::string(mWeaponMap[mWeaponScale] + mWeaponTypeMap[mWeaponType]).c_str()]["width"].GetFloat();
	mHeight = doc[std::string(mWeaponMap[mWeaponScale] + mWeaponTypeMap[mWeaponType]).c_str()]["height"].GetFloat();
	mWeaponDamage = doc[std::string(mWeaponMap[mWeaponScale] + mWeaponTypeMap[mWeaponType]).c_str()]["damage"].GetFloat();
	mWeaponTime = doc[std::string(mWeaponMap[mWeaponScale] + mWeaponTypeMap[mWeaponType]).c_str()]["weapon time"].GetInt();
	mWeaponReset = doc[std::string(mWeaponMap[mWeaponScale] + mWeaponTypeMap[mWeaponType]).c_str()]["weapon reset"].GetInt();
}

void Weapon::removeAttack(Attack *a)
{
	if(mpAttacks.size() != 0)
	{
		for(auto w : mpAttacks)
		{
			for(unsigned int i = mpAttacks.size() - 1; i >= 0; i--)
			{
				if(mpAttacks[i] == a)
				{
					mpAttacks.erase(mpAttacks.begin() + i);
					return;
				}
			}
		}
	}
}