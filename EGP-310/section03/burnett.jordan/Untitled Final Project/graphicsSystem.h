#pragma once
#include "SFML\Graphics\RenderWindow.hpp"
#include "SFML\Graphics\Sprite.hpp"
#include "SFML\Graphics\Text.hpp"
//DEBUG erase when done with draw(convexshape&) ////////
#include "SFML\Graphics\ConvexShape.hpp"
////////////////////////////////////////////////////////
#include "sprite.h"
#include "font.h"
#include "jsonData.h"
#include "EventListener.h"
#include "EventSystem.h"
#include "movePlayerEvent.h"
#include <string>

class GraphicsSystem : public EventListener
{
	friend class InputSystem; //for polling RenderWindow Events
public:
	GraphicsSystem();

	~GraphicsSystem();

	void init(float w, float h);

	void cleanup();

	void clear();

	//DEBUG used to see hitbox locations//////
	void draw(const sf::ConvexShape &shape);
	//////////////////////////////////////////

	void draw(const Vector2D &targetLoc, Sprite &sprite);

	void draw(const Vector2D &targetLoc, Sprite &sprite, float theta);

	void draw(const Vector2D &targetLoc, Sprite &sprite, const Color &color);

	//draws an object in reverse orientation related to the scale provided (repositions object). Negative scale = reverse direction.
	void drawReverse(const Vector2D &targetLoc, Sprite &sprite, const Vector2D &scale);

	//draws an object reflected over axes related to the scale provided (does NOT reposition object). Negative scale = reverse direction. can also be used to draw scaled objects
	void drawReflect(const Vector2D &targetLoc, Sprite &sprite, const Vector2D &scale, float theta = 0.f);


	virtual void handleEvent(const Event &theEvent);


	void flip();

	//just updates the camera position, does NOT draw
	void update();


	void writeText(const Vector2D &targetLoc, Font &font, Color &color, std::string &message);

	//setters and getters
	void setHeight(float h);

	void setWidth(float w);

	//returns absolute center coordinates, that is the relative center plus camera pan
	Vector2D getTopLeft();

	float getHeight();

	float getWidth();
private:
	float mWidth, mHeight;
	Vector2D mCameraPosition;
	Vector2D mTopLeft;

	sf::RenderWindow mDisplay;
};