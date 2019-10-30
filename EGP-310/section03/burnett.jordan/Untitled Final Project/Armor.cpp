#include "Armor.h"
#include "game.h"

Armor::Armor(int protection, std::string type)
{
	mProtection = protection;
	mType = type;
	mGuiIcon = new Animation(*Game::getInstance()->getUnitManager().getUnitAnimations()[mType + "_pickup_animation"]);
}

Armor::~Armor() {
	delete mColor;
	mGuiIcon->cleanup();
	delete mGuiIcon;
}

void Armor::loadArmorJSON(const std::string &armorType)
{

}

void Armor::setColor(Color* color)
{
	mColor = color;
	mGuiIcon->setColor(color);
}

void Armor::draw(int slot)
{
	GraphicsSystem &graphics = Game::getInstance()->getGraphics();
	graphics.draw(graphics.getTopLeft() + Vector2D(10 + (float)slot * 50, 50), *(mGuiIcon->getCurrentSprite()));
}
