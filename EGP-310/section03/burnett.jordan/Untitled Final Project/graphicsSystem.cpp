#include "graphicsSystem.h"

void setPosition(sf::Transformable &trans, const Vector2D &vec)
{
	trans.setPosition(sf::Vector2f(vec.getX(), vec.getY()));
}

GraphicsSystem::GraphicsSystem()
{
	mWidth = 0;
	mHeight = 0;

	gpEventSystem->addListener(MOVE_PLAYER_EVENT, this);
}

GraphicsSystem::~GraphicsSystem()
{
	cleanup();
}

void GraphicsSystem::init(float w, float h)
{
	std::string title(JSONData::getInstance()->getJSON()["game"]["title"].GetString());
	mDisplay.create(sf::VideoMode((unsigned int)w, (unsigned int)h, 32), sf::String(title));
	mWidth = w;
	mHeight = h;
}

void GraphicsSystem::cleanup()
{
	mDisplay.close();
}

void GraphicsSystem::clear()
{
	mDisplay.clear(sf::Color::Black);
}

void GraphicsSystem::draw(const sf::ConvexShape &shape)
{
	mDisplay.draw(shape);
}

void GraphicsSystem::draw(const Vector2D &targetLoc, Sprite &sprite)
{
	sf::Sprite temp(sprite.getTexture()->mBitmap, sf::IntRect((int)sprite.getSourceLoc().getX(), (int)sprite.getSourceLoc().getY(), (int)sprite.getWidth(), (int)sprite.getHeight()));
	temp.setPosition(sf::Vector2f(targetLoc.getX(), targetLoc.getY()));
	if(sprite.getColor())
	{
		temp.setColor(sf::Color(sprite.getColor()->mColor));
	}
	temp.setColor(sf::Color(temp.getColor().r, temp.getColor().g, temp.getColor().b, (sf::Uint8)sprite.getTransparency()));
	mDisplay.draw(temp);
}

void GraphicsSystem::draw(const Vector2D &targetLoc, Sprite &sprite, float theta)
{
	sf::Sprite temp(sprite.getTexture()->mBitmap, sf::IntRect((int)sprite.getSourceLoc().getX(), (int)sprite.getSourceLoc().getY(), (int)sprite.getWidth(), (int)sprite.getHeight()));
	float degrees = theta;/* * 180.0f / 3.1415926f;*/
	const Vector2D offset(targetLoc.getX() + sprite.getWidth() / 2 , targetLoc.getY() + sprite.getHeight() / 2);
	setPosition(temp, offset);
	temp.setOrigin((sprite.getWidth()) / 2, (sprite.getHeight()) / 2);
	temp.rotate(degrees);
	if(sprite.getColor())
	{
		temp.setColor(sf::Color(sprite.getColor()->mColor));
	}
	temp.setColor(sf::Color(temp.getColor().r, temp.getColor().g, temp.getColor().b, (sf::Uint8)sprite.getTransparency()));
	mDisplay.draw(temp);
}

void GraphicsSystem::draw(const Vector2D &targetLoc, Sprite &sprite, const Color &color)
{
	sf::Sprite temp(sprite.getTexture()->mBitmap , sf::IntRect((int)sprite.getSourceLoc().getX(), (int)sprite.getSourceLoc().getY(), (int)sprite.getWidth(), (int)sprite.getHeight()));
	temp.setPosition(sf::Vector2f(targetLoc.getX() , targetLoc.getY()));
	temp.setColor(color.mColor);
	temp.setColor(sf::Color(temp.getColor().r, temp.getColor().g, temp.getColor().b, (sf::Uint8)sprite.getTransparency()));
	mDisplay.draw(temp);
}

void GraphicsSystem::drawReverse(const Vector2D &targetLoc, Sprite &sprite, const Vector2D &scale)
{
	sf::Sprite temp(sprite.getTexture()->mBitmap, sf::IntRect((int)sprite.getSourceLoc().getX(), (int)sprite.getSourceLoc().getY(), (int)sprite.getWidth(), (int)sprite.getHeight()));
	temp.setPosition(sf::Vector2f(targetLoc.getX(), targetLoc.getY()));
	temp.scale(scale.getX(), scale.getY());
	//only supports unscaled drawing for reflected sprites
	if(scale.getX() == -1)
	{
		temp.move(sprite.getWidth(), 0);
	}
	if(scale.getY() == -1)
	{
		temp.move(0, sprite.getHeight());
	}
	if(sprite.getColor())
	{
		temp.setColor(sf::Color(sprite.getColor()->mColor));
	}
	temp.setColor(sf::Color(temp.getColor().r, temp.getColor().g, temp.getColor().b, (sf::Uint8)sprite.getTransparency()));
	mDisplay.draw(temp);
}

void GraphicsSystem::drawReflect(const Vector2D &targetLoc, Sprite &sprite, const Vector2D &scale, float theta)
{
	sf::Sprite temp(sprite.getTexture()->mBitmap, sf::IntRect((int)sprite.getSourceLoc().getX(), (int)sprite.getSourceLoc().getY(), (int)sprite.getWidth(), (int)sprite.getHeight()));
	float degrees = theta;/* * 180.0f / 3.1415926f;*/
	const Vector2D offset(targetLoc.getX() - sprite.getWidth() / 2, targetLoc.getY() + sprite.getHeight() / 2);
	setPosition(temp, offset);
	temp.setOrigin((sprite.getWidth()) / 2, (sprite.getHeight()) / 2);
	temp.rotate(degrees);
	//temp.setPosition(sf::Vector2f(targetLoc.getX(), targetLoc.getY()));
	temp.scale(scale.getX(), scale.getY());
	if(sprite.getColor())
	{
		temp.setColor(sf::Color(sprite.getColor()->mColor));
	}
	temp.setColor(sf::Color(temp.getColor().r, temp.getColor().g, temp.getColor().b, (sf::Uint8)sprite.getTransparency()));
	mDisplay.draw(temp);
}

void GraphicsSystem::handleEvent(const Event &theEvent)
{
	if(theEvent.getType() == MOVE_PLAYER_EVENT)
	{
		mCameraPosition = static_cast<const MovePlayerEvent&>(theEvent).getPlayerPosition();
	}
}

void GraphicsSystem::flip()
{
	mDisplay.display();
}

void GraphicsSystem::update()
{
	sf::View view(sf::Vector2f(mCameraPosition.getX(), mCameraPosition.getY()), sf::Vector2f(mWidth, mHeight));
	mDisplay.setView(view);
	mTopLeft = Vector2D(view.getCenter().x - mWidth / 2, view.getCenter().y - mHeight / 2);
}

void GraphicsSystem::writeText(const Vector2D &targetLoc, Font &font, Color &color, std::string &message){
	sf::Text temp(message, font.mFont);
	setPosition(temp, targetLoc);
	temp.setFillColor(color.mColor);
	temp.setCharacterSize(font.getFontSize());
	mDisplay.draw(temp);
}

void GraphicsSystem::setHeight(float h)
{
	mHeight = h;
}

void GraphicsSystem::setWidth(float w)
{
	mWidth = w;
}

Vector2D GraphicsSystem::getTopLeft()
{
	return mTopLeft;
}

float GraphicsSystem::getHeight()
{
	return mHeight;
}

float GraphicsSystem::getWidth()
{
	return mWidth;
}
