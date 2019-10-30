#include "sprite.h"

Sprite::Sprite()
{
	mWidth = 0;
	mHeight = 0;
	mSourceLoc.setX(0);
	mSourceLoc.setY(0);
	mTexture = NULL;
}

Sprite::Sprite(GraphicsBuffer &g)
{
	mTexture = &g;
	mWidth = g.getWidth();
	mHeight = g.getHeight();
	mSourceLoc.setX(0);
	mSourceLoc.setY(0);
}

Sprite::~Sprite()
{
}

void Sprite::setHeight(int h)
{
	mHeight = h;
}

void Sprite::setWidth(int w)
{
	mWidth = w;
}

void Sprite::setSourceLoc(int x, int y)
{
	mSourceLoc.setX(x);
	mSourceLoc.setY(y);
}

void Sprite::setTexture(GraphicsBuffer *g)
{
	mTexture = g;
}

int Sprite::getHeight()
{
	return mHeight;
}

int Sprite::getWidth()
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