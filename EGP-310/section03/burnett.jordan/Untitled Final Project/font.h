#pragma once
#include "SFML\Graphics\Font.hpp"
#include "SFML\Graphics\Text.hpp"
#include <string>
#include "Trackable.h"

class Font : public Trackable{
	friend class GraphicsSystem;
public:
	Font();

	Font(const std::string &fontPath, int fs);

	~Font();

	void loadFont(const std::string &fontPath, int fs);

	//getters
	int getWidth(std::string message);
	int getFontSize();
private:
	int mFontSize;

	sf::Font mFont;
};