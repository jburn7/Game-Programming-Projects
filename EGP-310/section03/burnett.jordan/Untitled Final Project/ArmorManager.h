#pragma once
#include "Armor.h"
#include <map>
#include <string>
#include "jsonData.h"
#include <random>

class ArmorManager : public Trackable
{
private:
	static std::map<std::string, Armor*> sArmorMap;
public:
	static void init();
	static void cleanup();
	static Armor* getArmor(std::string type);
	static Armor* getRandomArmor();
};