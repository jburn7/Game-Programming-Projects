#pragma once
#include "Collectible.h"
#include <Trackable.h>

class ItemSpawner :public Trackable {
	friend class Collectible;
private:
	Vector2D mPos;
	bool mShouldSpawn;
	float mSpawnProbability;
	PICKUP_TYPE mType;
public:
	ItemSpawner(PICKUP_TYPE type, Vector2D pos);
	~ItemSpawner();
	bool getCanSpawn() const { return mShouldSpawn; };
	float getSpawnProbability() const { return mSpawnProbability; };
	void spawn();
};