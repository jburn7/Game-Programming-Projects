#pragma once
#include "inputComponent.h"
#include "Entity.h"

class ProjectileEntityInputComponent : public InputComponent
{
public:
	ProjectileEntityInputComponent(Vector2D *playerPos);
	~ProjectileEntityInputComponent();

	virtual void update(Entity *e);

	virtual void setDetectionRadius(float radius);

	virtual Vector2D *getTargetAttackLoc() { return &mTargetAttackLoc; }
private:
	bool mMovingLeft;
	float mDetectionRadius;
	Vector2D *mPlayerPos;
};