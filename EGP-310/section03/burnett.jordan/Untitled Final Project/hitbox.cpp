#include "hitBox.h"

//DEBUG //////////////
#include "game.h"
//////////////////////

HitBox::HitBox()
{
	//DEBUG used to see hitbox locations //////////
	mOutline.setFillColor(sf::Color::Blue);
	mOutline.setOutlineThickness(-2);
	mOutline.setOutlineColor(sf::Color::Red);
	mOutline.setFillColor(sf::Color(mOutline.getFillColor().r, mOutline.getFillColor().g, mOutline.getFillColor().b, 125));
	///////////////////////////////////////////////
}

HitBox::~HitBox()
{
}

void HitBox::create(std::vector<Vector2D> &points, Vector2D &pos)
{
	mOutline.setPointCount(points.size());
	for(unsigned int i = 0; i < points.size(); i++)
	{
		mOutline.setPoint(i, sf::Vector2f(points[i].getX(), points[i].getY()));
	}

	setPosition(pos);
}

void HitBox::draw()
{
	Game::getInstance()->getGraphics().draw(mOutline);
}

void HitBox::flipX(int dir)
{
}

void HitBox::move(const Vector2D &vel)
{
	for(unsigned int i = 0; i < mOutline.getPointCount(); i++)
	{
		mOutline.setPoint(i, sf::Vector2f(mOutline.getPoint(i).x + vel.getX(), mOutline.getPoint(i).y + vel.getY()));
	}
}

void HitBox::rotate(float degrees)
{
	mOutline.rotate(degrees);
}

void HitBox::setPosition(const Vector2D &pos)
{
	mOutline.setPosition(pos.getX(), pos.getY());
	/*for(unsigned int i = 0; i < mOutline.getPointCount(); i++)
	{
		mOutline.setPoint(i, sf::Vector2f(mOutline.getPoint(i).x + pos.getX(), mOutline.getPoint(i).y + pos.getY()));
	}*/
}
