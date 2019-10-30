#pragma once
#include "graphicsBuffer.h"
#include "Vector2D.h"

class Sprite : public Trackable
{
public:
	Sprite();

	Sprite(GraphicsBuffer &g);

	~Sprite();

	void setHeight(int h);

	void setWidth(int w);

	void setSourceLoc(int x, int y);

	void setTexture(GraphicsBuffer *g);

	int getHeight();

	int getWidth();

	Vector2D getSourceLoc();

	GraphicsBuffer *getTexture();
private:
	Vector2D mSourceLoc;
	int mWidth, mHeight;

	GraphicsBuffer *mTexture;
};