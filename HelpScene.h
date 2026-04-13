#pragma once
#include "Scene.h"

class HelpScene : public Scene
{
public:
    HelpScene(GraphicsInterface* GI, MyPhysics* mph);

    void Update(float dt) override;
    void Render() override;
};
