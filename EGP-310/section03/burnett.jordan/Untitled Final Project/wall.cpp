#include "wall.h"
#include "Entity.h"
#include "game.h"

Wall::Wall(Vector2D pos, Sprite *s, float width) : Unit(pos, s)
{
	mType = U_WALL;
	mWidth = width * Unit::getWidth();

	mScale = mWidth / Unit::getWidth();

	loadHitbox();
}

Wall::~Wall()
{
}

void Wall::draw()
{
	Game::getInstance()->getGraphics().drawReverse(mPosition, *mAnimationMap[mCurrentAnimation]->getCurrentSprite(), Vector2D(mScale, 1.f));
}

void Wall::collide(Entity *o)
{
	//std::cout << "Wall on Entity collision.\n";
	o->collide(this);
}
