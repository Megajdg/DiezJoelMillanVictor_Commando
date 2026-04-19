#include "AnimationSet.h"
#include "Animation.h"
#include "AnimationPowerUp.h"



AnimationSet LoadPowerUpAnimations()
{

    AnimationSet set;

    float frameW = 26;
    float frameH = 21;

    float startX = 2;
    float startY = 2;

    float sepX = 4;
    float sepY = 0;

    auto AddAnim = [&](const std::string& name, int row, int colStart, int colEnd, float frameTime = 0.08f, bool loop = true)
        {
            Animation* a = new Animation();
            a->name = name;
            a->frameTime = frameTime;
            a->loop = loop;

            float y = startY + row * (frameH + sepY);

            for (int i = colStart; i <= colEnd; i++)
            {
                float x = startX + i * (frameW + sepX);
                a->frames.push_back({ x, y, frameW, frameH });
            }

            set.anims[name] = a;
        };


    AddAnim("grenadePU", 0, 0, 1, 0.2f);


    return set;
}