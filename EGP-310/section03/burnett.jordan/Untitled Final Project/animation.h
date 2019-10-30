#pragma once
#include "sprite.h"
#include <vector>

class Animation : public Trackable
{
public:
	Animation();
	Animation(const Animation &other);
	Animation(std::vector<Sprite*> &frames);

	~Animation();

	void cleanup();

	void changeSpeed(int deltaSpeed);

	void setColor(Color *color);

	void setSpeed(int newMs);

	//1 = opaque, 0 = invis
	void setTransparency(float newTransparency);

	void update(int timeElapsed);

	Sprite *getCurrentSprite();

	unsigned int getCurrentFrameNumber(){ return mCurrentFrame; }
private:
	bool mLoop;
	int mMsPerFrame;
	int mTimeOfLastFrame;
	unsigned int mCurrentFrame;

	std::vector<Sprite*> mFrames;
};