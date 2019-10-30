#include "Entity.h"
#include "wall.h"
#include "game.h"
#include "Collectible.h"

bool Entity::msInit = false;
std::map<EntityType, std::string> Entity::msEntityTypeMap = {};

Entity::Entity(Vector2D pos, InputComponent *input, const std::string &entityType) : Unit(pos)
{
	if(!msInit)
	{
		msInit = true;
		msEntityTypeMap[E_MELEE_ENEMY] = "melee enemy";
		msEntityTypeMap[E_BOW_ENEMY] = "bow enemy";
	}

	mType = U_ENTITY;
	mHitRecovery = false;
	mIsOnGround = false;
	mIsPlayer = false;
	mTimer.start();
	mInput = input;

	loadEntityJSON(entityType);

	if(entityType == "melee enemy")
	{
		mEntityType = E_MELEE_ENEMY;
	}
	else if(entityType == "bow enemy")
	{
		mEntityType = E_BOW_ENEMY;
	}

	mCurrentAnimation = "standing";

	loadHitbox();

	mPreviousPosition = mPosition;

	std::vector<Vector2D> feetBox;
	feetBox.push_back(Vector2D(mPosition.getX(), mPosition.getY() + getHeight()));
	feetBox.push_back(Vector2D(mPosition.getX(), mPosition.getY() + getHeight() + 1));
	feetBox.push_back(Vector2D(mPosition.getX() + getWidth(), mPosition.getY() + getHeight() + 1));
	feetBox.push_back(Vector2D(mPosition.getX() + getWidth(), mPosition.getY() + getHeight()));
	mFeetBox.create(feetBox, Vector2D(0, 0));

	if(dynamic_cast<PlayerInputComponent*>(input))
	{
		mIsPlayer = true;
		rapidjson::Value &val = JSONData::getInstance()->getJSON()["player"];
		setColor(Color(val["color"]["r"].GetInt(), val["color"]["g"].GetInt(), val["color"]["b"].GetInt()));
		mHealth = mMaxHealth = val["health"].GetFloat();
		mMaxHorizVelocity = val["maxHorizontalVelocity"].GetFloat();
		if(mpWeapon)
		{
			delete mpWeapon;
		}
		mpWeapon = new Weapon(WeaponType(val["weapon"]["weapon_type"].GetInt()), val["weapon"]["weapon_scale"].GetInt());
	}
}

Entity::Entity(rapidjson::Value &saveData) : Unit(saveData)
{
	mEntityType = EntityType(saveData["entity_type"].GetInt());
	std::string entityType = "";
	switch(mEntityType)
	{
	case E_MELEE_ENEMY:
		entityType = "melee enemy";
		break;
	case E_BOW_ENEMY:
		entityType = "bow enemy";
		break;
	}

	switch(saveData["input_type"].GetInt())
	{
	case IN_PLAYER:
		mInput = Game::getInstance()->getInputManager().getNewPlayerInputComponent();
		break;
	case IN_MELEE_ENEMY:
		mInput = Game::getInstance()->getInputManager().getNewMeleeEntityInputComponent();
		break;
	case IN_BOW_ENEMY:
		mInput = Game::getInstance()->getInputManager().getNewProjectileEntityInputComponent();
		break;
	}
	mInput->load(saveData);

	loadEntityJSON(entityType);

	mVelocity = Vector2D(saveData["velocityX"].GetFloat(), saveData["velocityY"].GetFloat());
	mIsOnGround = saveData["is_on_ground"].GetBool();
	mHealth = saveData["remaining_health"].GetFloat();
	mMaxHealth = saveData["max_health"].GetFloat();
	if(mpWeapon)
	{
		delete mpWeapon;
	}
	mpWeapon = new Weapon(WeaponType(saveData["weapon"]["weapon_type"].GetInt()), saveData["weapon"]["weapon_scale"].GetInt());
	//load powerups

	mHitRecovery = saveData["hit_recovery"].GetBool();

	mIsPlayer = saveData["is_player"].GetBool();

	/*for(auto &a : JSONData::getInstance()->getJSON()[entityType.c_str()]["animations"].GetArray())
	{
		mAnimationMap.insert(std::pair<std::string, Animation*>(a["description"].GetString(), new Animation(*Game::getInstance()->getUnitManager().getUnitAnimations()[a["animationName"].GetString()])));
	}*/

	for(auto &a : saveData["active_powerups"].GetArray())
	{
		std::string name = a["name"].GetString();
		mPowerupRemaining[name] = { a["time_remaining"].GetInt(), a["power"].GetFloat() };
		if(name == "speedboost")
		{
			mMaxHorizVelocity *= mPowerupRemaining["speedboost"].second;
		}
		else if(name == "jumpboost")
		{
			mJumpVelocity *= a["power"].GetFloat();
		}
	}

	loadHitbox();

	std::vector<Vector2D> feetBox;
	feetBox.push_back(Vector2D(mPosition.getX(), mPosition.getY() + getHeight()));
	feetBox.push_back(Vector2D(mPosition.getX(), mPosition.getY() + getHeight() + 1));
	feetBox.push_back(Vector2D(mPosition.getX() + getWidth(), mPosition.getY() + getHeight() + 1));
	feetBox.push_back(Vector2D(mPosition.getX() + getWidth(), mPosition.getY() + getHeight()));
	mFeetBox.create(feetBox, Vector2D(0, 0));

	/*rapidjson::Value &doc = JSONData::getInstance()->getJSON();

	mpHealthBar = new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer(doc[entityType.c_str()]["health_bar"].GetString()));
	mpHealthBarOutline = new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer(doc[entityType.c_str()]["health_bar_outline"].GetString()));*/

	if(mAlternateColor)
	{
		setColor(*mAlternateColor);
	}
}

