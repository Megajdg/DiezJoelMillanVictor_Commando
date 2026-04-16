#include "Animation.h"

void Animation::Update(float dt)
{
    if (finished || frames.empty())
        return;

    timer += dt;

    while (timer >= frameTime)
    {
        timer -= frameTime;
        currentFrame++;

        if (currentFrame >= (int)frames.size())
        {
            if (loop)
            {
                currentFrame = 0;
            }
            else
            {
                currentFrame = (int)frames.size() - 1;
                finished = true;
                break;
            }
        }
    }
}

Frame Animation::GetCurrentFrame() const
{
    return frames[currentFrame];
}

void Animation::Reset()
{
    currentFrame = 0;
    timer = 0.0f;
    finished = false;
}