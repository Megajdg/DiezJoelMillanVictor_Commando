#pragma once
#include <vector>
#include <string>

struct Frame
{
    float x, y, w, h;
};

class Animation
{
public:
    std::string name;
    std::vector<Frame> frames;

    float frameTime = 0.1f;
    float timer = 0.f;
    int currentFrame = 0;

    bool loop = true;
    bool finished = false;

    void Reset();
    void Update(float dt);
    Frame GetCurrentFrame() const;
};