Entity::~Entity()
{
	if(mInput)
	{
		delete mInput;
		mInput = NULL;
	}

	if(mpWeapon)
	{
		delete mpWeapon;
		mpWeapon = NULL;
	}

	if(mpHealthBar)
	{
		delete mpHealthBar;
		mpHealthBar = NULL;
	}

	if(mpHealthBarOutline)
	{
		delete mpHealthBarOutline;
		mpHealthBarOutline = NULL;
	}

	mTimer.stop();

	for(auto x : mAnimationMap)
		delete x.second;
	mAnimationMap.clear();

	/*for (auto a : mArmor)
	{
		delete a.second;
	}
	mArmor.clear();*/

	if(mAlternateColor)
	{
		delete mAlternateColor;
		mAlternateColor = NULL;
	}

	//Unit::cleanup();
}

void Entity::draw()
{
	//mHitbox.draw();
	Unit::draw();

	Game::getInstance()->getGraphics().drawReverse(Vector2D(mPosition.getX(), mPosition.getY() - 14), *mpHealthBar, Vector2D((float)mHealth / (float)mMaxHealth, 1)); //need to modify for healthbar percentage
	Game::getInstance()->getGraphics().draw(Vector2D(mPosition.getX(), mPosition.getY() - 14), *mpHealthBarOutline);

	int slot = 0;
	for (auto& x : mArmor) {
		x.second->draw(slot++);
	}
}

void Entity::save(rapidjson::Value &obj, rapidjson::Value &val, rapidjson::Document::AllocatorType &alloc)
{
	Unit::save(obj, val, alloc);

	obj.AddMember("entity_type", mEntityType, alloc);

	mInput->save(obj, val, alloc);

	//things to save:
	//velocity
	obj.AddMember("velocityX", mVelocity.getX(), alloc);
	obj.AddMember("velocityY", mVelocity.getY(), alloc);
	//is on ground
	val.SetBool(mIsOnGround);
	obj.AddMember("is_on_ground", val, alloc);
	//health
	obj.AddMember("remaining_health", mHealth, alloc);
	obj.AddMember("max_health", mMaxHealth, alloc);
	//weapon type
	mpWeapon->save(obj, val, alloc);
	//powerups remaining
	rapidjson::Value arr(rapidjson::kArrayType);
	for(auto &a : mPowerupRemaining)
	{
		rapidjson::Value temp(rapidjson::kObjectType);
		val.SetString(a.first.c_str(), alloc);
		temp.AddMember("name", val, alloc);
		temp.AddMember("time_remaining", a.second.first, alloc);
		temp.AddMember("power", a.second.second, alloc);
		arr.PushBack(temp, alloc);
	}
	obj.AddMember("active_powerups", arr, alloc);
	//hit recovery
	val.SetBool(mHitRecovery);
	obj.AddMember("hit_recovery", val, alloc);

	val.SetBool(mIsPlayer);
	obj.AddMember("is_player", val, alloc);
}

