#include "UnitManager.h"
#include "Unit.h"
#include "MemoryPool.h"

UnitManager::UnitManager(Uint32 capacity /*= 8*/) :mCapacity(capacity)
{
	mpPool = new MemoryPool(capacity, sizeof(Unit));
}


UnitManager::~UnitManager()
{
	delete mpPool;
}

Unit* UnitManager::createUnit(Uint32 x, Uint32 y, Uint32 velX, Uint32 velY, float rotation, Sprite* pSprite)
{
	Byte* ptr = mpPool->allocateObject();
	if (ptr != NULL)
	{
		//create unit using placement new syntax
		Unit* pUnit = new (ptr)Unit(x,y);
		mpUnits.insert(pUnit);
		return pUnit;
	}
	else
	{
		return NULL;
	}
}

void UnitManager::destroyUnit(Unit*& pUnit)
{
	auto it = mpUnits.find(pUnit);
	if (it != mpUnits.end())
	{
		mpUnits.erase(it);
		mpPool->freeObject((Byte*)pUnit);
		pUnit = NULL;
	}
}
