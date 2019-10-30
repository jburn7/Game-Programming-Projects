#include "graphicsBuffer.h"

GraphicsBuffer::GraphicsBuffer()
{
	mWidth = mHeight = 0;
}

GraphicsBuffer::GraphicsBuffer(Color &color, float w, float h)
{
	create(w, h);
	mWidth = w;
	mHeight = h;
}

GraphicsBuffer::GraphicsBuffer(std::string &resource)
{
	mBitmap.loadFromFile(resource);

	mWidth = (float)mBitmap.getSize().x;
	mHeight = (float)mBitmap.getSize().y;
}

GraphicsBuffer::~GraphicsBuffer()
{
}

void GraphicsBuffer::create(float w, float h)
{
	mBitmap.create((unsigned int)w, (unsigned int)h);
}

void GraphicsBuffer::setHeight(float h)
{
	mHeight = h;
}

void GraphicsBuffer::setWidth(float w)
{
	mWidth = w;
}

float GraphicsBuffer::getHeight()
{
	return mHeight;
}

float GraphicsBuffer::getWidth()
{
	return mWidth;
}