#pragma once
#include "inputComponent.h"
#include "Entity.h"

class MeleeEntityInputComponent :public InputComponent
{
private:
	bool mMovingLeft, mWallLeft, mWallRight, mStanding, mPlayerInRadius;
	//TODO: add functionality for these two
	float mDetectionRadius;
	Vector2D mAnchorPos;
	Vector2D *mPlayerPos;
public:
	MeleeEntityInputComponent(Vector2D *playerPos);
	virtual ~MeleeEntityInputComponent();
	//virtual void save(rapidjson::Value &obj, rapidjson::Value &val, rapidjson::Document::AllocatorType &alloc);
	virtual void setDetectionRadius(float radius);
	void update(Entity* u);
};