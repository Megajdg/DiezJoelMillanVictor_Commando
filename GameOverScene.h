#pragma once
#include "Scene.h"

class GameOverScene : public Scene
{
public:
    GameOverScene(GraphicsInterface* GI, MyPhysics* mph);

    void Update(float dt) override;
    void Render() override;
};
