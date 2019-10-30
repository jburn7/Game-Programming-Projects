#pragma once
#include "defines.h"
#include <set>

class Unit;
class MemoryPool;
class Sprite;

class UnitManager
{
public:
	UnitManager(Uint32 capacity = 8);
	~UnitManager();

	Unit* createUnit(Uint32 x, Uint32 y, Uint32 velX, Uint32 velY, float rotation, Sprite* pSprite);
	void destroyUnit(Unit*& pUnit);

private:
	MemoryPool* mpPool;
	std::set<Unit*> mpUnits;
	Uint32 mCapacity;
};