void Entity::update(int timeElapsed)
{
	mItemPickup = false;
	mInput->update(this);
	if (mAttack && mpWeapon)
		entityAttack(timeElapsed);
	mFacingLeft = (mMoveLeft || mMoveRight) ? mMoveLeft : mFacingLeft;

	for(auto x = mPowerupRemaining.begin(); x != mPowerupRemaining.end();)
	{
		if(x->second.first < 0)
		{
			if(x->first == "speedboost")
				mMaxHorizVelocity /= x->second.second;
			if(x->first == "jumpboost")
				mJumpVelocity /= x->second.second;
			mPowerupRemaining.erase(x++);
		}
		else
		{
			x++;
		}
	}

	mPhysics.update(this);

	if(mIsPlayer)
	{
		gpEventSystem->fireEvent(new MovePlayerEvent(MOVE_PLAYER_EVENT, Vector2D(mPosition.getX() + getWidth() / 2, mPosition.getY() + getHeight() / 2)));
	}

	if(mVelocity.getX() != 0)
	{
		mCurrentAnimation = "walking";
	}
	else
	{
		mCurrentAnimation = "standing";
	}

	mCollided = false;

	if(mHitRecovery)
	{
		if(mTimer.getElapsedTime() > mHitRegenTime)
		{
			for(auto &a : mAnimationMap)
			{
				a.second->setTransparency(1);
			}
			mHitRecovery = false;
		}
	}

	Unit::update(timeElapsed);

	//if animation == any walking animation && animation current frame == frame with foot hitting floor
	    //play appropriate footstep sound

	/*
		not sure where to play animation-based sounds like footsteps
		if I play them here, then I need to know more about the animation such as when it switches to a particular frame
		and if I play them inside the animation update, then the animation needs to know what it is in order to play the right sounds

		option 3: animations contain maps from keyframes to sound strings to be played. gg ez.
	*/
	//DEBUG ////////////////////////////////////
	if(mAnimationMap[mCurrentAnimation]->getCurrentFrameNumber() == 0)
	{
		//std::cout << "playing sound\n";
		//gpEventSystem->fireEvent(PlaySoundEvent(PLAY_SOUND_EVENT, mSoundMap["footstep_1"]));
	}
	////////////////////////////////////////////
	for (auto &x : mPowerupRemaining) {
		if (x.second.first > 0)
			x.second.first -= timeElapsed;
	}

	mIsOnGround = false;

	if(mPosition.getY() > mMaxYValue)
	{
		die();
	}
}

void Entity::update(int timeElapsed, std::vector<std::vector<bool>>& wallMap)
{
	mItemPickup = false;
	mInput->update(this);
	if(mAttack && mpWeapon)
		entityAttack(timeElapsed);
	mFacingLeft = (mMoveLeft || mMoveRight) ? mMoveLeft : mFacingLeft;

	for(auto x = mPowerupRemaining.begin(); x != mPowerupRemaining.end();)
	{
		if(x->second.first < 0)
		{
			if(x->first == "speedboost")
				mMaxHorizVelocity /= x->second.second;
			if(x->first == "jumpboost")
				mJumpVelocity /= x->second.second;
			mPowerupRemaining.erase(x++);
		}
		else
		{
			x++;
		}
	}

	mPhysics.update(this, wallMap);

	if(mIsPlayer)
	{
		gpEventSystem->fireEvent(new MovePlayerEvent(MOVE_PLAYER_EVENT, Vector2D(mPosition.getX() + getWidth() / 2, mPosition.getY() + getHeight() / 2)));
	}

	if(mVelocity.getX() != 0)
	{
		mCurrentAnimation = "walking";
	}
	else
	{
		mCurrentAnimation = "standing";
	}

	mCollided = false;

	if(mHitRecovery)
	{
		if(mTimer.getElapsedTime() > mHitRegenTime)
		{
			for(auto &a : mAnimationMap)
			{
				a.second->setTransparency(1);
			}
			mHitRecovery = false;
		}
	}

	Unit::update(timeElapsed);

	//if animation == any walking animation && animation current frame == frame with foot hitting floor
	//play appropriate footstep sound

	/*
	not sure where to play animation-based sounds like footsteps
	if I play them here, then I need to know more about the animation such as when it switches to a particular frame
	and if I play them inside the animation update, then the animation needs to know what it is in order to play the right sounds

	option 3: animations contain maps from keyframes to sound strings to be played. gg ez.
	*/
	//DEBUG ////////////////////////////////////
	if(mAnimationMap[mCurrentAnimation]->getCurrentFrameNumber() == 0)
	{
		//std::cout << "playing sound\n";
		//gpEventSystem->fireEvent(PlaySoundEvent(PLAY_SOUND_EVENT, mSoundMap["footstep_1"]));
	}
	////////////////////////////////////////////
	for(auto &x : mPowerupRemaining)
	{
		if(x.second.first > 0)
			x.second.first -= timeElapsed;
	}

	//mIsOnGround = false;

	if(mPosition.getY() > mMaxYValue)
	{
		die();
	}
}

