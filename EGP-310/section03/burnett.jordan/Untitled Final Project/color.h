#pragma once
#include "SFML\Graphics\Color.hpp"
#include "Trackable.h"

class GraphicsBuffer;

class Color : public Trackable
{
	friend class GraphicsSystem;
public:
	Color();
	Color(int red, int green, int blue);
	Color(const Color &other);
	~Color();

	int getRed(){ return mColor.r; }
	int getGreen() { return mColor.g; }
	int getBlue() { return mColor.b; }
	int getTransparency() { return mColor.a; }
private:
	sf::Color mColor;
};