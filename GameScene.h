#pragma once
#include "Scene.h"

class GameScene : public Scene
{
public:
    GameScene(GraphicsInterface* GI, MyPhysics* mph);

    void Update(float dt) override;
    void Render() override;

private:
    class Player* player = nullptr;
};