void Entity::takeDamage(float damage)
{
	if(!mHitRecovery)
	{
		if(mTimer.getElapsedTime() > mTimeLastHit + mHitRegenTime)
		{
			for (auto x : mArmor) {
				if (x.second)
					damage -= x.second->getProtection();
			}
			if (damage > 0)
				mHealth -= damage;
			if(mHealth <= 0)
			{
				die();
			}
			mHitRecovery = true;
			for(auto &a : mAnimationMap)
			{
				a.second->setTransparency(.5);
			}
			mTimer.start();
		}
	}
}

float Entity::getCollisionDamageValue()
{
	//called when the player collides with an enemy; return the enemy's base damage
	return mDamageVal;
}

void Entity::entityAttack(int timeElapsed)
{
	if(mpWeapon->isWeaponReady(timeElapsed))
	{
		if(mInput->getTargetAttackLoc())
		{
			if(mFacingLeft)
			{
				mpWeapon->generateAttack(timeElapsed, 10, getCenter(), mHandRight, -1, this, *mInput->getTargetAttackLoc());
			}
			else
			{
				mpWeapon->generateAttack(timeElapsed, 10, getCenter(), mHandRight, 1, this, *mInput->getTargetAttackLoc());
			}
		}
		else
		{
			if(mFacingLeft)
			{
				mpWeapon->generateAttack(timeElapsed, 10, getCenter(), mHandRight, -1, this);
			}
			else
			{
				mpWeapon->generateAttack(timeElapsed, 10, getCenter(), mHandRight, 1, this);
			}
		}
	}
}

void Entity::setPosition(const Vector2D &pos)
{
	Unit::setPosition(pos);
	mFeetBox.setPosition(pos);
}

//returns absolute hand coordinates, not relative ones
Vector2D Entity::getHandCoord()
{
	if(!mFacingLeft)
	{
		return getCenter() + mHandRight;
	}
	else
	{
		return getCenter() + Vector2D(mHandRight.getX() * -1, mHandRight.getY());
	}
}

void Entity::collide(Unit *g)
{
	if(g != this)
		g->collide(this);
}

void Entity::collide(Wall *o)
{	
	mCollision.resolveCollision(this, o);
}

void Entity::collide(Attack *g)
{
	mCollision.resolveCollision(this, g);
}

void Entity::die()
{
	gpEventSystem->fireEvent(new PlaySoundEvent(PLAY_SOUND_EVENT, mSoundMap["death"]));
	gpEventSystem->fireEvent(new UnitRemoveEvent(UNIT_REMOVE_EVENT, this));
	if(mIsPlayer)
	{
		gpEventSystem->fireEvent(new PlayerDeathEvent(PLAYER_DEATH_EVENT));
	}
}

void Entity::modifyDifficulty(float multiplier)
{
	if(!mIsPlayer)
	{
		//load up all difficulty-dependent members from JSON again and then multiply it
		rapidjson::Value &doc = JSONData::getInstance()->getJSON()[msEntityTypeMap[mEntityType].c_str()];
		float healthRatio = mHealth / mMaxHealth;
		mMaxHealth = mHealth = doc["health"].GetFloat() * multiplier;
		mHealth = mMaxHealth * healthRatio;
		mDamageVal = doc["damageVal"].GetFloat() * multiplier;
	}
}

