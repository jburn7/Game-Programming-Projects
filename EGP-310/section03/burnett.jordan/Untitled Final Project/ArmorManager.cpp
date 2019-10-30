#include "ArmorManager.h"

std::map<std::string, Armor*> ArmorManager::sArmorMap = {};

void ArmorManager::init()
{
	rapidjson::Document &doc = JSONData::getInstance()->getJSON();
	rapidjson::Value &a = doc["armor"];
	for (auto& armor : a.GetArray())
	{
		//Normally, when you say there's literally no reason something shouldn't work, it's hyperbole; in this case, however there is LITERALLY no reason this shouldn't work.
		//std::string classname = armor["class"].GetString();
		//So have a stupid hack instead:
		std::string classname;
		switch (armor["name"].GetString()[8]) {
		case 'e':
		case 's':
			classname = "chest";
			break;
		case 'l':
		case 'm':
			classname = "helmet";
			break;
		}
		sArmorMap[armor["name"].GetString()] = new Armor(armor["protection"].GetInt(), classname);
		sArmorMap[armor["name"].GetString()]->setColor(new Color(armor["color"]["r"].GetInt(), armor["color"]["g"].GetInt(), armor["color"]["b"].GetInt()));
	}
}

void ArmorManager::cleanup()
{
	for (auto x : sArmorMap)
		delete x.second;
	sArmorMap.clear();
}

Armor* ArmorManager::getArmor(std::string type)
{
	return sArmorMap[type];
}

Armor* ArmorManager::getRandomArmor()
{
	int i = std::rand() % sArmorMap.size();
	for (auto x = sArmorMap.begin(); x != sArmorMap.end(); x++, i--)
		if (i == 0)
			return x->second;
	//Won't ever be reached
	return NULL;
}
