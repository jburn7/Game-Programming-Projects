#include "graphicsBuffer.h"

GraphicsBuffer::GraphicsBuffer()
{
	mBitmap = NULL;
	mWidth = mHeight = 0;
}

GraphicsBuffer::GraphicsBuffer(Color &color, int w, int h)
{
	create(w, h);
	mWidth = w;
	mHeight = h;
	color.setFillColor(*this);
}

GraphicsBuffer::GraphicsBuffer(std::string &resource)
{
	mBitmap = al_load_bitmap(resource.c_str());
	if(mBitmap)
	{
		mWidth = al_get_bitmap_width(mBitmap);
		mHeight = al_get_bitmap_height(mBitmap);
	}
	else
	{
		std::cout << "Failed to load " << resource << std::endl;
	}
}

GraphicsBuffer::~GraphicsBuffer()
{
	if(mBitmap)
	{
		al_destroy_bitmap(mBitmap);
		mBitmap = NULL;
	}
}

void GraphicsBuffer::create(int w, int h)
{
	mBitmap = al_create_bitmap(w, h);
}

void GraphicsBuffer::setHeight(int h)
{
	mHeight = h;
}

void GraphicsBuffer::setWidth(int w)
{
	mWidth = w;
}

int GraphicsBuffer::getHeight()
{
	return al_get_bitmap_height(mBitmap);
}

int GraphicsBuffer::getWidth()
{
	return al_get_bitmap_width(mBitmap);
}