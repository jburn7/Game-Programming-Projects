#pragma once
#include "MeleeWeaponBehavior.h"
#include "bowProjectileWeaponBehavior.h"
#include "Attack.h"
#include "jsonData.h"
#include "EventSystem.h"
#include "playSoundEvent.h"
#include "unitAddEvent.h"

enum WeaponType
{
	W_LANCE,
	W_SWORD,
	W_AXE,
	W_BOW,
	//New types here
	W_NUMTYPES
};

class Weapon :public Trackable
{
	friend class Attack;
public:
	Weapon(WeaponType wType, int wScale);
	virtual ~Weapon();

	//give each Attack a pointer to its owner, use fwd dec to Update Attack() as a reg world object then it uses its entity owner as necessary in Update, along with
	//normal params
	virtual Attack *generateAttack(int timeElapsed, float baseDamage, const Vector2D &origin, Vector2D &topLeft, int xDirection, Entity *e, const Vector2D &targetLoc = Vector2D());

	virtual void save(rapidjson::Value &obj, rapidjson::Value &val, rapidjson::Document::AllocatorType &alloc);

	bool isWeaponReady(double timeElapsed);

	Animation* getAttackAnimation();
	std::string getTypeName();

	int getWeaponScale() { return mWeaponScale; }
protected:
	void loadJSON();
	void removeAttack(Attack *a);

	int mWeaponTime; //time that weapon is active
	int mWeaponReset; //time until weapon can attack again
	int mTimeOfAttack;

	float mWidth;
	float mHeight; //data for melee weapons only

	float mWeaponDamage;
	int mWeaponScale; //0 = bronze, 1 = steel, 2 = silver
	WeaponType mWeaponType;
	WeaponBehavior *mpBehavior;

	std::vector<std::string> mWeaponMap;
	std::vector<std::string> mWeaponTypeMap;
	std::vector<Attack*> mpAttacks; //delete all attacks upon weapon being deleted
};