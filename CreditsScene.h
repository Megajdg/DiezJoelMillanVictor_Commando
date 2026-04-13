#pragma once
#include "Scene.h"

class CreditsScene : public Scene
{
public:
    CreditsScene(GraphicsInterface* GI, MyPhysics* mph);

    void Update(float dt) override;
    void Render() override;
};
