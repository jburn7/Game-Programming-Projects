#pragma once
#include "unit.h"

enum InputType
{
	IN_BOW_ENEMY,
	IN_MELEE_ENEMY,
	IN_PLAYER
};

class InputComponent
{
protected:
	InputType mInputType;
	//std::vector<Unit*>* mUnits;
	Vector2D mTargetAttackLoc;
	std::vector<std::vector<bool>>* mWallMask;
public:
	InputComponent();
	virtual ~InputComponent();

	virtual Vector2D *getTargetAttackLoc() { return NULL; }

	virtual void setDetectionRadius(float radius) {}

	virtual void load(rapidjson::Value &saveData);
	virtual void save(rapidjson::Value &obj, rapidjson::Value &val, rapidjson::Document::AllocatorType &alloc);

	int getWallWidth(int &x, int &y);
	bool queryWall(int x, int y);

	virtual void update(Entity *u) = 0;

	/*
		each object will own a pointer to this class or have a NULL pointer
		if the pointer is valid, then th eobject will call update
		this will most likely result in some sort of AI algorithm, but the 
		PlayerInputComponent will act based off of PlayerInputEvents
		the goal is simply to modify booleans in the calling object, i.e. the move left/right/jump/attack booleans in Entity
	*/
};