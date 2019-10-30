#include "Collectible.h"
#include "game.h"

void Collectible::setSoundName()
{
	//change this to add to mSounds map instead
	switch(mPickupType)
	{
	case P_ARMOR:
		mCollectedSoundName = "default_armor_pickup_sound";
		break;
	case P_JUMP:
		mCollectedSoundName = "default_generic_pickup_sound";
		break;
	case P_SPEED:
		mCollectedSoundName = "default_generic_pickup_sound";
		break;
	case P_SLOWDOWN:
		mCollectedSoundName = "default_generic_pickup_sound";
		break;
	case P_WEAPON:
		mCollectedSoundName = "default_armor_pickup_sound";
		break;
	case P_VICTORY_ITEM:
		mCollectedSoundName = "default_victory_pickup_sound";
		break;
	default:
		mCollectedSoundName = "default_generic_pickup_sound";
		break;
	}
}

Collectible::Collectible(Vector2D pos, Sprite* s) :Unit(pos, s)
{
	mType = U_COLLECTIBLE;
	mPickupType = P_INVALID;
	setSoundName();
}

Collectible::Collectible(Vector2D pos, Sprite* s, PICKUP_TYPE type):Unit(pos, s)
{
	mType = U_COLLECTIBLE;
	mPickupType = type;
	setSoundName();
}

Collectible::Collectible(Vector2D pos, Sprite* s, PICKUP_TYPE type, int duration, float power) : Unit(pos, s)
{
	mType = U_COLLECTIBLE;
	mPickupType = type;
	mDuration = duration;
	mPower = power;
	setSoundName();
}

Collectible::Collectible(Vector2D pos, Animation* a) :Unit(pos, a)
{
	mType = U_COLLECTIBLE;
	mPickupType = P_INVALID;
	setSoundName();
}

Collectible::Collectible(Vector2D pos, Animation* a, PICKUP_TYPE type):Unit(pos,a)
{
	mType = U_COLLECTIBLE;
	mPickupType = type;
	setSoundName();
}

Collectible::Collectible(Vector2D pos, Animation* a, PICKUP_TYPE type, int duration, float power) : Unit(pos, a)
{
	mType = U_COLLECTIBLE;
	mPickupType = type;
	mDuration = duration;
	mPower = power;
	setSoundName();
}

Collectible::Collectible(rapidjson::Value &saveData) : Unit(saveData)
{
	setSoundName();

	mPickupType = PICKUP_TYPE(saveData["pickup_type"].GetInt());


	rapidjson::Document &doc = JSONData::getInstance()->getJSON();
	std::string spritesheetName;
	switch(mPickupType)
	{
	case P_ARMOR:
		spritesheetName = doc["pickup_armor"]["spritesheetName"].GetString();
		break;
	case P_SPEED:
		spritesheetName = doc["pickup_speedup"]["spritesheetName"].GetString();
		break;
	case P_JUMP:
		spritesheetName = doc["pickup_jumpboost"]["spritesheetName"].GetString();
		break;
	case P_SLOWDOWN:
		//spritesheetName = doc["pickup_slowdown"]["spritesheetName"].GetString();
		break;
	case P_VICTORY_ITEM:
		spritesheetName = doc["pickup_victory_item"]["spritesheetName"].GetString();
		break;
	case P_WEAPON:
		//spritesheetName = doc["pickup_weapon"]["spritesheetName"].GetString();
		break;
	}

	if(spritesheetName != "")
		mAnimationMap["default"] = new Animation(std::vector<Sprite*>(1, new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer(spritesheetName))));

	loadHitbox();
}

Collectible::~Collectible()
{
	if (mSpawner)
		mSpawner->mShouldSpawn = true;
}

void Collectible::collide(Entity* e)
{

}

void Collectible::setPosition(const Vector2D &pos)
{
	Unit::setPosition(pos);
}

void Collectible::update(int dt)
{
	Unit::update(dt);
}

void Collectible::draw() {
	//mHitbox.draw();
	Unit::draw();
}

void Collectible::save(rapidjson::Value &obj, rapidjson::Value &val, rapidjson::Document::AllocatorType &alloc)
{
	Unit::save(obj, val, alloc);

	obj.AddMember("pickup_type", mPickupType, alloc);
}

void Collectible::playCollectedSound()
{
	gpEventSystem->fireEvent(new PlaySoundEvent(PLAY_SOUND_EVENT, mCollectedSoundName));
}

void Collectible::loadCollectibleJSON(const std::string &collectibleType)
{
	rapidjson::Value &doc = JSONData::getInstance()->getJSON();
	mWidth = doc[collectibleType.c_str()]["width"].GetInt();
	mHeight = doc[collectibleType.c_str()]["height"].GetInt();
	for (auto &a : doc[collectibleType.c_str()]["sounds"].GetArray())
	{
		mSoundMap.insert(std::pair<std::string, std::string>(a["description"].GetString(), a["soundBufferName"].GetString()));
	}
	mDuration = doc[collectibleType.c_str()]["duration"].GetInt();
	mPower = doc[collectibleType.c_str()]["power"].GetFloat();
}

ArmorPickup::ArmorPickup(Vector2D pos, Animation* a, Armor* armor)
	:Collectible(pos, a, P_ARMOR)
{
	mArmorType = armor->getType();
	mArmor = armor;
	setColor(*(armor->mColor));
}

ArmorPickup::ArmorPickup(rapidjson::Value &saveData) : Collectible(saveData)
{
	mArmorType = saveData["armor_type"].GetString();

	if(mAnimationMap["default"])
	{
		delete mAnimationMap["default"];
	}
	mAnimationMap["default"] = new Animation(*Game::getInstance()->getUnitManager().getUnitAnimations()[mArmorType + "_pickup_animation"]);

	//mArmor = new Armor(JSONData::getInstance()->getJSON()[mArmorType.c_str()]["protection"].GetInt(), mArmorType);

	//DEBUG ///////////////////
	mArmor = new Armor(10, mArmorType);
	///////////////////////////
}

ArmorPickup::~ArmorPickup()
{
}

void ArmorPickup::save(rapidjson::Value &obj, rapidjson::Value &val, rapidjson::Document::AllocatorType &alloc)
{
	Collectible::save(obj, val, alloc);

	val.SetString(mArmorType.c_str(), alloc);
	obj.AddMember("armor_type", val, alloc);
}

WeaponPickup::WeaponPickup(Vector2D pos, Animation* a, Weapon* weapon)
	:Collectible(pos, a, P_WEAPON)
{
	//mAlternateColor = weapon->mAlternateColor();
	mWeapon = weapon;
	switch(P_WEAPON)
	{
	case W_LANCE:
		break;
	case W_BOW:
		break;
	case W_SWORD:
		break;
	case W_AXE:
		break;
	}
}

WeaponPickup::WeaponPickup(rapidjson::Value & saveData) : Collectible(saveData)
{
	if(mAnimationMap["default"])
	{
	delete mAnimationMap["default"];
	}
	mAnimationMap["default"] = new Animation(*Game::getInstance()->getUnitManager().getUnitAnimations()[mWeaponType + "_pickup_animation"]);
}

WeaponPickup::~WeaponPickup()
{
	/*if(mWeapon)
	{
		delete mWeapon;
	}*/
}
