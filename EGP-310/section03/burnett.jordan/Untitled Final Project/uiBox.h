#pragma once
#include "Vector2D.h"
#include "sprite.h"
#include "font.h"

class UIBox
{
public:
	UIBox(Vector2D pos, float width, float height, Vector2D textOffset, const std::string &text, const std::string &function, Sprite *sprite);
	UIBox(const UIBox &other);
	~UIBox();

	bool contains(Vector2D pos);

	void draw(Vector2D pos, Font &font, Color &color);

	std::string &getFunction(){ return mFunction; }
private:
	Vector2D mPosition;
	float mWidth, mHeight;
	std::string mText, mFunction;
	Sprite *mBackground;
	Vector2D mScale;
	Vector2D mTextOffset;
	Color mColor;
};