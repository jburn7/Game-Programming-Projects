#pragma once
#include "SFML\Graphics\ConvexShape.hpp"
#include "Vector2D.h"

//used to abstract the outline of the Units (i.e. the sf::ConvexShape)
class HitBox
{
	friend class CollisionComponent;
public:
	HitBox();
	//HitBox(std::vector<Vector2D> &points, Vector2D &pos);
	~HitBox();

	//defined in consecutive order, cw or ccw
	void create(std::vector<Vector2D> &points, Vector2D &pos);

	//DEBUG used to see hitbox locations /////
	void draw();
	//////////////////////////////////////////

	//-1 for left, 1 for right
	void flipX(int dir);

	void move(const Vector2D &vel);

	void rotate(float degrees);

	//setters
	void setPosition(const Vector2D &pos);

	//getters
	float getLeftBounds(){ return mOutline.getGlobalBounds().left; }
	float getTopBounds() { return mOutline.getGlobalBounds().top; }
	float getHeight() { return mOutline.getGlobalBounds().height; }
	float getWidth(){ return mOutline.getGlobalBounds().width; }
private:	//DEBUG ///////////////////////////
	sf::ConvexShape mOutline;
};