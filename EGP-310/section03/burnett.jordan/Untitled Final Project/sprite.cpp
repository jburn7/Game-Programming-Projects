#include "sprite.h"

Sprite::Sprite()
{
	mWidth = 0;
	mHeight = 0;
	mSourceLoc.setX(0);
	mSourceLoc.setY(0);
	mTexture = NULL;
	mTransparency = 100;
	mColor = NULL;
}

Sprite::Sprite(GraphicsBuffer &g)
{
	mTexture = &g;
	mWidth = g.getWidth();
	mHeight = g.getHeight();
	mSourceLoc.setX(0);
	mSourceLoc.setY(0);
	mTransparency = 255;
	mColor = NULL;
}

Sprite::~Sprite()
{
}

void Sprite::setColor(Color *color)
{
	if(mColor)
	{
		delete mColor;
	}
	mColor = color;
}

void Sprite::setHeight(float h)
{
	mHeight = h;
}

void Sprite::setWidth(float w)
{
	mWidth = w;
}

void Sprite::setSourceLoc(float x, float y)
{
	mSourceLoc.setX(x);
	mSourceLoc.setY(y);
}

void Sprite::setTexture(GraphicsBuffer *g)
{
	mTexture = g;
}

void Sprite::setTransparency(float newTransparency)
{
	if(newTransparency < 0)
	{
		newTransparency = 0;
	}
	else if(newTransparency > 1)
	{
		newTransparency = 1;
	}
	mTransparency = 255 * newTransparency;
}

float Sprite::getHeight()
{
	return mHeight;
}

float Sprite::getWidth()
{
	return mWidth;
}

Vector2D Sprite::getSourceLoc()
{
	return mSourceLoc;
}

GraphicsBuffer *Sprite::getTexture()
{
	return mTexture;
}