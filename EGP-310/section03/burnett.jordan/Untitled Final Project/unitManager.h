#pragma once
#include "unit.h"
#include "EventListener.h"
#include "PerformanceTracker.h"
#include "difficultyChangeEvent.h"
#include <map>
#include <unordered_set>

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
	std::vector<std::vector<bool>>* getWallMask() { return &mWallMask; };
	std::vector<std::vector<Wall*>> &getWallMaskPointers() { return mWallMaskPointers; }

	//handling Units
	void addUnit(Unit *unit);

	void clear();

	void deleteUnit(int index);

	Unit *getUnit(int index);

	//game flow functions
	void draw();

	void load(rapidjson::Document &doc);

	void resetLevel(Vector2D &snakeSpawn); //keeps all the walls, but removes the snake and powerups

	void save(rapidjson::Document &doc, rapidjson::Document::AllocatorType &alloc);

	void update(int timeElapsed);

	//event functions
	virtual void handleEvent(const Event& theEvent);


	bool queryWall(int x, int y);


	//setters
	void setLevelEnded(bool torf);
	void setWallMask(const std::vector<std::vector<bool>> &wallMask, std::vector<Wall*> &walls);

	//cleanup functions
	void cleanup();
private:
	bool mLevelEnded;

	std::vector<Unit*> mUnits;
	std::vector<Unit*> mMovingUnits; //only holds pointers to units in mUnits
	std::vector<std::vector<bool>> mWallMask;
	std::vector<std::vector<Wall*>> mWallMaskPointers;
	std::map<std::string, Animation*> mUnitAnimations;
};