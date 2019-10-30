#include "font.h"

Font::Font(){
	mpFont = NULL;
	mFontSize = 0;
}

Font::Font(std::string &fontPath, int fs){
	loadFont(fontPath, fs);
}

Font::~Font(){
	if(mpFont){
		al_destroy_font(mpFont);
		mpFont = NULL;
	}
}

void Font::loadFont(std::string &fontPath, int fs)
{
	mFontSize = fs;
	mpFont = al_load_font(fontPath.c_str(), mFontSize, 0);
}

int Font::getWidth(std::string message)
{
	return  al_get_text_width(mpFont, message.c_str());
}

int Font::getFontSize(){
	return mFontSize;
}
