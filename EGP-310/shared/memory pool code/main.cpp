#include "MemoryPool.h"
#include "defines.h"
#include "UnitManager.h"
#include "Unit.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

struct Tester
{
	int a;
	int b;
	bool c;
	char d;
};

const unsigned int NUM_OBJECTS = 10;

void main()
{
	srand((unsigned int)time(0));
	UnitManager manager(5000);
	Unit* pUnit = manager.createUnit(100, 234, 0, 0, 0.0f, NULL);
	manager.destroyUnit(pUnit);
	manager.destroyUnit(pUnit);

	/*MemoryPool testerPool(NUM_OBJECTS, sizeof(Tester));
	Tester* testers[NUM_OBJECTS];

	//init Tester array
	for (unsigned int i = 0; i < NUM_OBJECTS; i++)
	{
		testers[i] = NULL;
	}

	//allocate tester and place it in random location in tester array
	for (unsigned int i = 0; i < NUM_OBJECTS; i++)
	{
		unsigned int index = rand() % NUM_OBJECTS;

		//allocate a tester
		Tester* ptr = (Tester*)testerPool.allocateObject();

		//if the slot for this index is already taken - free the old one first
		if (testers[index]!=NULL)
		{
			testerPool.freeObject((Byte*)testers[index]);
		}

		//set the indexed tester to the newly allocated one
		testers[index] = ptr;
	}*/

	system("pause");
}