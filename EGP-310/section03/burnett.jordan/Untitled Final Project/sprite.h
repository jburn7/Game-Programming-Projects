#pragma once
#include "graphicsBuffer.h"
#include "Vector2D.h"

class Sprite : public Trackable
{
public:
	Sprite();

	Sprite(GraphicsBuffer &g);

	~Sprite();

	void setColor(Color *color);

	void setHeight(float h);

	void setWidth(float w);

	void setSourceLoc(float x, float y);

	void setTexture(GraphicsBuffer *g);

	//1 = opaque, 0 = invisible
	void setTransparency(float newTransparency);

	Color *getColor() { return mColor; }

	float getHeight();

	float getTransparency(){ return mTransparency; }

	float getWidth();

	Vector2D getSourceLoc();

	GraphicsBuffer *getTexture();
private:
	Vector2D mSourceLoc;
	float mWidth, mHeight;
	float mTransparency;
	Color *mColor;

	GraphicsBuffer *mTexture;
};