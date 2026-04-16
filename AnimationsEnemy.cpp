#include "AnimationSet.h"
#include "Animation.h"

AnimationSet LoadEnemyAnimations()
{
    AnimationSet set;

    float frameW = 24;
    float frameH = 21;

    float startX = 3;
    float startY = 11;

    float sepX = 6;
    float sepY[4] = { 3, 3, 3, 3 };

    auto rowY = [&](int row)
        {
            float y = startY;
            for (int i = 0; i < row; i++)
                y += frameH + sepY[i];
            return y;
        };

    auto AddAnim = [&](const std::string& name, int row, int colStart, int colEnd, float frameTime = 0.1f, bool loop = true)
        {
            Animation* a = new Animation();
            a->name = name;
            a->frameTime = frameTime;
            a->loop = loop;

            float y = rowY(row);
            for (int i = colStart; i <= colEnd; i++)
                a->frames.push_back({ startX + i * (frameW + sepX), y, frameW, frameH });

            set.anims[name] = a;
        };

    // RUN
    AddAnim("run_diag_front_left", 0, 0, 3);
    AddAnim("run_front", 0, 4, 7);
    AddAnim("run_diag_front_right", 0, 8, 11);

    AddAnim("run_left", 1, 0, 3);
    AddAnim("run_right", 1, 8, 11);

    AddAnim("run_diag_back_left", 2, 0, 3);
    AddAnim("run_back", 2, 4, 7);
    AddAnim("run_diag_back_right", 2, 8, 11);

    // THROW
    AddAnim("throw", 3, 0, 2, 0.12f);

    // DEATH
    {
        Animation* a = new Animation();
        a->name = "death";
        a->frameTime = 0.15f;
        a->loop = false;

        float y = rowY(3);
        a->frames.push_back({ 107, y, frameW, frameH });
        a->frames.push_back({ 137, y, frameW, frameH });

        set.anims["death"] = a;
    }

    // IDLES
    {
        Animation* a = new Animation();
        a->name = "idle_front";
        a->frameTime = 0.2f;
        a->frames.push_back({ 183, rowY(0), frameW, frameH });
        set.anims["idle_front"] = a;
    }

    {
        Animation* a = new Animation();
        a->name = "idle_back";
        a->frameTime = 0.2f;
        a->frames.push_back({ 183, rowY(2), frameW, frameH });
        set.anims["idle_back"] = a;
    }

    {
        Animation* a = new Animation();
        a->name = "idle_left";
        a->frameTime = 0.2f;
        a->frames.push_back({ startX, rowY(1), frameW, frameH });
        set.anims["idle_left"] = a;
    }

    {
        Animation* a = new Animation();
        a->name = "idle_right";
        a->frameTime = 0.2f;
        a->frames.push_back({ startX + 8 * (frameW + sepX), rowY(1), frameW, frameH });
        set.anims["idle_right"] = a;
    }

    return set;

}