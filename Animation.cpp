#include "Animation.h"

void Animation::Update(float deltaTime)
{
    if (frames.empty() || finished)
        return;

    timer += deltaTime;

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

const SDL_FRect& Animation::GetCurrentFrame() const
{
    // Asumimos que siempre se llama cuando hay frames
    return frames[currentFrame];
}

void Animation::Reset()
{
    currentFrame = 0;
    timer = 0.0f;
    finished = false;
}