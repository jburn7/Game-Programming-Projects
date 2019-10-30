#include "Color.h"
#include "graphicsBuffer.h"

Color::Color()
{
}

Color::Color(int red, int green, int blue)
{
	r = red;
	g = green;
	b = blue;
	if(r < 0 || r > 255)
	{
		r = 0;
	}
	if(g < 0 || g > 255)
	{
		g = 0;
	}
	if(b < 0 || b > 255)
	{
		b = 0;
	}
}

Color::~Color()
{
}

void Color::setFillColor(GraphicsBuffer &target)
{
	ALLEGRO_BITMAP *oldTarget = al_get_target_bitmap();
	al_set_target_bitmap(target.mBitmap);
	al_clear_to_color(al_map_rgb(r, g, b));
	al_set_target_bitmap(oldTarget);
}