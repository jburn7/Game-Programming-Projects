#include "uiBox.h"
#include "game.h"

UIBox::UIBox(Vector2D pos, float width, float height, Vector2D textOffset, const std::string &text, const std::string &function, Sprite *sprite)
{
	mPosition = pos;
	mWidth = width;
	mHeight = height;
	mTextOffset = textOffset;
	mText = text;
	mFunction = function;
	mBackground = sprite;
	mBackground->setTransparency(200);

	mScale = Vector2D(mWidth / mBackground->getWidth(), mHeight / mBackground->getHeight());
}

UIBox::UIBox(const UIBox &other)
{
	mPosition = other.mPosition;
	mWidth = other.mWidth;
	mHeight = other.mHeight;
	mText = other.mText;
	mFunction = other.mFunction;
	if(mBackground)
	{
		//delete mBackground;
	}
	mBackground = new Sprite(*other.mBackground);
	mScale = other.mScale;
	mTextOffset = other.mTextOffset;
	mColor = other.mColor;
}

UIBox::~UIBox()
{
	if(mBackground)
	{
		delete mBackground;
		mBackground = NULL;
	}
}

bool UIBox::contains(Vector2D pos)
{
	return pos.getX() > mPosition.getX() && pos.getX() < mPosition.getX() + mWidth && pos.getY() > mPosition.getY() && pos.getY() < mPosition.getY() + mHeight;
}

void UIBox::draw(Vector2D pos, Font &font, Color &color)
{
	Game::getInstance()->getGraphics().drawReverse(mPosition + pos, *mBackground, mScale);
	Game::getInstance()->getGraphics().writeText(mPosition + pos + mTextOffset, font, color, mText);
}
