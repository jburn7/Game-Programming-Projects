#include "graphicsSystem.h"

GraphicsSystem::GraphicsSystem()
{
	mDisplay = NULL;
	mWidth = 0;
	mHeight = 0;
}

GraphicsSystem::~GraphicsSystem()
{
	cleanup();
}

void GraphicsSystem::init(int w, int h)
{
	//initialize systems
	if (!al_init())
	{
		std::cout << "This could be a problem... Allegro didn't initialize properly.\n";
	}

	if (!al_init_image_addon())
	{
		std::cout << "This could be a problem... Allegro couldn't initialize the image addon.\n";
	}

	al_init_font_addon();

	if(!al_init_ttf_addon())
	{
		std::cout << "This could be a problem... Allegro couldn't initialize the ttf addon.\n";
	}

	mDisplay = al_create_display(w, h);
	mWidth = w;
	mHeight = h;
}

void GraphicsSystem::cleanup()
{
	if(mDisplay)
	{
		al_destroy_display(mDisplay);
		mDisplay = NULL;
	}
}

void GraphicsSystem::clear()
{
	al_clear_to_color(al_map_rgb(0, 0, 0));
}

void GraphicsSystem::draw(const Vector2D &targetLoc, Sprite &sprite)
{
	al_draw_bitmap_region(sprite.getTexture()->mBitmap, sprite.getSourceLoc().getX(), sprite.getSourceLoc().getY(), sprite.getWidth(), sprite.getHeight(), targetLoc.getX(), targetLoc.getY(), 0);
}

//void GraphicsSystem::draw(const Vector2D &targetLoc, Sprite &sprite, const double &scale)
//{
//	al_draw_scaled_bitmap(sprite.getTexture()->mBitmap, sprite.getSourceLoc().getX(), sprite.getSourceLoc().getY(), sprite.getWidth(), sprite.getHeight(), targetLoc.getX(), targetLoc.getY(), sprite.getWidth() * scale, sprite.getHeight() * scale, 0);
//}

void GraphicsSystem::draw(const Vector2D &targetLoc, Sprite &sprite, double theta)
{
	ALLEGRO_TRANSFORM *t = new ALLEGRO_TRANSFORM;
	al_identity_transform(t);
	al_translate_transform(t, -targetLoc.getX() - sprite.getWidth() / 2, -targetLoc.getY() -sprite.getHeight() / 2);
	al_rotate_transform(t, theta);
	al_translate_transform(t, targetLoc.getX() + sprite.getWidth() / 2, targetLoc.getY() + sprite.getHeight() / 2);
	al_use_transform(t);
	al_draw_bitmap_region(sprite.getTexture()->mBitmap, sprite.getSourceLoc().getX(), sprite.getSourceLoc().getY(), sprite.getWidth(), sprite.getHeight(), targetLoc.getX(), targetLoc.getY(), 0);
	ALLEGRO_TRANSFORM t2;
	al_identity_transform(&t2);
	al_use_transform(&t2);
	delete t;

	/*ALLEGRO_BITMAP *b;
	b = al_create_sub_bitmap(sprite.getTexture()->mBitmap, sprite.getSourceLoc().getX(), sprite.getSourceLoc().getY(), sprite.getWidth(), sprite.getHeight());
	al_draw_rotated_bitmap(b, (targetLoc.getX() + sprite.getWidth()) / 2, (targetLoc.getY() + sprite.getHeight()) / 2, sprite.getWidth() / 2 + targetLoc.getX(), sprite.getHeight() / 2, theta, 0);
	al_destroy_bitmap(b);*/
}

void GraphicsSystem::draw(GraphicsBuffer &targetBuffer, const Vector2D &targetLoc, Sprite &sprite)
{
	al_set_target_bitmap(targetBuffer.mBitmap);
	al_draw_bitmap_region(sprite.getTexture()->mBitmap, sprite.getSourceLoc().getX(), sprite.getSourceLoc().getY(), sprite.getWidth(), sprite.getHeight(), targetLoc.getX(), targetLoc.getY(), 0);
	al_set_target_bitmap(al_get_backbuffer(mDisplay));
}

void GraphicsSystem::draw(GraphicsBuffer &targetBuffer, const Vector2D &targetLoc, Sprite &sprite, const double &scale)
{
	al_set_target_bitmap(targetBuffer.mBitmap);
	al_draw_scaled_bitmap(sprite.getTexture()->mBitmap, sprite.getSourceLoc().getX(), sprite.getSourceLoc().getY(), sprite.getWidth(), sprite.getHeight(), targetLoc.getX(), targetLoc.getY(), sprite.getWidth() * scale, sprite.getHeight() * scale, 0);
	al_set_target_bitmap(al_get_backbuffer(mDisplay));
}

void GraphicsSystem::flip()
{
	al_flip_display();
	//IMPORTANT: when below is commented out, the objects on screen duplicate themselves, though no new objects are being created so there shouldn't be any memory worries
	//if there does seem to be a problem, then that means this workaround backfired. but for now, this works
	//the odd part is that this code was not needed for assignment 4
	//al_clear_to_color(al_map_rgb(0 , 0 , 0));
}

void GraphicsSystem::writeText(const Vector2D &targetLoc, Font &font, Color &color, std::string &message){
	al_draw_text(font.mpFont, color.getOwnColor(), targetLoc.getX(), targetLoc.getY(), 0, message.c_str());
}

//void GraphicsSystem::writeText(GraphicsBuffer &targetBuffer, const Vector2D &targetLoc, Font &font, Color &color, const std::string &message){
//	al_set_target_bitmap(targetBuffer.mBitmap);
//	al_draw_text(font.font, color.getOwnColor(), targetLoc.getX(), targetLoc.getY(), 0, message.c_str());
//	al_set_target_bitmap(al_get_backbuffer(mDisplay));
//}

void GraphicsSystem::setHeight(int h)
{
	mHeight = h;
}

void GraphicsSystem::setWidth(int w)
{
	mWidth = w;
}

ALLEGRO_BITMAP *GraphicsSystem::getBackbuffer()
{
	return al_get_backbuffer(mDisplay);
}

int GraphicsSystem::getHeight()
{
	return mHeight;
}

int GraphicsSystem::getWidth()
{
	return mWidth;
}
