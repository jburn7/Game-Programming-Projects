#pragma once
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include "sprite.h"
#include "font.h"

class GraphicsSystem : public Trackable
{
public:
	GraphicsSystem();

	~GraphicsSystem();

	void init(int w, int h);

	void cleanup();

	void clear();

	void draw(const Vector2D &targetLoc, Sprite &sprite);

	void draw(const Vector2D &targetLoc, Sprite &sprite, double theta);

	void draw(GraphicsBuffer &targetBuffer, const Vector2D &targetLoc, Sprite &sprite);

	void draw(GraphicsBuffer &targetBuffer, const Vector2D &targetLoc, Sprite &sprite, const double &scale);

	void flip();

	void writeText(const Vector2D &targetLoc, Font &font, Color &color, std::string &message);

	//setters and getters
	void setHeight(int h);

	void setWidth(int w);

	int getHeight();

	int getWidth();
private:
	ALLEGRO_BITMAP *getBackbuffer();

	int mWidth, mHeight;

	ALLEGRO_DISPLAY *mDisplay;
};