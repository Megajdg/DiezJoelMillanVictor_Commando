#pragma once
#include "Scene.h"

class VictoryScene : public Scene
{
public:
    VictoryScene(GraphicsInterface* GI, MyPhysics* mph);

    void Update(float dt) override;
    void Render() override;

private:
    float timer = 0.0f;
};