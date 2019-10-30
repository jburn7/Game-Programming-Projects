#include "game.h"
#include "MemoryTracker.h"
#include "jsonData.h"

int main()
{
	srand(time(NULL));
	gpEventSystem = new EventSystem;
	Game::initInstance();
	Game::getInstance()->init();
	Game::getInstance()->loop();
	Game::getInstance()->cleanup();

	Game::cleanupInstance();

	delete gpEventSystem;

	gMemoryTracker.reportAllocations(std::cout);
	system("pause");

	return 0;
}