void Entity::move(Vector2D &vel)
{
	mPreviousPosition = mPosition;
	mPosition += vel;
	mHitbox.move(vel);
	mFeetBox.move(vel);
	/*mpInput->UpdatePos(shape.getPosition());
	moved = true;*/
}

void Entity::loadEntityJSON(const std::string &entityType)
{
	rapidjson::Value &doc = JSONData::getInstance()->getJSON();

	mWidth = doc[entityType.c_str()]["width"].GetInt();
	mHeight = doc[entityType.c_str()]["height"].GetInt();

	//whenever I integrate prototyping into the JSON data, the code will look as follows:
	/*
	if(doc[entityType.c_str()]["someVal"].asVal() == validValue){
	value = [above code];
	}
	else{
	std::string prototype = doc[entityType.c_str()]["prototype"].asStr();
	value = doc[protoType]["someVal"].asVal(); //can make this a recursive function too allow as many nested prototypes as I'd like
	}
	*/

	mMaxHealth = mHealth = doc[entityType.c_str()]["health"].GetFloat();
	mDamageVal = doc[entityType.c_str()]["damageVal"].GetFloat();
	mHitRegenTime = doc[entityType.c_str()]["hitRegenTime"].GetInt();
	mMaxHorizVelocity = doc[entityType.c_str()]["maxHorizontalVelocity"].GetFloat();
	mMaxVerticalVelocity = doc[entityType.c_str()]["maxVerticalVelocity"].GetFloat();
	mJumpVelocity = doc[entityType.c_str()]["jumpVelocity"].GetFloat();
	mHandRight = Vector2D(doc[entityType.c_str()]["handX"].GetFloat(), doc[entityType.c_str()]["handY"].GetFloat()); //relative to center of Entity
	mpWeapon = new Weapon(WeaponType(doc[entityType.c_str()]["weapon"]["weapon_type"].GetInt()), doc[entityType.c_str()]["weapon"]["weapon_scale"].GetInt());

	for(auto &a : doc[entityType.c_str()]["animations"].GetArray())
	{
		mAnimationMap.insert(std::pair<std::string, Animation*>(a["description"].GetString(), new Animation(*Game::getInstance()->getUnitManager().getUnitAnimations()[a["animationName"].GetString()])));
	}
	for(auto &a : doc[entityType.c_str()]["sounds"].GetArray())
	{
		mSoundMap.insert(std::pair<std::string, std::string>(a["description"].GetString(), a["soundBufferName"].GetString()));
	}

	mInput->setDetectionRadius(doc[entityType.c_str()]["detectionRadius"].GetFloat());

	std::string temp = "level" + to_string(Game::getInstance()->getLevelManager().getCurrentLevelNumber() + 1);
	mMaxYValue = doc[temp.c_str()]["maxYVal"].GetInt();

	mpHealthBar = new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer(doc[entityType.c_str()]["health_bar"].GetString()));
	mpHealthBarOutline = new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer(doc[entityType.c_str()]["health_bar_outline"].GetString()));
}

void Entity::applyCollectible(Collectible *c)
{
	switch (c->getPickupType()) {
	case P_SPEED:
		if(mPowerupRemaining["speedboost"].first <= 0)
			mMaxHorizVelocity *= c->getPower();
		mPowerupRemaining["speedboost"] = { 1000000 * c->getDuration(), c->getPower() };
		break;
	case P_JUMP:
		if(mPowerupRemaining["jumpboost"].first <= 0)
			mJumpVelocity *= c->getPower();
		mPowerupRemaining["jumpboost"] = { 1000000 * c->getDuration(), c->getPower() };
		break;
	case P_ARMOR:
		mArmor[static_cast<ArmorPickup*>(c)->getArmor()->getType()] = static_cast<ArmorPickup*>(c)->getArmor();
		break;
	case P_WEAPON:
		if(mpWeapon != static_cast<WeaponPickup*>(c)->getWeapon())
		{
			//lete mpWeapon;
		}
		mpWeapon = static_cast<WeaponPickup*>(c)->getWeapon();
		break;
	case P_VICTORY_ITEM:
		gpEventSystem->fireEvent(new VictoryItemFoundEvent(VICTORY_ITEM_FOUND_EVENT));
		break;
	}
	c->playCollectedSound();
}
