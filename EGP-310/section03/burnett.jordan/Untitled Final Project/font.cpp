#include "font.h"

Font::Font()
{
	mFontSize = 0;
}

Font::Font(const std::string &fontPath, int fs)
{
	loadFont(fontPath, fs);
}

Font::~Font()
{
}

void Font::loadFont(const std::string &fontPath, int fs)
{
	mFontSize = fs;
	mFont.loadFromFile(fontPath);
}

int Font::getWidth(std::string message)
{
	sf::Text text(message, mFont, mFontSize);
	return (int)text.getGlobalBounds().width;
}

int Font::getFontSize(){
	return mFontSize;
}
