#pragma once
#include "Scene.h"

class AreaCompleteScene : public Scene
{
public:
    AreaCompleteScene(GraphicsInterface* GI, MyPhysics* mph, int areaNumber);

    void Update(float dt) override;
    void Render() override;

private:
    float timer = 0.0f;
    int area = 1;
};