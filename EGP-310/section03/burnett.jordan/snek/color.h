#pragma once
#include "allegro5\allegro.h"
#include "allegro5\allegro_color.h"
#include "Trackable.h"

class GraphicsBuffer;

class Color : public Trackable
{
	friend class GraphicsSystem;
public:
	Color();
	Color(int red, int green, int blue);
	~Color();

	void setFillColor(GraphicsBuffer &target);
private:
	ALLEGRO_COLOR Color::getOwnColor() { return al_map_rgb(r, g, b); }

	int r, g, b;

	ALLEGRO_COLOR color;
};