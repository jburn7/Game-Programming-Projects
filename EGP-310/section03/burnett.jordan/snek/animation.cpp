#include "animation.h"

Animation::Animation()
{
	mMsPerFrame = 100;
	mTimeOfLastFrame = 0;
	mCurrentFrame = 0;
}

Animation::Animation(const Animation &other)
{
	mLoop = other.mLoop;
	mMsPerFrame = other.mMsPerFrame;
	mTimeOfLastFrame = other.mTimeOfLastFrame;
	mCurrentFrame = other.mCurrentFrame;
	for(unsigned int i = 0; i < other.mFrames.size(); i++)
	{
		mFrames.push_back(new Sprite(*other.mFrames[i]));
	}
}

Animation::Animation(std::vector<Sprite*>& frames) : Animation()
{
	mFrames = frames;
}

Animation::~Animation()
{
	cleanup();
}

void Animation::cleanup()
{
	for(unsigned int i = 0; i < mFrames.size(); i++)
	{
		if(mFrames[i])
		{
			delete mFrames[i];
			mFrames[i] = NULL;
		}
	}
	mFrames.clear();
}

void Animation::changeSpeed(double deltaSpeed)
{
	mMsPerFrame -= deltaSpeed;
	if(mMsPerFrame <= 0)
	{
		mMsPerFrame = 1;
	}
}

void Animation::setSpeed(int newMs)
{
	mMsPerFrame = newMs;
}

void Animation::update(double timeElapsed)
{
	if(timeElapsed - mTimeOfLastFrame > mMsPerFrame)
	{
		mCurrentFrame++;
		if(mCurrentFrame >= mFrames.size())
		{
			mCurrentFrame = 0;
		}
		mTimeOfLastFrame = timeElapsed;
	}
}

Sprite *Animation::getCurrentSprite()
{
	return mFrames[mCurrentFrame];
}
