#pragma once
#include <allegro5\allegro.h>
#include "allegro5\allegro_font.h"
#include "allegro5\allegro_ttf.h"
#include <string>
#include "Trackable.h"

class Font : public Trackable{
	friend class GraphicsSystem;
public:
	Font();

	Font(std::string &fontPath, int fs);

	~Font();

	void loadFont(std::string &fontPath, int fs);

	//getters
	int getWidth(std::string message);
	int getFontSize();
private:
	int mFontSize;

	ALLEGRO_FONT *mpFont;
};