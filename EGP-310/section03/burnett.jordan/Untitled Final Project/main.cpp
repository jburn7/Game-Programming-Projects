#include "game.h"
#include "MemoryTracker.h"
#include "PerformanceTracker.h"
#include "jsonData.h"
#include <iomanip>

//weapon spritesheet: http://ffhacktics.com/smf/index.php?topic=6636.0

int main()
{
	srand((unsigned int)time(NULL));
	gpEventSystem = new EventSystem;
	gpPerformanceTracker = new PerformanceTracker;
	Game::initInstance();
	Game::getInstance()->init();
	gpPerformanceTracker->startTracking("total runtime");
	Game::getInstance()->loop();
	gpPerformanceTracker->stopTracking("total runtime");
	Game::getInstance()->cleanup();

	Game::cleanupInstance();

	//calculate percentages of time spent in each section (updating, collision detection, drawing, etc)
	double totalTime = gpPerformanceTracker->getElapsedTime("total runtime");
	double inputPercentage = gpPerformanceTracker->getElapsedTime("input") / totalTime * 100;

	double updatePercentage = gpPerformanceTracker->getElapsedTime("update") / totalTime * 100;
	double unitUpdatePercentage = gpPerformanceTracker->getElapsedTime("unit update") / gpPerformanceTracker->getElapsedTime("update") * 100;
	double collisionUpdatePercentage = gpPerformanceTracker->getElapsedTime("collision update") / gpPerformanceTracker->getElapsedTime("update") * 100;
	double satCollisionPercentage = gpPerformanceTracker->getElapsedTime("SAT") / gpPerformanceTracker->getElapsedTime("collision update") * 100;

	double aabbCollisionPercentage = gpPerformanceTracker->getElapsedTime("total AABB") / gpPerformanceTracker->getElapsedTime("collision update") * 100;
	double myAabbCollisionPercentage = gpPerformanceTracker->getElapsedTime("My AABB") / gpPerformanceTracker->getElapsedTime("total AABB") * 100;
	double sfAabbCollisionPercentage = gpPerformanceTracker->getElapsedTime("SF AABB") / gpPerformanceTracker->getElapsedTime("total AABB") * 100;

	double renderPercentage = gpPerformanceTracker->getElapsedTime("render") / totalTime * 100;
	double sleepPercentage = gpPerformanceTracker->getElapsedTime("sleep") / totalTime * 100;

	cout << fixed << setprecision(1) 
		 << "\nInput Percentage: " << inputPercentage 
		 << "\nUpdate Percentage: " << updatePercentage
		 << "\n\tUnit update percentage: " << unitUpdatePercentage
		 << "\n\tCollision update percentage: " << collisionUpdatePercentage
		 << "\n\t\tSAT collision percentage: " << satCollisionPercentage
		 << "\n\t\tTotal AABB Percentage: " << aabbCollisionPercentage
		 << "\n\t\t\tMy AABB collision percentage: " << myAabbCollisionPercentage
		 << "\n\t\t\tSF AABB collision percentage: " << sfAabbCollisionPercentage
		 << "\nRender Percentage: " << renderPercentage 
		 << "\nSleep percentage: " << sleepPercentage << endl << endl;

	delete gpEventSystem;
	delete gpPerformanceTracker;

	gMemoryTracker.reportAllocations(std::cout);
	system("pause");

	return 0;
}