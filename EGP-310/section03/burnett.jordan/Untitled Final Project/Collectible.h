#pragma once
#include"unit.h"
#include "collisionComponent.h"
#include "EventSystem.h"
#include "unitRemoveEvent.h"
#include "playSoundEvent.h"
#include "jsonData.h"
#include "Armor.h"
#include "weapon.h"
#include <string>

enum PICKUP_TYPE {
	P_INVALID = -1,
	P_WEAPON,
	P_ARMOR,
	P_SPEED,
	P_SLOWDOWN,
	P_JUMP,
	P_VICTORY_ITEM
};

class Collectible :public Unit
{
	friend class ItemSpawner;
	friend class Level;
protected:
	void setSoundName();

	PICKUP_TYPE mPickupType;
	ItemSpawner* mSpawner;
	int mDuration;
	float mPower;
	int mWidth;
	int mHeight;
	std::string mCollectedSoundName;
public:
	Collectible(Vector2D pos, Sprite* s);
	Collectible(Vector2D pos, Sprite* s, PICKUP_TYPE type);
	Collectible(Vector2D pos, Sprite* s, PICKUP_TYPE type, int duration, float power);
	Collectible(Vector2D pos, Animation* a);
	Collectible(Vector2D pos, Animation* a, PICKUP_TYPE type);
	Collectible(Vector2D pos, Animation* a, PICKUP_TYPE type, int duration, float power);
	Collectible(Vector2D pos, PICKUP_TYPE type, int duration, float power);
	Collectible(rapidjson::Value &saveData);
	virtual ~Collectible();
	virtual void collide(Entity *e);
	virtual void setPosition(const Vector2D &pos);
	virtual void update(int dt);
	virtual void draw();
	virtual void save(rapidjson::Value &obj, rapidjson::Value &val, rapidjson::Document::AllocatorType &alloc);
	void playCollectedSound();
	PICKUP_TYPE getPickupType() const { return mPickupType; };
	int getDuration() const { return mDuration; };
	float getPower() const { return mPower; };
	virtual bool getInputRequired() { return false; };
	void loadCollectibleJSON(const std::string &collectibleType);
};

class ArmorPickup :public Collectible {
protected:
	std::string mArmorType;
	Armor* mArmor;
public:
	ArmorPickup(Vector2D pos, Animation* a, Armor* armor);
	ArmorPickup(rapidjson::Value &saveData);
	virtual ~ArmorPickup();
	virtual void save(rapidjson::Value &obj, rapidjson::Value &val, rapidjson::Document::AllocatorType &alloc);
	std::string getArmorType() { return mArmorType; };
	Armor* getArmor() { return mArmor; };
	virtual bool getInputRequired() { return true; };
};

class WeaponPickup :public Collectible {
	friend class CollisionComponent;
protected:
	std::string mWeaponType;
	Weapon* mWeapon;
public:
	WeaponPickup(Vector2D pos, Animation* a, Weapon* weapon);
	WeaponPickup(rapidjson::Value &saveData);
	virtual ~WeaponPickup();
	std::string getWeaponType() { return mWeaponType; };
	Weapon* getWeapon() { return mWeapon; };
	virtual bool getInputRequired() { return true; };
};