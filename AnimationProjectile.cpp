#include "AnimationSet.h"
#include "Animation.h"

AnimationSet LoadProjectileAnimations()
{
    AnimationSet set;

    float frameW = 24;
    float frameH = 21;

    float startX = 2;
    float startY = 2;

    float sepX = 0;
    float sepY = 3;

    auto AddAnim = [&](std::string name, int row, int colStart, int colEnd, float frameTime = 0.05f)
        {
            Animation* a = new Animation();
            a->name = name;
            a->frameTime = frameTime;
            a->loop = true;

            float y = startY + row * (frameH + sepY);

            for (int i = colStart; i <= colEnd; i++)
                a->frames.push_back({ startX + i * (frameW + sepX), y, frameW, frameH });

            set.anims[name] = a;
        };

    // Ejemplo:
    // AddAnim("bullet", 0, 0, 0);
    // AddAnim("grenade", 0, 0, 0);
    AddAnim("explosion", 0, 7, 9, .08f);

    return set;
}