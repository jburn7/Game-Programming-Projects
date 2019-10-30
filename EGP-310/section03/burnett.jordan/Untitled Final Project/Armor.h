#pragma once
#include <string>
#include "Trackable.h"
#include "Animation.h"

class Armor : public Trackable
{
	friend class ArmorPickup;
private:
	int mProtection;
	std::string mType;
	Animation* mGuiIcon;
	Color* mColor;
public:
	Armor(int protection, std::string type);
	~Armor();
	int getProtection() const { return mProtection; };
	std::string getType() const { return mType; };
	void loadArmorJSON(const std::string &armorType);
	void setColor(Color* color);
	void draw(int slot);
};