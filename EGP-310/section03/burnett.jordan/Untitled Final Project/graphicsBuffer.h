#pragma once
#include "SFML\Graphics\Texture.hpp"
#include <string>
#include "color.h"

class GraphicsBuffer : public Trackable
{
	friend class Color;
	friend class GraphicsSystem;
public:
	GraphicsBuffer();

	GraphicsBuffer(Color &color, float w, float h);

	GraphicsBuffer(std::string &resource);

	~GraphicsBuffer();

	void create(float w, float h);

	void setHeight(float h);

	void setWidth(float w);

	float getHeight();

	float getWidth();
private:
	float mWidth, mHeight;

	sf::Texture mBitmap;
};