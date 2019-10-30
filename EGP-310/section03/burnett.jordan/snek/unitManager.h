#pragma once
#include "unit.h"
#include "EventListener.h"
#include <map>

class UnitManager : public EventListener
{
public:
	UnitManager();
	~UnitManager();

	//handling animations for units
	void setUnitAnimations(std::map<std::string, Animation*> &animations);

	//getters
	std::map<std::string, Animation*> &getUnitAnimations();

	std::vector<Unit*> &getUnits(){ return mUnits; }

	//handling Units
	void addUnit(Unit *unit);

	void clear();

	void deleteUnit(int index);

	Unit *getUnit(int index);

	//game flow functions
	void draw();

	void resetLevel(Vector2D &snakeSpawn); //keeps all the walls, but removes the snake and powerups

	void update(double timeElapsed);

	//event functions
	virtual void handleEvent(const Event& theEvent);

	//setters
	void setLevelEnded(bool torf);

	//cleanup functions
	void cleanup();
private:
	bool mLevelEnded;

	std::vector<Unit*> mUnits;
	std::map<std::string, Animation*> mUnitAnimations;
};